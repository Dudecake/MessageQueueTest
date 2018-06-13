//
// Created by dudecake on 30/12/17.
//
#include "activemqclient/receivingqueue.h"
#include "activemqclient/activemqclient.h"

ActiveMQTestClient::ReceivingQueue::ReceivingQueue(QString queueName, QObject *parent) : QObject(parent)
{
    activemq::core::ActiveMQConnection* amqConnection = dynamic_cast<activemq::core::ActiveMQConnection*>(ActiveMQTestClient::ActiveMQClient::getConnection());
    if( amqConnection != nullptr ) {
        amqConnection->addTransportListener( this );
    }
    session = std::unique_ptr<cms::Session>(ActiveMQTestClient::ActiveMQClient::getConnection()->createSession(cms::Session::INDIVIDUAL_ACKNOWLEDGE));
    destination = std::unique_ptr<cms::Destination>(session->createQueue(queueName.append("?consumer.prefetchSize=1").toStdString()));
    consumer = std::unique_ptr<cms::MessageConsumer>(session->createConsumer(destination.get()));
    consumer->setMessageListener(this);
}

void ActiveMQTestClient::ReceivingQueue::onMessage(const cms::Message *message)
{
    emit messageReceived(Envelope(message->clone()));
}