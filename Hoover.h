
#ifndef HOOVER_H
#define HOOVER_H
#include "House.h"
#include "enums.h"



class Hoover{
private:
    int currentX;                // Current X coordinate of the hoover
    int currentY;
    int battery;  // Declaration of battery as a private member variable
    int maxBatterySteps;         // Maximum steps the battery can last
    int maxSteps;                // Maximum steps the hoover can take
    int chargingStationX;        // X coordinate of the charging station
    int chargingStationY;       // Y coordinate of the charging station
    int stepsatdoc;//stepsatdoc
    House* house;

public:
    // Constructor
    Hoover(int startX, int startY, int maxBattery, int maxSteps, int stationX, int stationY,House* house);

    // Getters
    int getCurrentX() const;
    int getCurrentY() const;
    int getMaxBatterySteps() const;
    int getMaxSteps() const;
    int getChargingStationX() const;
    int getChargingStationY() const;
    int getBatteryLevel() const;
    // Setters (if needed)
    void setCurrentX(int x);
    void setCurrentY(int y);
    // Sensor implementations

    House gethouse() const ;

    // Other Hoover methods
    void move(Step direction);

};

#endif // HOOVER_H