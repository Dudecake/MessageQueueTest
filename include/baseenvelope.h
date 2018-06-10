//
// Created by dudecake on 30/12/17.
//

#ifndef MESSAGEQUEUETEST_BASEENVELOPE_H
#define MESSAGEQUEUETEST_BASEENVELOPE_H

#if defined(RABBITMQ)
#include <SimpleAmqpClient/Envelope.h>
typedef AmqpClient::BasicMessage::ptr_t MessagePointer;
#elif defined(ACTIVEMQ)
namespace cms
{
    class Message;
};
typedef const cms::Message* MessagePointer;
#elif KAFKA
namespace RdKafka
{
    class Message;
};
typedef RdKafka::Message* MessagePointer;
#endif

class BaseEnvelope
{
public:
    virtual MessagePointer getMessage() const = 0;

    virtual std::string getConsumerTag() const = 0;

    virtual unsigned long getDeliveryTag() const = 0;

    virtual std::string getExchange() const = 0;

    virtual bool getRedelivered() const = 0;

    virtual std::string getRoutingKey() const = 0;

    virtual std::string getDeliveryChannel() const = 0;

    virtual ~BaseEnvelope() = default;

protected:
    BaseEnvelope() = default;
};


#endif //MESSAGEQUEUETEST_BASEENVELOPE_H
