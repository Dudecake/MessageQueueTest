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
    connection = AmqpClient::Channel::Create(url.host().toStdString(), 5672, url.userName() != QStringLiteral("") ? url.userName().toStdString() : QStringLiteral("guest").toStdString(), url.password() != QStringLiteral("") ? url.password().toStdString() : QStringLiteral("guest").toStdString());
    connection->DeclareExchange(topic, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC, passive, durable, autoDelete);
    qWarning() << "Declared topic" << QString::fromStdString(topic) << "on channel" << connection.get();
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
