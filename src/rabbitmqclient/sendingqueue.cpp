/**
 * @file sendingqueue.cpp
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 17/05/2017
 * @version 0.0.1
 */

#include "rabbitmqclient/sendingqueue.h"

RabbitMQTestClient::SendingQueue::SendingQueue(QString queueName, bool passive, bool durable, bool exclusive, bool autoDelete, bool noLocal, bool noAck, bool exclusiveBind, QObject *parent) : QObject(parent)
{
    connection = RabbitMQClient::createConnection();
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
