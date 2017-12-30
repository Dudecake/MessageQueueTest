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
    connection = AmqpClient::Channel::Create(url.host().toStdString(), 5672, url.userName() != QStringLiteral("") ? url.userName().toStdString() : QStringLiteral("guest").toStdString(), url.password() != QStringLiteral("") ? url.password().toStdString() : QStringLiteral("guest").toStdString());
    queue = connection->DeclareQueue(queueName.toStdString(), passive, durable, exclusive, autoDelete);
    qWarning() << "Declared queue" << QString::fromStdString(queue) << "on channel" << connection.get();
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
    connection = AmqpClient::Channel::Create(url.host().toStdString(), 5672, url.userName() != QStringLiteral("") ? url.userName().toStdString() : QStringLiteral("guest").toStdString(), url.password() != QStringLiteral("") ? url.password().toStdString() : QStringLiteral("guest").toStdString());
    queue = connection->DeclareQueue(queueName.toStdString(), passive, durable, exclusive, autoDelete);
    qWarning() << "Declared queue" << QString::fromStdString(queue) << "on channel" << connection.get();
    qWarning() << "Binding queue on channel" << connection.get();
    connection->BindQueue(queue, topic.getTopic(), routingKey.toStdString());
    qWarning() << "Bound queue" << QString::fromStdString(queue) << "to" << QString::fromStdString(topic.getTopic()) << "on channel" << connection.get();
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

