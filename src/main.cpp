#include <iostream>
#include <QCommandLineParser>
#include <QUrlQuery>
#ifdef ACTIVEMQ
#include <QTextStream>
#include <activemq/library/ActiveMQCPP.h>
#include "activemqclient/activemqclient.h"
#elif RABBITMQ
#include "rabbitmqclient/rabbitmqclient.h"
#elif KAFKA
#include <librdkafka/rdkafkacpp.h>
#include <kafkaclient/kafkaclient.h>
#endif

#include <QCoreApplication>
#include <QTimer>

#include "program.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationDomain("dudecake.github.io");
    QCoreApplication::setOrganizationName("Independent");
    QCoreApplication::setApplicationName("MessageQueueTest");
    QCoreApplication::setApplicationVersion("1.0.0");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
                              {"H", "host", QCoreApplication::translate("main", "Host to connect to", "host")},
                              {"P", "port", QCoreApplication::translate("main", "Port to connect to", "port")},
                              {"u", "user", QCoreApplication::translate("main", "User to use", "user")},
                              {"p", "pass", QCoreApplication::translate("main", "Password to use", "pass")},
                              {{"s", "secure"}, QCoreApplication::translate("main", "Use SSL")}
    });
    parser.process(a);
    QString host = "127.0.0.1";
    QString scheme = "tcp";
    if (parser.isSet("s"))
        scheme = "ssl";
    if (parser.isSet("H"))
        host = parser.value("H");
    QUrl brokerUrl;
    brokerUrl.setScheme(scheme);
#ifdef RABBITMQ
    int port = 5672;
    if (parser.isSet("s"))
        port = 6571;
    if (parser.isSet("p"))
        port = parser.value("p").toInt();
    QString user = "guest";
    if (parser.isSet("u"))
        user = parser.value("u");
    QString pass = "guest";
    if (parser.isSet("p"))
        pass = parser.value("p");
    brokerUrl.setHost(host);
    brokerUrl.setPort(port);
    brokerUrl.setUserName(user);
    brokerUrl.setPassword(pass);
    RabbitMQTestClient::RabbitMQClient::initClient(brokerUrl);
#elif ACTIVEMQ
    int port = 61616;
    if (parser.isSet("port"))
        port = parser.value("port").toInt();
    QString user = "admin";
    if (parser.isSet("user"))
        user = parser.value("user");
    QString pass = "admin";
    if (parser.isSet("pass"))
        pass = parser.value("pass");
    activemq::library::ActiveMQCPP::initializeLibrary();
    brokerUrl.setHost(host);
    brokerUrl.setPort(port);
    QString urlString;
    QTextStream ts(&urlString);
    ts << "username=" << user << "&password=" << pass;
    brokerUrl.setScheme(urlString);
    ActiveMQTestClient::ActiveMQClient::initClient(brokerUrl);
#elif KAFKA
    int port = 9092;
    if (parser.isSet("port"))
        port = parser.value("port").toInt();
    QString user = "guest";
    if (parser.isSet("user"))
        user = parser.value("user");
    QString pass = "guest";
    if (parser.isSet("pass"))
        pass = parser.value("pass");
    brokerUrl.setHost(host);
    brokerUrl.setPort(port);
    KafkaTestClient::KafkaClient::initClient(brokerUrl);
#endif

    Program p;
    QTimer::singleShot(500, &p, &Program::run);
    int res = a.exec();

#ifdef ACTIVEMQ
    activemq::library::ActiveMQCPP::shutdownLibrary();
#elif KAFKA
    RdKafka::wait_destroyed(5000);
#endif
    return res;
}