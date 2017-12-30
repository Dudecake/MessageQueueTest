//
// Created by dudecake on 01/01/18.
//

#ifndef MESSAGEQUEUETEST_ENVELOPE_H
#define MESSAGEQUEUETEST_ENVELOPE_H

#include <librdkafka/rdkafkacpp.h>
#include <include/baseenvelope.h>
#include "baseenvelope.h"

namespace KafkaTestClient
{
    class Envelope : public BaseEnvelope
    {
    public:
        Envelope() = default;
        Envelope(MessagePointer message, std::string routingKey = std::string()) : message(message), routingKey(routingKey) {  }

        MessagePointer getMessage() const override
        {
            return message.get();
        }

        std::string getConsumerTag() const override
        {
            return std::string();
        }

        unsigned long getDeliveryTag() const override
        {
            return 0;
        }

        std::string getExchange() const override
        {
            return message->topic_name();
        }

        bool getRedelivered() const override
        {
            return false;
        }

        std::string getRoutingKey() const override
        {
            return routingKey;
        }

        std::string getDeliveryChannel() const override
        {
            return nullptr;
        }

    private:
        std::shared_ptr<RdKafka::Message> message;
        std::string routingKey;
    };
};

Q_DECLARE_METATYPE(KafkaTestClient::Envelope);

#endif //MESSAGEQUEUETEST_ENVELOPE_H
