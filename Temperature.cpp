#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(std::string sensorId, double threshold)
    : Sensor(sensorId, SensorType::TEMPERATURE, threshold), lastReading_(0) {}

double TemperatureSensor::getLastReading() const { return lastReading_; }

bool TemperatureSensor::checkThreshold(double value)
{
    lastReading_ = value;
    return value > threshold_;
}
