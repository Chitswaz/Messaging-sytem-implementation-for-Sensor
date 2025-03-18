#include "Broker.h"
#include <iostream>

Broker::Broker() : running_(true) {}

Broker::~Broker() { stop(); }

void Broker::subscribe(const std::function<void(std::shared_ptr<Sensor>)> &subscriber)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    subscribers.push_back(subscriber);
}

void Broker::receiveEvent(std::shared_ptr<Sensor> sensor)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    messageQueue.push(sensor);
    queueCond.notify_one();
}

void Broker::processQueue()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCond.wait(lock, [this] { return !messageQueue.empty() || !running_; });

        if (!running_ && messageQueue.empty())
            break;

        while (!messageQueue.empty())
        {
            auto sensor = messageQueue.front();
            messageQueue.pop();
            lock.unlock();

            for (const auto &subscriber : subscribers)
                subscriber(sensor);

            lock.lock();
        }
    }
}

void Broker::start() { queueProcessor = std::thread(&Broker::processQueue, this); }

void Broker::stop()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        running_ = false;
        queueCond.notify_all();
    }
    if (queueProcessor.joinable())
        queueProcessor.join();
}
