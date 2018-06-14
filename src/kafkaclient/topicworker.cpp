//
// Created by dudecake on 02/01/18.
//
#include "kafkaclient/topicworker.h"
#include <QtDebug>

KafkaTestClient::TopicWorker::TopicWorker(std::shared_ptr<RdKafka::KafkaConsumer> consumer,
                                          std::shared_ptr<RdKafka::Topic> topic, int32_t partition, QObject *parent)
        : QThread(parent), consumer(consumer), partition(partition), topic(topic)
{

}

void KafkaTestClient::TopicWorker::notify()
{
    handled = true;
    waitCon.notify_one();
}

void KafkaTestClient::TopicWorker::run()
{
    qDebug() << "Started listening for messages";
    while (true)
    {
        RdKafka::Message *message = consumer->consume(50);
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
        if (this->isInterruptionRequested())
        {
            break;
        }
    }
}
