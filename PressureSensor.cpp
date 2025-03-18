#include "PressureSensor.h"

PressureSensor::PressureSensor(std::string sensorId, double threshold)
    : Sensor(sensorId, SensorType::PRESSURE, threshold), lastReading_(0) {}

double PressureSensor::getLastReading() const { return lastReading_; }

bool PressureSensor::checkThreshold(double value)
{
    lastReading_ = value;
    return value < threshold_;
}
