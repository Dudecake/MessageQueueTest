//
// Created by dudecake on 30/12/17.
//
#include <QtDebug>
#include "program.h"
#ifdef ACTIVEMQ
#include <cms/BytesMessage.h>
#endif

Program::Program(QObject *parent) : QObject(parent), r("foo.test", "0", this)
{
    connect(&r, &messageQueueProvider::Topic::messageReceived, this, &Program::handleMessage, Qt::QueuedConnection);
}

void Program::run()
{
    messageQueueProvider::Topic s("foo.test", this);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (int i = 0; i < 100; i++)
    {
        s.sendMessage(QString().sprintf("Hoi: %i", i).toStdString(), std::string("0"));
    }
    qDebug() << "Sent messages";
}

void Program::handleMessage(messageQueueProvider::Envelope envelope)
{
#ifdef ACTIVEMQ
    const cms::BytesMessage *bytesMessage = dynamic_cast<const cms::BytesMessage*>(envelope.getMessage());
    if (bytesMessage != nullptr)
    {
        std::string messageBody(reinterpret_cast<const char*>(bytesMessage->getBodyBytes()), bytesMessage->getBodyLength());
        qWarning() << QString::fromStdString(messageBody);
    }
    envelope.getMessage()->acknowledge();
#elif KAFKA
    std::string messageBody(reinterpret_cast<const char*>(envelope.getMessage()->payload()), envelope.getMessage()->len());
    qDebug() << QString::fromStdString(messageBody);
    r.notify();
#endif
}
