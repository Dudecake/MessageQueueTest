//
// Created by dudecake on 01/01/18.
//
#include "kafkaclient/topic.h"
#include "kafkaclient/kafkaclient.h"
#include <QtDebug>
#include <QCoreApplication>

int KafkaTestClient::Topic::groupCounter = 0;

KafkaTestClient::Topic::Topic(std::string topicName, QObject *parent) : QObject(parent)
{
    std::string errString;
    std::shared_ptr<RdKafka::Conf> conf = KafkaClient::getConf();
    if (conf->set("group.id", "kafkatest-topic-" + std::to_string(groupCounter++), errString) != RdKafka::Conf::CONF_OK)
    {
        qWarning() << "Failed to set group.id:" << QString::fromStdString(errString);
        QCoreApplication::exit(1);
        return;
    }
    producer = std::shared_ptr<RdKafka::Producer>(RdKafka::Producer::create(conf.get(), errString));
    if (!producer)
    {
        qWarning() << "Failed to create producer:" << QString::fromStdString(errString);
        QCoreApplication::exit(1);
        return;
    }
    internalTopic = std::shared_ptr<RdKafka::Topic>(RdKafka::Topic::create(producer.get(), topicName, KafkaClient::getTopicConf().get(), errString));
    if (!internalTopic)
    {
        qWarning() << "Failed to create topic:" << QString::fromStdString(errString);
        QCoreApplication::exit(1);
        return;
    }
    producer->poll(0);
//    worker = std::unique_ptr<TopicWorker>(new TopicWorker(consumer, internalTopic, 0, this));//new ReceivingQueueWorker(connection, queue, consumerTag);
//    connect(worker.get(), &TopicWorker::messageReceived, [this](KafkaTestClient::Envelope envelope){
//        emit messageReceived(envelope);
//    });
//    worker->start();
}

KafkaTestClient::Topic::Topic(std::string topicName, std::string routingKey, QObject *parent) : QObject(parent)
{
    std::string errString;
    std::shared_ptr<RdKafka::Conf> conf = KafkaClient::getConf();
    if (conf->set("group.id", "kafkatest-topic-" + std::to_string(groupCounter++), errString) != RdKafka::Conf::CONF_OK)
    {
        qWarning() << "Failed to set group.id:" << QString::fromStdString(errString);
        QCoreApplication::exit(1);
        return;
    }
    consumer = std::shared_ptr<RdKafka::KafkaConsumer>(RdKafka::KafkaConsumer::create(conf.get(), errString));
    if (!consumer)
    {
        qWarning() << "Failed to create consumer:" << QString::fromStdString(errString);
        QCoreApplication::exit(1);
        return;
    }
//    internalTopic = std::shared_ptr<RdKafka::Topic>(RdKafka::Topic::create(consumer.get(), topicName, KafkaClient::getTopicConf().get(), errString));
//    if (!internalTopic)
//    {
//        qWarning() << "Failed to create topic:" << QString::fromStdString(errString);
//        QCoreApplication::exit(1);
//        return;
//    }
    RdKafka::ErrorCode errorCode = consumer->subscribe({ topicName });
    if (errorCode)
    {
        qWarning() << "Failed to subscribe to" << QString::fromStdString(topicName) << QString::fromStdString(RdKafka::err2str(errorCode));
    }
    qDebug() << "Subscribed to" << QString::fromStdString(topicName);
    worker = std::unique_ptr<TopicWorker>(new TopicWorker(consumer, internalTopic, std::stoi(routingKey), this));//new ReceivingQueueWorker(connection, queue, consumerTag);
    connect(worker.get(), &TopicWorker::messageReceived, [this](KafkaTestClient::Envelope envelope){
        emit messageReceived(envelope);
    });
    worker->start();
}

void KafkaTestClient::Topic::sendMessage(std::string messageBody, std::string routingKey)
{
    RdKafka::ErrorCode errorCode = producer->produce(internalTopic.get(), std::stoi(routingKey), RdKafka::Producer::RK_MSG_COPY, const_cast<char *>(messageBody.c_str()), messageBody.size(),
                                                     nullptr, nullptr);
    if (errorCode != RdKafka::ERR_NO_ERROR)
    {
        qWarning() << "Failed to send message:" << QString::fromStdString(RdKafka::err2str(errorCode));
    }
    producer->poll(0);
}

KafkaTestClient::Topic::~Topic()
{
    if (consumer)
        consumer->close();
}
