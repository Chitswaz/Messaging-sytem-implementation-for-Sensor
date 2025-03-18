#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor(std::string sensorId, double threshold);
    double getLastReading() const override;

protected:
    bool checkThreshold(double value) override;

private:
    double lastReading_;
};

#endif // TEMPERATURE_SENSOR_H
