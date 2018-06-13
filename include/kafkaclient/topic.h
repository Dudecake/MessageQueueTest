//
// Created by dudecake on 01/01/18.
//

#ifndef MESSAGEQUEUETEST_TOPIC_H
#define MESSAGEQUEUETEST_TOPIC_H

#include <QObject>
#include <memory>
#include <librdkafka/rdkafkacpp.h>

#include "kafkaclient/envelope.h"
#include "kafkaclient/topicworker.h"

namespace KafkaTestClient
{
    class Topic : public QObject
    {
        Q_OBJECT
    public:
        static void restoreGroupCounter(int groupCounter) { if (Topic::groupCounter == 0) Topic::groupCounter = groupCounter; }
        static int getGroupCounter() { return groupCounter; }
        explicit Topic(std::string topicName, QObject *parent = nullptr);
        explicit Topic(std::string topicName, std::string routingKey = std::string(), QObject *parent = nullptr);
        void inline notify() { if (worker) worker->notify(); }
        virtual ~Topic();

    private:
        static int groupCounter;
        std::shared_ptr<RdKafka::KafkaConsumer> consumer;
        std::shared_ptr<RdKafka::Producer> producer;
        std::shared_ptr<RdKafka::Topic> internalTopic;
        KafkaTestClient::TopicWorker *worker;
        int32_t partition;

    public slots:
        void sendMessage(std::string messageBody, std::string routingKey = "");

    signals:
        void messageReceived(KafkaTestClient::Envelope envelope);
    };
};

#endif //MESSAGEQUEUETEST_TOPIC_H
