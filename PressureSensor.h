#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include "Sensor.h"

class PressureSensor : public Sensor
{
public:
    PressureSensor(std::string sensorId, double threshold);
    double getLastReading() const override;

protected:
    bool checkThreshold(double value) override;

private:
    double lastReading_;
};

#endif // PRESSURE_SENSOR_H
