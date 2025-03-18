#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <memory>
#include <functional>

enum class SensorType
{
    TEMPERATURE,
    PRESSURE,
    HUMIDITY,
    UNKNOWN
};

struct SensorEvent
{
    std::string sensorId;
    SensorType type;
    double value;
};

class Sensor : public std::enable_shared_from_this<Sensor>
{
public:
    Sensor(std::string sensorId, SensorType type, double threshold);
    virtual ~Sensor() = default;

    void setBroker(const std::function<void(std::shared_ptr<Sensor>)> &brokerFunc);
    void readValue(double value);

    std::string getSensorId() const;
    SensorType getType() const;
    double getThreshold() const;
    virtual double getLastReading() const = 0;

protected:
    virtual bool checkThreshold(double value) = 0;

    std::string sensorId_;
    SensorType type_;
    double threshold_;
    std::function<void(std::shared_ptr<Sensor>)> broker;
};

#endif // SENSOR_H
