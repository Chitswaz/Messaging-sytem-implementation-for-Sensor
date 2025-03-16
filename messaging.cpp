#include <string>
#include <memory>
#include <functional>
#include <iostream>
#include <vector>

// Event type enum
enum class SensorType
{
    TEMPERATURE,
    PRESSURE,
    HUMIDITY,
    UNKNOWN
};

// Generalized Event Structure
struct SensorEvent
{
    std::string sensorId;
    SensorType type;
    double value;
};

class Sensor : public std::enable_shared_from_this<Sensor>
{
public:
    Sensor(std::string sensorId, SensorType type, double threshold)
        : sensorId_(sensorId), type_(type), threshold_(threshold) {}

    virtual ~Sensor() = default; // Ensures proper cleanup in derived classes

    void setBroker(const std::function<void(std::shared_ptr<Sensor>)> &brokerFunc)
    {
        broker = brokerFunc;
    }

    void readValue(double value)
    {
        if (checkThreshold(value))
        {
            if (broker)
            {
                broker(shared_from_this()); // Pass the object itself as a shared_ptr
            }
        }
    }

    std::string getSensorId() const { return sensorId_; }
    SensorType getType() const { return type_; }
    double getThreshold() const { return threshold_; }
    virtual double getLastReading() const = 0; // Pure virtual function

protected:
    virtual bool checkThreshold(double value) = 0; // Pure virtual function

    std::string sensorId_;
    SensorType type_;
    double threshold_;
    std::function<void(std::shared_ptr<Sensor>)> broker;
};

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor(std::string sensorId, double threshold)
        : Sensor(sensorId, SensorType::TEMPERATURE, threshold), lastReading_(0) {}

    double getLastReading() const override { return lastReading_; }

protected:
    bool checkThreshold(double value) override
    {
        lastReading_ = value;
        return value > threshold_; // Trigger event if temperature exceeds threshold
    }

private:
    double lastReading_;
};

class PressureSensor : public Sensor {
public:
    PressureSensor(std::string sensorId, double threshold)
        : Sensor(sensorId, SensorType::PRESSURE, threshold), lastReading_(0) {}

    double getLastReading() const override { return lastReading_; }

protected:
    bool checkThreshold(double value) override {
        lastReading_ = value;
        return value < threshold_; // Trigger event if pressure falls below threshold
    }

private:
    double lastReading_;
};

class HumiditySensor : public Sensor {
public:
    HumiditySensor(std::string sensorId, double threshold)
        : Sensor(sensorId, SensorType::HUMIDITY, threshold), lastReading_(0) {}

    double getLastReading() const override { return lastReading_; }

protected:
    bool checkThreshold(double value) override {
        lastReading_ = value;
        return value > threshold_; // Trigger event if humidity exceeds threshold
    }

private:
    double lastReading_;
};

class UnknownSensor : public Sensor {
public:
    UnknownSensor(std::string sensorId, double threshold)
        : Sensor(sensorId, SensorType::UNKNOWN, threshold), lastReading_(0) {}

    double getLastReading() const override { return lastReading_; }

protected:
    bool checkThreshold(double value) override {
        lastReading_ = value;
        return false; // Unknown sensors do not trigger events
    }

private:
    double lastReading_;
};

class Broker {
public:
    void subscribe(const std::function<void(std::shared_ptr<Sensor>)> &subscriber) {
        subscribers.push_back(subscriber);
    }

    void receiveEvent(std::shared_ptr<Sensor> sensor) {
        for (const auto &subscriber : subscribers) {
            subscriber(sensor);
        }
    }

private:
    std::vector<std::function<void(std::shared_ptr<Sensor>)>> subscribers;
};

void subscriberFunction(std::shared_ptr<Sensor> sensor) {
    if (!sensor) return;

    // Try casting to TemperatureSensor
    if (auto tempSensor = std::dynamic_pointer_cast<TemperatureSensor>(sensor)) {
        std::cout << "🔥 ALERT! Temperature sensor " << tempSensor->getSensorId()
                  << " reported high temperature: " << tempSensor->getLastReading() << "°C" << std::endl;
    }
    // Try casting to PressureSensor
    else if (auto pressureSensor = std::dynamic_pointer_cast<PressureSensor>(sensor)) {
        std::cout << "⚠️ WARNING! Pressure sensor " << pressureSensor->getSensorId()
                  << " reported low pressure: " << pressureSensor->getLastReading() << " PSI" << std::endl;
    }
    // Try casting to HumiditySensor
    else if (auto humiditySensor = std::dynamic_pointer_cast<HumiditySensor>(sensor)) {
        std::cout << "💧 NOTICE! Humidity sensor " << humiditySensor->getSensorId()
                  << " reported high humidity: " << humiditySensor->getLastReading() << "%" << std::endl;
    }
    // Handle unknown sensor types
    else {
        std::cout << "❓ UNKNOWN! Sensor " << sensor->getSensorId()
                  << " of unknown type reported value: " << sensor->getLastReading() << std::endl;
    }
}

int main() {
    auto tempSensor = std::make_shared<TemperatureSensor>("temp1", 30.0);
    auto pressureSensor = std::make_shared<PressureSensor>("pressure1", 15.0);
    auto humiditySensor = std::make_shared<HumiditySensor>("humidity1", 80.0);
    auto unknownSensor = std::make_shared<UnknownSensor>("unknown1", 0.0);
    Broker broker;

    // Subscribe the subscriber to the broker
    broker.subscribe(subscriberFunction);

    // Connect sensors to the broker
    tempSensor->setBroker([&broker](std::shared_ptr<Sensor> sensor) {
        broker.receiveEvent(sensor);
    });

    pressureSensor->setBroker([&broker](std::shared_ptr<Sensor> sensor) {
        broker.receiveEvent(sensor);
    });

    humiditySensor->setBroker([&broker](std::shared_ptr<Sensor> sensor) {
        broker.receiveEvent(sensor);
    });

    unknownSensor->setBroker([&broker](std::shared_ptr<Sensor> sensor) {
        broker.receiveEvent(sensor);
    });

    // Simulate sensor readings
    tempSensor->readValue(32.5);   // High temperature alert
    pressureSensor->readValue(12.0);  // Low pressure warning
    humiditySensor->readValue(85.0);  // High humidity notice
    unknownSensor->readValue(99.9);   // Unknown sensor reading

    return 0;
}