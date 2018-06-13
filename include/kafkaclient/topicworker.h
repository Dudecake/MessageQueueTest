//
// Created by dudecake on 02/01/18.
//

#ifndef MESSAGEQUEUETEST_TOPICWORKER_H
#define MESSAGEQUEUETEST_TOPICWORKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <librdkafka/rdkafkacpp.h>

#include "kafkaclient/envelope.h"

namespace KafkaTestClient
{
    class TopicWorker : public QThread
    {
    Q_OBJECT
    public:
        explicit TopicWorker(std::shared_ptr<RdKafka::KafkaConsumer> consumer, std::shared_ptr<RdKafka::Topic> topic, int32_t partition = 0, QObject *parent = nullptr);
        void notify();
        ~TopicWorker() = default;

    protected:
        [[noreturn]]
        virtual void run() override;

    private:
        std::shared_ptr<RdKafka::KafkaConsumer> consumer;
        std::shared_ptr<RdKafka::Topic> topic;
        int32_t partition;
        QMutex mutex;
        QWaitCondition waitCon;
        bool handled;

    signals:
        void messageReceived(KafkaTestClient::Envelope envelope);
    };
};


#endif //MESSAGEQUEUETEST_TOPICWORKER_H
