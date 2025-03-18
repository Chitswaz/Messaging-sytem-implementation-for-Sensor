#include "Sensor.h"

Sensor::Sensor(std::string sensorId, SensorType type, double threshold)
    : sensorId_(sensorId), type_(type), threshold_(threshold) {}

void Sensor::setBroker(const std::function<void(std::shared_ptr<Sensor>)> &brokerFunc)
{
    broker = brokerFunc;
}

void Sensor::readValue(double value)
{
    if (checkThreshold(value) && broker)
    {
        broker(shared_from_this());
    }
}

std::string Sensor::getSensorId() const { return sensorId_; }
SensorType Sensor::getType() const { return type_; }
double Sensor::getThreshold() const { return threshold_; }
