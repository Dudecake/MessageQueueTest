/**
 * @file topic.cpp
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 17/05/2017
 * @version 0.0.1
 */

#include "rabbitmqclient/topic.h"
#include "rabbitmqclient/rabbitmqclient.h"


RabbitMQTestClient::Topic::Topic(QString topicName, bool passive, bool durable, bool autoDelete, QObject *parent) : QObject(parent)
{
    topic = topicName.toStdString();
    QUrl url = RabbitMQTestClient::RabbitMQClient::getBrokerUrl();
    if (url.scheme() == "ssl")
        connection = AmqpClient::Channel::CreateSecure(std::string(), url.host().toStdString(), std::string(), std::string(), 5671, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    else
        connection = AmqpClient::Channel::Create(url.host().toStdString(), 5672, url.userName() != QString() ? url.userName().toStdString() : QString("guest").toStdString(), url.password() != QString() ? url.password().toStdString() : QString("guest").toStdString());
    qWarning() << "Opened" << url.scheme() << "connection to" << url.host();
    connection->DeclareExchange(topic, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, passive, durable, autoDelete);
}

std::string RabbitMQTestClient::Topic::getTopic() const
{
    return topic;
}

void RabbitMQTestClient::Topic::sendMessage(std::string messageBody, std::string routingKey)
{
    AmqpClient::BasicMessage::ptr_t message;
    if (!messageBody.empty())
    {
        message = AmqpClient::BasicMessage::Create(messageBody);
        connection->BasicPublish(topic, routingKey, message);
    }
}
