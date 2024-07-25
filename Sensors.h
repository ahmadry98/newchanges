#ifndef SENSORS_H
#define SENSORS_H

#include "Hoover.h"
#include "dirt_sensor.h"
#include "wall_sensor.h"
#include "battery_meter.h"
#include "House.h"
#include "enums.h"

class MyDirtSensor : public DirtSensor {
private:
    Hoover* hoover; // Pointer to the Hoover object

public:
    // Constructor
    MyDirtSensor(Hoover* hoover) : hoover(hoover) {}

    // Implementations
    int getDirtLevel() const  {
        return hoover->gethouse().getLayout()[hoover->getCurrentX()][hoover->getCurrentY()]; // Example implementation; replace with actual logic
    }

    int getAllDirtLevel()  {
        return hoover->gethouse().getTotalDirt(); // Example implementation; replace with actual logic
    }

    int dirtLevel() const override {
        return getDirtLevel(); // Implementation for pure virtual function from DirtSensor
    }
};
class MyWallSensor  :public  WallsSensor {
private:
     Hoover* hoover; // Pointer to the house object

public:
    // Constructor
    MyWallSensor( Hoover *hoover) : hoover(hoover) {}

    bool isWall(Direction direction) const {
        switch (direction) {
            case Direction::North:
                return hoover->gethouse().getLayout()[hoover->getCurrentX() - 1][hoover->getCurrentY()] == -1;
            case Direction::East:
                return hoover->gethouse().getLayout()[hoover->getCurrentX()][hoover->getCurrentY() + 1] == -1;
            case Direction::South:
                return hoover->gethouse().getLayout()[hoover->getCurrentX() + 1][hoover->getCurrentY()] == -1;
            case Direction::West:
                return hoover->gethouse().getLayout()[hoover->getCurrentX()][hoover->getCurrentY() - 1] == -1;
            default:
                return false;
        }
    }
};

class MyBatterySensor : public BatteryMeter {
private:
    Hoover* hoover; // Pointer to the Hoover object

public:
    // Constructor
    MyBatterySensor(Hoover* hoover) : hoover(hoover) {}

    int getBatteryLevel() const {
        return hoover->getBatteryLevel();
    }

    int getMaxBatteryLevel()  {
        return hoover->getMaxBatterySteps();
    }

    std::size_t getBatteryState() const override {
        return getBatteryLevel(); // Implementation for pure virtual function from BatteryMeter
    }
};

#endif // SENSORS_H