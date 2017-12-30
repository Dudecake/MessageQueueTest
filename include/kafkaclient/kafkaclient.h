//
// Created by dudecake on 01/01/18.
//

#ifndef MESSAGEQUEUETEST_KAFKACLIENT_H
#define MESSAGEQUEUETEST_KAFKACLIENT_H

#include <QtDebug>
#include <QUrl>
#include <memory>
#include <librdkafka/rdkafkacpp.h>

namespace KafkaTestClient
{
    class KafkaClient
    {
    public:
        static void initClient(QUrl url)
        {
            conf = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
            topicConf = std::shared_ptr<RdKafka::Conf>(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));
            std::string errString;
            if (conf->set("metadata.broker.list", url.host().toStdString(), errString) != RdKafka::Conf::CONF_OK)
            {
                qWarning() << "Failed to set brokers:" << QString::fromStdString(errString);
            }
            if (conf->set("default_topic_conf", topicConf.get(), errString) != RdKafka::Conf::CONF_OK)
            {
                qWarning() << "Failed to set default topic config:" << QString::fromStdString(errString);
            }
        }
        static std::shared_ptr<RdKafka::Conf> getConf() { return conf; }
        static std::shared_ptr<RdKafka::Conf> getTopicConf() { return topicConf; }

    private:
        static std::shared_ptr<RdKafka::Conf> conf;
        static std::shared_ptr<RdKafka::Conf> topicConf;
    };
};

#endif //MESSAGEQUEUETEST_KAFKACLIENT_H
