//
// Created by dudecake on 30/12/17.
//

#ifndef MESSAGEQUEUETEST_PROGRAM_H
#define MESSAGEQUEUETEST_PROGRAM_H

#include <QObject>

#ifdef RABBIT
#include "rabbitmqclient/topic.h"

namespace messageQueueProvider = RabbitMQTestClient;
#elif ACTIVEMQ
#include "activemqclient/topic.h"

namespace messageQueueProvider = ActiveMQTestClient;
#elif KAFKA
#include "kafkaclient/topic.h"

namespace messageQueueProvider = KafkaTestClient;
#endif

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);

private:
    messageQueueProvider::Topic r;
public slots:
    void run();
    void handleMessage(messageQueueProvider::Envelope envelope);
};

#endif //MESSAGEQUEUETEST_PROGRAM_H
