//
// Created by dudecake on 30/12/17.
//

#ifndef MESSAGEQUEUETEST_ENVELOPE_H
#define MESSAGEQUEUETEST_ENVELOPE_H

#include <QObject>
#include <memory>
#include <cms/Message.h>
#include "baseenvelope.h"

namespace ActiveMQTestClient
{
    class Envelope : BaseEnvelope
    {
    public:
        Envelope() = default;
        Envelope(MessagePointer message, std::string routingKey = std::string()) : message(message), routingKey(routingKey) {  }

        virtual MessagePointer getMessage() const override
        {
            return message.get();
        }

        virtual std::string getConsumerTag() const override
        {
            return std::string();
        }

        virtual unsigned long getDeliveryTag() const override
        {
            return 0u;
        }

        virtual std::string getExchange() const override
        {
            return std::string();//message->getCMSDestination()->getCMSProperties().getProperty();
        }

        virtual bool getRedelivered() const override
        {
            return message->getCMSRedelivered();
        }

        virtual std::string getRoutingKey() const override
        {
            return routingKey;
        }

        virtual std::string getDeliveryChannel() const override
        {
            return std::string();
        }

        virtual ~Envelope() = default;

    private:
        std::shared_ptr<const cms::Message> message;
        std::string routingKey;
    };
};

Q_DECLARE_METATYPE(ActiveMQTestClient::Envelope);

#endif //MESSAGEQUEUETEST_ENVELOPE_H
