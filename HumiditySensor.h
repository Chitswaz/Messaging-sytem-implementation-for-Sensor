#ifndef HUMIDITY_SENSOR_H
#define HUMIDITY_SENSOR_H

#include "Sensor.h"

class HumiditySensor : public Sensor
{
public:
    HumiditySensor(std::string sensorId, double threshold);
    double getLastReading() const override;

protected:
    bool checkThreshold(double value) override;

private:
    double lastReading_;
};

#endif // HUMIDITY_SENSOR_H
