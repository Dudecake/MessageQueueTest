//
// Created by dudecake on 30/12/17.
//
#include <QtDebug>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <thread>
#include <ostream>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include "program.h"

#ifdef ACTIVEMQ
#include <cms/BytesMessage.h>
#endif

int64_t Program::messageCount = 1000;

Program::Program(std::string dataDir, QObject *parent) :
        QObject(parent),
        dataDir(dataDir),
#ifdef RABBITMQ
        r("foo.test", false, false, true, this),
        queue(r, QString(), "0", false, false, true, true, this),
#else
        r("foo.test", "0", this),
#endif
        inMessId(0)
{
#ifdef RABBITMQ
    connect(&queue, &messageQueueProvider::ReceivingQueue::messageReceived, this, &Program::handleMessage, Qt::QueuedConnection);
#else
    connect(&r, &messageQueueProvider::Topic::messageReceived, this, &Program::handleMessage, Qt::QueuedConnection);
#endif
    std::time_t startTime = std::chrono::high_resolution_clock::to_time_t(std::chrono::high_resolution_clock::now());
    std::stringstream ss;
    ss << '-' << std::put_time(std::localtime(&startTime), "%F_%T") << '-';
    dateString = ss.str();
    csvMessOut.set_delimiter(';', std::string());
    csvMessIn.set_delimiter(';', std::string());
    csvMessOut << "messId" << "sendTime (ms)" << NEWLINE;
    csvMessIn << "messId" << "recvTime (ms)" << NEWLINE;
}

void Program::run()
{
    using std::chrono::duration;
    using std::chrono::time_point;
    using std::chrono::high_resolution_clock;

    messageQueueProvider::Topic s("foo.test", this);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    inMessInterval = high_resolution_clock::now();
    time_point<high_resolution_clock> messStart;
    time_point<high_resolution_clock> start = high_resolution_clock::now();
    for (int i = 0; i < messageCount; i++)
    {
        messStart = high_resolution_clock::now();
        s.sendMessage(QString().sprintf("Hallo, dit is testbericht: %i", i).toStdString(), std::string("0"));
        csvMessOut << i << duration<double, std::milli>(high_resolution_clock::now() - messStart).count() << NEWLINE;
    }
    qDebug() << "Sent messages in" << duration<double, std::milli>(high_resolution_clock::now() - start).count() << "ns";
    std::stringstream ss;
    ss << dataDir << "messageOut" << dateString << AMQP_PROVIDER << ".csv";
    std::ofstream csvFile(ss.str(), std::ios_base::trunc);
    csvFile << csvMessOut.get_text();
    QTimer::singleShot(2500, [&]{ QCoreApplication::exit(0); });
}

void Program::handleMessage(messageQueueProvider::Envelope envelope)
{
    using std::chrono::duration;
    using std::chrono::high_resolution_clock;

#ifdef RABBITMQ
    const AmqpClient::BasicMessage::ptr_t message = envelope.getMessage();
    std::string messageBody = message->Body();
    if (!messageBody.empty())
    {
        //qWarning() << "Got:" << QString::fromStdString(messageBody);
    }
    if (messageQueueProvider::ReceivingQueue *queue = qobject_cast<messageQueueProvider::ReceivingQueue*>(sender()))
    {
        queue->notify();
    }
#elif ACTIVEMQ
    const cms::BytesMessage *bytesMessage = dynamic_cast<const cms::BytesMessage*>(envelope.getMessage());
    if (bytesMessage != nullptr)
    {
        std::string messageBody(reinterpret_cast<const char*>(bytesMessage->getBodyBytes()), bytesMessage->getBodyLength());
        //qWarning() << "Got:" << QString::fromStdString(messageBody);
    }
    envelope.getMessage()->acknowledge();
#elif KAFKA
    std::string messageBody(reinterpret_cast<const char*>(envelope.getMessage()->payload()), envelope.getMessage()->len());
    //qWarning() << "Got:" << QString::fromStdString(messageBody);
    r.notify();
#endif
    csvMessIn << inMessId << duration<double, std::milli>(high_resolution_clock::now() - inMessInterval).count() << NEWLINE;
    inMessInterval = high_resolution_clock::now();
    inMessId++;
    if (inMessId == messageCount)
        QCoreApplication::exit(0);
}

Program::~Program()
{
    std::stringstream ss;
    ss << dataDir << "messageIn" << dateString << AMQP_PROVIDER << ".csv";
    std::ofstream csvFile(ss.str(), std::ios_base::trunc);
    csvFile << csvMessIn.get_text();
#ifdef KAFKA
    ss.str(std::string());
    ss << dataDir << "groupCounter";
    std::ofstream counterFile(ss.str(), std::ios_base::trunc);
    counterFile << (KafkaTestClient::Topic::getGroupCounter() + 1);
#endif
}
