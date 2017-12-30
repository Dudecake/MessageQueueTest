//
// Created by dudecake on 02/01/18.
//

#ifndef MESSAGEQUEUETEST_RECEIVINGQUEUEWORKER_H
#define MESSAGEQUEUETEST_RECEIVINGQUEUEWORKER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <librdkafka/rdkafkacpp.h>

#include "kafkaclient/envelope.h"

namespace KafkaTestClient
{
    class ReceivingQueueWorker : public QThread
    {
        Q_OBJECT
    public:
        explicit ReceivingQueueWorker(std::shared_ptr<RdKafka::Consumer> &consumer, std::shared_ptr<RdKafka::Queue> &queue, QObject *parent = nullptr);
        void notify();

    protected:
        [[noreturn]]
        virtual void run() override;

    private:
        std::shared_ptr<RdKafka::Consumer> consumer;
        std::shared_ptr<RdKafka::Queue> queue;
        QMutex mutex;
        QWaitCondition waitCon;
        bool handled;

    signals:
        void messageReceived(KafkaTestClient::Envelope envelope);
    };
};

#endif //MESSAGEQUEUETEST_RECEIVINGQUEUEWORKER_H
