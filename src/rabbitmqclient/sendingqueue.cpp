/**
 * @file sendingqueue.cpp
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 17/05/2017
 * @version 0.0.1
 */

#include "rabbitmqclient/sendingqueue.h"

RabbitMQTestClient::SendingQueue::SendingQueue(QString queueName, bool passive, bool durable, bool exclusive, bool autoDelete, bool noLocal, bool noAck, bool exclusiveBind, QObject *parent) : QObject(parent)
{
    QUrl url = RabbitMQTestClient::RabbitMQClient::getBrokerUrl();
    if (url.scheme() == "ssl")
        connection = AmqpClient::Channel::CreateSecure(std::string(), url.host().toStdString(), std::string(), std::string(), 5671, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    else
        connection = AmqpClient::Channel::Create(url.host().toStdString(), 5672, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    qWarning() << "Opened" << url.scheme() << "connection to" << url.host();
    queue = connection->DeclareQueue(queueName.toStdString(), passive, durable, exclusive, autoDelete);
    consumerTag = connection->BasicConsume(queue, "", noLocal, noAck, exclusiveBind);
}

void RabbitMQTestClient::SendingQueue::sendMessage(std::string messageBody)
{
    AmqpClient::BasicMessage::ptr_t message;
    if (!messageBody.empty())
    {
        message = AmqpClient::BasicMessage::Create(messageBody);
        connection->BasicPublish("", queue, message);
    }
}
