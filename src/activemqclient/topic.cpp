//
// Created by dudecake on 28/12/17.
//
#include "activemqclient/topic.h"
#include "activemqclient/activemqclient.h"
#include <activemq/commands/ActiveMQBytesMessage.h>

ActiveMQTestClient::Topic::Topic(QString topicName, QObject *parent) : QObject(parent), consumer(nullptr)
{
    this->topic = topicName.toStdString();
    session = ActiveMQTestClient::ActiveMQClient::getConnection()->createSession(cms::Session::INDIVIDUAL_ACKNOWLEDGE);
    destination = session->createTopic(topicName.append("?consumer.prefetchSize=1").toStdString());
    producer = std::unique_ptr<cms::MessageProducer>(session->createProducer(destination));
    producer->setDeliveryMode(cms::DeliveryMode::NON_PERSISTENT);
}

ActiveMQTestClient::Topic::Topic(QString topicName, QString routingKey, QObject *parent) : QObject(parent)
{
    this->topic = topicName.toStdString();
    this->routingKey = routingKey.toStdString();
    activemq::core::ActiveMQConnection* amqConnection = dynamic_cast<activemq::core::ActiveMQConnection*>(ActiveMQTestClient::ActiveMQClient::getConnection());
    if( amqConnection != nullptr ) {
        amqConnection->addTransportListener( this );
    }
    session = ActiveMQTestClient::ActiveMQClient::getConnection()->createSession(cms::Session::INDIVIDUAL_ACKNOWLEDGE);
    destination = session->createTopic(topicName.append("?consumer.prefetchSize=1").toStdString());
    if (!routingKey.isEmpty())
        consumer = session->createConsumer(destination, QString("routingKey='%1'").arg(routingKey).toStdString());
    else
        consumer = session->createConsumer(destination);
    consumer->setMessageListener(this);
}

void ActiveMQTestClient::Topic::sendMessage(std::string messageBody, std::string routingKey)
{
    cms::BytesMessage *message = session->createBytesMessage(reinterpret_cast<const unsigned char*>(messageBody.c_str()), messageBody.length());
    message->setStringProperty("routingKey", routingKey);
    producer->send(message);
    delete message;
}

void ActiveMQTestClient::Topic::onMessage(const cms::Message *message)
{
    try
    {
        const activemq::commands::ActiveMQBytesMessage *mess = static_cast<const activemq::commands::ActiveMQBytesMessage*>(message);
        emit messageReceived(Envelope(mess->cloneDataStructure(), routingKey));
    }
    catch (std::exception &ex)
    {
        qWarning() << "Shit be whack:" << ex.what();
        QCoreApplication::exit(1);
    }
}

