/**
 * @file receivingqueueworker.cpp
 * @author Coen Koomen <coen_koomen@hotmail.com>
 * @date 18/05/2017
 * @version 0.0.1
 */

#include "rabbitmqclient/receivingqueueworker.h"

RabbitMQTestClient::ReceivingQueueWorker::ReceivingQueueWorker(AmqpClient::Channel::ptr_t &connection, std::string &queue, std::string &consumerTag, QObject *parent) : QThread(parent)
{
    this->connection = connection;
    this->queue = queue;
    this->consumerTag = consumerTag;
}

void RabbitMQTestClient::ReceivingQueueWorker::notify()
{
    handled = true;
    waitCon.notify_one();
}

void RabbitMQTestClient::ReceivingQueueWorker::run()
{
    AmqpClient::Envelope::ptr_t response;
    while (true)
    {
        connection->BasicConsumeMessage(consumerTag, response);
        emit messageReceived(Envelope(response));
        while(!handled)
        {
            mutex.lock();
            waitCon.wait(&mutex);
            mutex.unlock();
        }
        handled = false;
    }
}
