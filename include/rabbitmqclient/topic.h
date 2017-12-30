/**
 * @file topic.h
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 17/05/2017
 * @version 0.0.1
 */

#ifndef TOPIC_H
#define TOPIC_H

#include <QObject>
#include <QtDebug>
#include <QUrl>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace RabbitMQTestClient
{
/**
 * @brief The Topic class represent a topic for sending messages or binding to a queue
 */
    class Topic : public QObject
    {
    Q_OBJECT
    public:
        /**
         * @brief Topic creates a topic on the message broker
         * @param topicName name for the topic
         * @param passive declares topic passive, if the topic doesn't exist and is passive the broker will return an error
         * @param durable Indicates if the topic should survive broker restart
         * @param autoDelete indicates whether the topic should be deleted if no queues are bound to it
         * @param parent the parent of the topic
         */
        explicit Topic(QString topicName, bool passive = false, bool durable = false, bool autoDelete = false,
                       QObject *parent = nullptr);

        /**
         * @brief getTopic gets the topic name
         * @return the name of the topic
         */
        std::string getTopic() const;

    private:
        /**
         * @brief connection to the message broker
         */
        AmqpClient::Channel::ptr_t connection;
        /**
         * @brief topic name of the topic
         */
        std::string topic;

    signals:

    public slots:

        /**
         * @brief sendMessage publishes a message to the topic
         * @param messageBody body of the message to be sent
         * @param routingKey routing key to use when publishing
         */
        void sendMessage(std::string messageBody, std::string routingKey = "");
    };
};

#endif // TOPIC_H
