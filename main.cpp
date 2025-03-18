#include "TemperatureSensor.h"
#include "PressureSensor.h"
#include "HumiditySensor.h"
#include "UnknownSensor.h"
#include "Broker.h"
#include <iostream>
#include <thread>
#include <chrono>

void subscriberFunction(std::shared_ptr<Sensor> sensor)
{
    if (!sensor)
        return;
    std::cout << "Sensor " << sensor->getSensorId() << " reported value: " << sensor->getLastReading() << std::endl;
}

int main()
{
    Broker broker;
    broker.subscribe(subscriberFunction);

    auto tempSensor = std::make_shared<TemperatureSensor>("temp1", 30.0);
    tempSensor->setBroker([&broker](std::shared_ptr<Sensor> sensor) { broker.receiveEvent(sensor); });

    broker.start();
    tempSensor->readValue(32.5);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    broker.stop();

    return 0;
}
