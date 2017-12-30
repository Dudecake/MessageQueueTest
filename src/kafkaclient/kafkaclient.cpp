//
// Created by dudecake on 02/01/18.
//
#include "kafkaclient/kafkaclient.h"

std::shared_ptr<RdKafka::Conf> KafkaTestClient::KafkaClient::conf = std::shared_ptr<RdKafka::Conf>();
std::shared_ptr<RdKafka::Conf> KafkaTestClient::KafkaClient::topicConf = std::shared_ptr<RdKafka::Conf>();