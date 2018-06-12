/**
 * @file receivingqueue.h
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 17/05/2017
 * @version 0.0.1
 */

#ifndef SENDINGQUEUE_H
#define SENDINGQUEUE_H

#include <QObject>
#include <QtDebug>
#include <QTimer>
#include <QThread>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "rabbitmqclient/rabbitmqclient.h"
#include "rabbitmqclient/envelope.h"
#include "rabbitmqclient/topic.h"
#include "receivingqueueworker.h"

namespace RabbitMQTestClient
{
/**
 * @brief The ReceivingQueue class represents a queue for receiving messages
 */
    class ReceivingQueue : public QObject
    {
    Q_OBJECT
    public:
        /**
         * @brief ReceivingQueue creates a queue on the message broker, call notify() after handling a message
         * @param queueName name of the queue, if empty the broker will generate a name
         * @param passive declares queue passive, if the queue doesn't exist and is passive the broker will return an error
         * @param durable indicates whether the queue should survive a broker restart
         * @param exclusive indicates this client is the only client that can use this queue
         * @param autoDelete indicates whether the queue should be deleted after any exchanges bound to it have been unbound
         * @param parent the parent of the queue
         */
        explicit ReceivingQueue(QString queueName, bool passive = false, bool durable = false,
                                bool exclusive = true, bool autoDelete = true, bool noLocal = true, bool noAck = true,
                                bool exclusiveBind = true, QObject *parent = 0);

        /**
         * @brief ReceivingQueue ReceivingQueue creates a queue on the message broker and binds it to the given topic, call notify() after handling a message
         * @param topic the topic to bind to
         * @param queueName name of the queue, if empty the broker will generate a name
         * @param routingKey routing key of the messages to be delivered to this queue
         * @param passive declares queue passive, if the queue doesn't exist and is passive the broker will return an error
         * @param durable indicates whether the queue should survive a broker restart
         * @param exclusive indicates this client is the only client that can use this queue
         * @param autoDelete indicates whether the queue should be deleted after any exchanges bound to it have been unbound
         * @param parent the parent of the queue
         */
        explicit ReceivingQueue(Topic &topic, QString queueName, QString routingKey = "", bool passive = false,
                                bool durable = false,
                                bool exclusive = true, bool autoDelete = true, QObject *parent = 0);

        void notify();

        virtual ~ReceivingQueue();

    private:
        bool bound;
        /**
         * @brief connection to the message broker
         */
        AmqpClient::Channel::ptr_t connection;
        /**
         * @brief queue name of the queue
         */
        std::string queue;
        /**
         * @brief consumerTag name of the consumer
         */
        std::string consumerTag;
        /**
         * @brief workerThread worker thread which handles receiving the messages
         */
        ReceivingQueueWorker *workerThread;

    signals:

        /**
         * @brief messageReceived signal emitted when a message is received
         * @param message the received message
         */
        void messageReceived(const Envelope message);

    public slots:
    };
};

#endif // SENDINGQUEUE_H
