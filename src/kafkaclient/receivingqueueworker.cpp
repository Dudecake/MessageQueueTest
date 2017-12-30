//
// Created by dudecake on 02/01/18.
//
#include "kafkaclient/receivingqueueworker.h"

KafkaTestClient::ReceivingQueueWorker::ReceivingQueueWorker(std::shared_ptr<RdKafka::Consumer> &consumer,
                                                            std::shared_ptr<RdKafka::Queue> &queue, QObject *parent)
        : QThread(parent), consumer(consumer), queue(queue)
{

}

void KafkaTestClient::ReceivingQueueWorker::notify()
{
    handled = true;
    waitCon.notify_one();
}

void KafkaTestClient::ReceivingQueueWorker::run()
{
    while (true)
    {
        RdKafka::Message *message = consumer->consume(queue.get(), -1);
        if (message->err() == RdKafka::ERR_NO_ERROR)
        {
            emit messageReceived(Envelope(message));
            while (!handled)
            {
                mutex.lock();
                waitCon.wait(&mutex);
                mutex.unlock();
            }
            handled = false;
        }
        else
        {
            delete message;
        }
    }
}
