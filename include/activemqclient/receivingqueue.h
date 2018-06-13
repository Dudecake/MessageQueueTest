//
// Created by dudecake on 30/12/17.
//

#ifndef MESSAGEQUEUETEST_RECEIVINGQUEUE_H
#define MESSAGEQUEUETEST_RECEIVINGQUEUE_H

#include <QObject>
#include <cms/Session.h>
#include <cms/MessageListener.h>
#include <cms/MessageConsumer.h>
#include <activemq/transport/DefaultTransportListener.h>
#include "activemqclient/envelope.h"

namespace ActiveMQTestClient
{
    class ReceivingQueue : public QObject, public cms::MessageListener, public activemq::transport::DefaultTransportListener
    {
        Q_OBJECT
    public:
        explicit ReceivingQueue(QString queueName, QObject *parent = nullptr);
        void onMessage(const cms::Message *message) override;

    private:
        cms::Session *session;
        cms::Destination *destination;
        cms::MessageConsumer *consumer;

    signals:
        void messageReceived(ActiveMQTestClient::Envelope envelope);
    };
};


#endif //MESSAGEQUEUETEST_RECEIVINGQUEUE_H
