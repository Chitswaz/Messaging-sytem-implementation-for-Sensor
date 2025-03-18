#ifndef UNKNOWN_SENSOR_H
#define UNKNOWN_SENSOR_H

#include "Sensor.h"

class UnknownSensor : public Sensor
{
public:
    UnknownSensor(std::string sensorId, double threshold);
    double getLastReading() const override;

protected:
    bool checkThreshold(double value) override;

private:
    double lastReading_;
};

#endif // UNKNOWN_SENSOR_H
