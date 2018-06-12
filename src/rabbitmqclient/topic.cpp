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
    connection = RabbitMQClient::createConnection();
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
