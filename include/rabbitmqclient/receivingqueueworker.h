/**
 * @file receivingqueueworker.h
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 18/05/2017
 * @version 0.0.1
 */

#ifndef RECEIVINGQUEUEWORKER_H
#define RECEIVINGQUEUEWORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "envelope.h"

namespace RabbitMQTestClient
{
/**
 * @brief The ReceivingQueueWorker class is a worker which handles receiving messages for a ReceivingQueue object
 */
    class ReceivingQueueWorker : public QThread
    {
    Q_OBJECT
    public:
        explicit ReceivingQueueWorker(AmqpClient::Channel::ptr_t &connection, std::string &queue,
                                      std::string &consumerTag, QObject *parent = nullptr);

        void notify();

    protected:
        [[noreturn]]
        virtual void run() override;

    private:
        AmqpClient::Channel::ptr_t connection;
        std::string queue;
        std::string consumerTag;
        QMutex mutex;
        QWaitCondition waitCon;
        bool handled;

    signals:
        void messageReceived(RabbitMQTestClient::Envelope envelope);

    public slots:
    };
};

#endif // RECEIVINGQUEUEWORKER_H
