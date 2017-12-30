//
// Created by dudecake on 31/12/17.
//
#include "activemqclient/sendingqueue.h"
#include "activemqclient/activemqclient.h"
#include <activemq/core/ActiveMQConnection.h>

ActiveMQTestClient::SendingQueue::SendingQueue(std::string queueName, QObject *parent) : QObject(parent)
{
    session = std::unique_ptr<cms::Session>(ActiveMQTestClient::ActiveMQClient::getConnection()->createSession(cms::Session::AUTO_ACKNOWLEDGE));
    destination = std::unique_ptr<cms::Destination>(session->createQueue(queueName));
    producer = std::unique_ptr<cms::MessageProducer>(session->createProducer(destination.get()));
    producer->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);
}

void ActiveMQTestClient::SendingQueue::sendMessage(std::string messageBody)
{
    cms::BytesMessage *message = session->createBytesMessage(reinterpret_cast<const unsigned char*>(messageBody.c_str()), messageBody.length());
    producer->send(message);
    delete message;
}