/**
 * @file envelope.h
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 09/05/2017
 * @version 0.0.1
 */

#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <QObject>
#include <SimpleAmqpClient/Envelope.h>
#include "baseenvelope.h"

namespace RabbitMQTestClient
{
/**
 * @brief The Envelope class is a wrapper for AmqpClient::Envelope::ptr_t
 */
    class Envelope : public BaseEnvelope
    {
    public:
        /**
         * @brief default constructor required for use in queued signals
         */
        explicit Envelope() = default;

        /**
         * @brief Envelope
         * @param amqpEnvelope
         */
        explicit Envelope(AmqpClient::Envelope::ptr_t amqpEnvelope)
        {
            this->amqpEnvelope = amqpEnvelope;
        }

        /**
         * @brief getMessage gets the payload of the envelope
         * @return the message
         */
        virtual AmqpClient::BasicMessage::ptr_t getMessage() const override
        {
            return amqpEnvelope->Message();
        }

        /**
         * @brief getConsumerTag gets the consumer tag for the consumer that delivered the message
         * @return the consumer that delivered the message
         */
        virtual std::string getConsumerTag() const override
        {
            return amqpEnvelope->ConsumerTag();
        }

        /**
         * @brief getDeliveryTag gets the delivery tag for the message
         * The delivery tag is a unique tag for a given message assigned by the broker
         * This tag is used when Ack'ing a message
         * @return the delivery tag for a message
         */
        virtual unsigned long getDeliveryTag() const override
        {
            return amqpEnvelope->DeliveryTag();
        }

        /**
         * @brief getExchange gets the name of the exchange that the message was published to
         * @return the name of the exchange that the message was published to
         */
        virtual std::string getExchange() const override
        {
            return amqpEnvelope->Exchange();
        }

        /**
         * @brief getRedilivered gets the flag that indicates whether the message was redelivered
         * @return whether the message was redelivered
         */
        virtual bool getRedelivered() const override
        {
            return amqpEnvelope->Redelivered();
        }

        /**
         * @brief getRoutingKey gets the routing key that the message was published with
         * @return routing key that the message was published with
         */
        virtual std::string getRoutingKey() const override
        {
            return amqpEnvelope->RoutingKey();
        }

        /**
         * @brief getDeliveryChannel
         * @return the delivery channe;
         */
        virtual unsigned short getDeliveryChannel() const override
        {
            return amqpEnvelope->DeliveryChannel();
        }

    private:
        /**
         * @brief amqpEnvelope
         */
        AmqpClient::Envelope::ptr_t amqpEnvelope;
    };
};

Q_DECLARE_METATYPE(RabbitMQTestClient::Envelope);

#endif // ENVELOPE_H
