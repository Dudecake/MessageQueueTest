#include <iostream>
#ifdef ACTIVEMQ
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
#ifdef RABBITMQ
    QUrl brokerUrl(QStringLiteral("amqp://localhost:5672"));
    brokerUrl.setUserName("guest");
    brokerUrl.setPassword("guest");
    RabbitMQTestClient::RabbitMQClient::initClient(brokerUrl);
#elif ACTIVEMQ
    activemq::library::ActiveMQCPP::initializeLibrary();
    QUrl brokerUrl(QStringLiteral("tcp://127.0.0.1:61616?username=admin&password=admin"));
    ActiveMQTestClient::ActiveMQClient::initClient(brokerUrl);
#elif KAFKA
    KafkaTestClient::KafkaClient::initClient(QUrl("tcp://localhost:9092"));
#endif
    std::cout << "Hello, World!" << std::endl;

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