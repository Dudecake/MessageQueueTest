//
// Created by dudecake on 30/12/17.
//

#ifndef MESSAGEQUEUETEST_PROGRAM_H
#define MESSAGEQUEUETEST_PROGRAM_H

#include <QObject>
#include <QStandardPaths>
#include "minicsv.h"

#ifdef RABBITMQ
#include "rabbitmqclient/receivingqueue.h"
#include "rabbitmqclient/topic.h"
#include "rabbitmqclient/envelope.h"

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
    explicit Program(std::string dataDir, QObject *parent = nullptr);
    virtual ~Program();

private:
    static int64_t messageCount;
    messageQueueProvider::Topic r;
#ifdef RABBITMQ
    messageQueueProvider::ReceivingQueue queue;
#endif
    mini::csv::ostringstream csvMessOut;
    mini::csv::ostringstream csvMessIn;
    std::chrono::time_point<std::chrono::high_resolution_clock> inMessInterval;
    int64_t inMessId;
    std::string dataDir;
    std::string dateString;
public slots:
    void run();
    void handleMessage(messageQueueProvider::Envelope envelope);
};

#endif //MESSAGEQUEUETEST_PROGRAM_H
