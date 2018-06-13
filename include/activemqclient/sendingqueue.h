//
// Created by dudecake on 31/12/17.
//

#ifndef MESSAGEQUEUETEST_SENDINGQUEUE_H
#define MESSAGEQUEUETEST_SENDINGQUEUE_H

#include <QObject>
#include <cms/Session.h>
#include <cms/Destination.h>
#include <cms/MessageProducer.h>

namespace ActiveMQTestClient
{
    class SendingQueue : public QObject
    {
    Q_OBJECT
    public:
        explicit SendingQueue(std::string queueName, QObject *parent);

    private:
        cms::Session *session;
        cms::Destination *destination;
        cms::MessageProducer *producer;
    public slots:
        void sendMessage(std::string messageBody);
    };
};

#endif //MESSAGEQUEUETEST_SENDINGQUEUE_H
