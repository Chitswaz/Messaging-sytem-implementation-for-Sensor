#include "UnknownSensor.h"

UnknownSensor::UnknownSensor(std::string sensorId, double threshold)
    : Sensor(sensorId, SensorType::UNKNOWN, threshold), lastReading_(0) {}

double UnknownSensor::getLastReading() const { return lastReading_; }

bool UnknownSensor::checkThreshold(double value)
{
    lastReading_ = value;
    return false;
}
