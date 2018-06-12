/**
 * @file receivingqueue.cpp
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 17/05/2017
 * @version 0.0.1
 */

#include "rabbitmqclient/receivingqueue.h"

RabbitMQTestClient::ReceivingQueue::ReceivingQueue(QString queueName, bool passive, bool durable, bool exclusive, bool autoDelete, bool noLocal, bool noAck, bool exclusiveBind, QObject *parent) : QObject(parent), bound(false)
{
    QUrl url = RabbitMQTestClient::RabbitMQClient::getBrokerUrl();
    if (url.scheme() == "ssl")
        connection = AmqpClient::Channel::CreateSecure(std::string(), url.host().toStdString(), std::string(), std::string(), 5671, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    else
        connection = AmqpClient::Channel::Create(url.host().toStdString(), 5672, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    qWarning() << "Opened" << url.scheme() << "connection to" << url.host();
    queue = connection->DeclareQueue(queueName.toStdString(), passive, durable, exclusive, autoDelete);
    consumerTag = connection->BasicConsume(queue, "", noLocal, noAck, exclusiveBind);
    workerThread = new ReceivingQueueWorker(connection, queue, consumerTag);
    connect(workerThread, &ReceivingQueueWorker::messageReceived, [&](Envelope envelope){
        emit messageReceived(envelope);
    });
    workerThread->start();
}

RabbitMQTestClient::ReceivingQueue::ReceivingQueue(Topic &topic, QString queueName, QString routingKey, bool passive, bool durable, bool exclusive, bool autoDelete, QObject *parent) : QObject(parent), bound(true)
{
    QUrl url = RabbitMQTestClient::RabbitMQClient::getBrokerUrl();
    if (url.scheme() == "ssl")
        connection = AmqpClient::Channel::CreateSecure(std::string(), url.host().toStdString(), std::string(), std::string(), 5671, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    else
        connection = AmqpClient::Channel::Create(url.host().toStdString(), 5672, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    qWarning() << "Opened" << url.scheme() << "connection to" << url.host();
    queue = connection->DeclareQueue(queueName.toStdString(), passive, durable, exclusive, autoDelete);
    connection->BindQueue(queue, topic.getTopic(), routingKey.toStdString());
    consumerTag = connection->BasicConsume(queue);
    workerThread = new ReceivingQueueWorker(connection, queue, consumerTag);
    connect(workerThread, &ReceivingQueueWorker::messageReceived, [&](Envelope envelope){
        emit messageReceived(envelope);
    });
    workerThread->start();
}

void RabbitMQTestClient::ReceivingQueue::notify()
{
    workerThread->notify();
}

RabbitMQTestClient::ReceivingQueue::~ReceivingQueue()
{
    if (workerThread != nullptr)
    {
        workerThread->quit();
        workerThread->deleteLater();
    }
}

