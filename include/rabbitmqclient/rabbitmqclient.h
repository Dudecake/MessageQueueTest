/**
 * @file moogclient.h
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 04/05/2017
 * @version 0.0.1
 *
 * @brief Client library for rabbitmq for use with the Moog Platform
 *
 * @section DESCRIPTION
 *
 * This is a client library for use with the Moog platform,  the library makes use of SimpleAmqpClient made by alanxz (https://github.com/alanxz/SimpleAmqpClient)
 * to connect to a rabbitmq message broker to decouple the server from the clients, which eases the development of the applications.
 */

#ifndef MOOGCLIENT_H
#define MOOGCLIENT_H

#include "rabbitmqclient/envelope.h"

#include <QMetaType>
#include <QUrl>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace RabbitMQTestClient
{
    /**
     * @brief The MoogClient class to initialize the client library
     */
    class RabbitMQClient
    {
    public:
        RabbitMQClient() = delete;

        /**
         * @brief initClient initializes the client library
         * @param brokerUrl the url of the broker to connect to
         */
        static void initClient(QUrl &brokerUrl)
        {
            RabbitMQClient::brokerUrl = brokerUrl;
            qRegisterMetaType<Envelope>();
            //RabbitMQClient::connection = AmqpClient::Channel::Create(brokerUrl.host().toStdString(), 5672, brokerUrl.userName() != QStringLiteral("") ? brokerUrl.userName().toStdString() : QStringLiteral("guest").toStdString(), brokerUrl.password() != QStringLiteral("") ? brokerUrl.password().toStdString() : QStringLiteral("guest").toStdString());
        }

        static inline QUrl getBrokerUrl()
        { return brokerUrl; }
        //static inline AmqpClient::Channel::ptr_t &getBrokerConnection() { return connection; }

    private:
        static QUrl brokerUrl;
        //static AmqpClient::Channel::ptr_t connection;
    };
};
#endif // MOOGCLIENT_H
