//
// Created by dudecake on 28/12/17.
//

#ifndef MESSAGEQUEUETEST_TOPIC_H
#define MESSAGEQUEUETEST_TOPIC_H

#include <QObject>
#include <memory>
#include <cms/Session.h>
#include <cms/MessageListener.h>
#include <cms/MessageConsumer.h>
#include <activemq/transport/DefaultTransportListener.h>
#include "activemqclient/envelope.h"

namespace ActiveMQTestClient
{
    class Topic : public QObject, public cms::MessageListener, public activemq::transport::DefaultTransportListener
    {
    Q_OBJECT
    public:
        explicit Topic(QString topicName, QObject *parent = nullptr);
        explicit Topic(QString topicName, QString routingKey, QObject *parent = nullptr);
        void onMessage(const cms::Message *message) override;

    private:
        std::unique_ptr<cms::Session> session;
        std::unique_ptr<cms::Destination> destination;
        std::unique_ptr<cms::MessageConsumer> consumer;
        std::unique_ptr<cms::MessageProducer> producer;
        std::string topic;
        std::string routingKey;

    public slots:
        void sendMessage(std::string messageBody, std::string routingKey = "");

    signals:
        void messageReceived(ActiveMQTestClient::Envelope envelope);
    };
};

#endif //MESSAGEQUEUETEST_TOPIC_H
