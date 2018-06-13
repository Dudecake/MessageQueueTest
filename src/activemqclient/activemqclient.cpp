//
// Created by dudecake on 28/12/17.
//
#include "activemqclient/activemqclient.h"

ActiveMQTestClient::ActiveMQClient ActiveMQTestClient::ActiveMQClient::ActiveMQClient::client = ActiveMQTestClient::ActiveMQClient();
cms::ConnectionFactory *ActiveMQTestClient::ActiveMQClient::ActiveMQClient::connectionFactory = nullptr;
cms::Connection *ActiveMQTestClient::ActiveMQClient::ActiveMQClient::connection = nullptr;