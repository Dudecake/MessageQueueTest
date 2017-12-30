//
// Created by dudecake on 28/12/17.
//
#include "activemqclient/activemqclient.h"

ActiveMQTestClient::ActiveMQClient ActiveMQTestClient::ActiveMQClient::ActiveMQClient::client = ActiveMQTestClient::ActiveMQClient();
std::shared_ptr<cms::ConnectionFactory> ActiveMQTestClient::ActiveMQClient::ActiveMQClient::connectionFactory = std::shared_ptr<cms::ConnectionFactory>();
std::shared_ptr<cms::Connection> ActiveMQTestClient::ActiveMQClient::ActiveMQClient::connection = std::shared_ptr<cms::Connection>();