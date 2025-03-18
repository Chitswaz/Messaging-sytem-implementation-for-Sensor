#ifndef BROKER_H
#define BROKER_H

#include "Sensor.h"
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

class Broker
{
public:
    Broker();
    ~Broker();

    void subscribe(const std::function<void(std::shared_ptr<Sensor>)> &subscriber);
    void receiveEvent(std::shared_ptr<Sensor> sensor);
    void start();
    void stop();

private:
    void processQueue();

    std::vector<std::function<void(std::shared_ptr<Sensor>)>> subscribers;
    std::queue<std::shared_ptr<Sensor>> messageQueue;
    std::mutex queueMutex;
    std::condition_variable queueCond;
    std::thread queueProcessor;
    std::atomic<bool> running_;
};

#endif // BROKER_H
