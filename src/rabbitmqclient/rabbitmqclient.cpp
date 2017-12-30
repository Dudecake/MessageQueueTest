#include "rabbitmqclient/rabbitmqclient.h"

#ifdef Q_OS_LINUX
QUrl RabbitMQTestClient::RabbitMQClient::brokerUrl = QUrl();
//AmqpClient::Channel::ptr_t RabbitMQClient::connection;
#endif
