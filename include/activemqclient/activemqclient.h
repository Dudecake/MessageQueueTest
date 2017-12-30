//
// Created by dudecake on 28/12/17.
//

#ifndef MESSAGEQUEUETEST_ACTIVEMQCLIENT_H
#define MESSAGEQUEUETEST_ACTIVEMQCLIENT_H

#include <QString>
#include <QUrl>
#include <QtDebug>
#include <QCoreApplication>
#include <memory>

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/library/ActiveMQCPP.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageListener.h>
#include <cms/ExceptionListener.h>

namespace ActiveMQTestClient
{
    class ActiveMQClient : public cms::ExceptionListener
    {
    public:
        static void initClient(QUrl &brokerUrl)
        {
//            connectionFactory = std::shared_ptr<cms::ConnectionFactory>(
//                    cms::ConnectionFactory::createCMSConnectionFactory(QString().sprintf("tcp://%s:61616?username=%s&password=&s", brokerUrl.host(), brokerUrl.userName() != QStringLiteral("") ? brokerUrl.userName() : QStringLiteral("admin"), brokerUrl.password() != QStringLiteral("") ? brokerUrl.password() : QStringLiteral("admin")).toStdString()));
            connectionFactory = std::shared_ptr<cms::ConnectionFactory>(
                    cms::ConnectionFactory::createCMSConnectionFactory(brokerUrl.toString(QUrl::UrlFormattingOption::None).toStdString()));
            connection = std::shared_ptr<cms::Connection>(connectionFactory->createConnection());
            connection->start();
            connection->setExceptionListener(&client);
        }

    private:
        void onException(const cms::CMSException &ex) override
        {
            qDebug() << QString::fromStdString(ex.getStackTraceString());
            QCoreApplication::exit(1);
        }

    public:

        static std::shared_ptr<cms::Connection> getConnection() { return connection; }

    private:
        ActiveMQClient() = default;
        static ActiveMQClient client;
        static std::shared_ptr<cms::ConnectionFactory> connectionFactory;
        static std::shared_ptr<cms::Connection> connection;
    };
}

#endif //MESSAGEQUEUETEST_ACTIVEMQCLIENT_H
