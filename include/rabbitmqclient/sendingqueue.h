/**
 * @file sendingqueue.h
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 17/05/2017
 * @version 0.0.1
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <QObject>
#include <QtDebug>
#include <QTimer>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "rabbitmqclient.h"
#include "topic.h"
#include "envelope.h"

namespace RabbitMQTestClient
{
/**
 * @brief The SendingQueue class represents a queue for sending messages
 */
    class SendingQueue : public QObject
    {
    Q_OBJECT
    public:
        /**
         * @brief SendingQueue creates a queue on the message broker
         * @param queueName name of the queue, if empty the broker will generate a name
         * @param passive declares queue passive, if the queue doesn't exist and is passive the broker will return an error
         * @param durable indicates whether the queue should survive a broker restart
         * @param exclusive indicates this client is the only client that can use this queue
         * @param autoDelete indicates whether the queue should be deleted after any exchanges bound to it have been unbound
         * @param parent the parent of the queue
         */
        explicit SendingQueue(QString queueName, bool passive = false, bool durable = false,
                              bool exclusive = true, bool autoDelete = true, bool noLocal = true, bool noAck = true,
                              bool exclusiveBind = true, QObject *parent = 0);

    private:
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

    public slots:
        void sendMessage(std::string messageBody);
    };
}

#endif // QUEUE_H
