#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(std::string sensorId, double threshold)
    : Sensor(sensorId, SensorType::HUMIDITY, threshold), lastReading_(0) {}

double HumiditySensor::getLastReading() const { return lastReading_; }

bool HumiditySensor::checkThreshold(double value)
{
    lastReading_ = value;
    return value > threshold_;
}
