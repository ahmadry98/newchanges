#include "Hoover.h"
#include "Sensors.h"
#include <list>

// Constructor implementation
Hoover::Hoover(int startX, int startY, int maxBattery, int maxSteps, int stationX, int stationY, House* house)
        : currentX(startX), currentY(startY), maxBatterySteps(maxBattery), maxSteps(maxSteps),
          chargingStationX(stationX), chargingStationY(stationY), house(house) {
    battery = maxBattery;
    stepsatdoc = 0;
}

void Hoover::move(Step direction) {
    switch (direction) {
        case Step::North:
            if (!house->isWall(currentX - 1, currentY)) {
                currentX--;
                if (house->getDirtLevel(currentX, currentY) > 0) {
                    house->cleanDirt(currentX, currentY);
                }
            }
            break;
        case Step::East:
            if (!house->isWall( currentX, currentY + 1)) {
                currentY++;
                if (house->getDirtLevel(currentX, currentY) > 0) {
                    house->cleanDirt(currentX, currentY);
                }
            }
            break;
        case Step::South:
            if (!house->isWall(currentX + 1, currentY)) {
                currentX++;
                if (house->getDirtLevel(currentX, currentY) > 0) {
                    house->cleanDirt(currentX, currentY);
                }
            }
            break;
        case Step::West:
            if (!house->isWall(currentX, currentY - 1)) {
                currentY--;
                if (house->getDirtLevel(currentX, currentY) > 0) {
                    house->cleanDirt(currentX, currentY);
                }
            }
            break;
        case Step::Stay:
            // Clean the current position if there is dirt
            if (house->getDirtLevel(currentX, currentY) > 0) {
                house->cleanDirt(currentX, currentY);
            }
            break;
            /*
        case NOMOVE:
            // Handle NOMOVE case if necessary
            break;
            */
    }

    battery--;

    // Check if at docking station
    if (currentX == chargingStationX && currentY == chargingStationY) {
        battery++;
        stepsatdoc++;
        battery = std::min(maxBatterySteps, battery + (stepsatdoc * (maxBatterySteps / 20)));
    } else {
        stepsatdoc = 0;
    }
}

// Getter implementations
int Hoover::getCurrentX() const {
    return currentX;
}

int Hoover::getCurrentY() const {
    return currentY;
}

House Hoover::gethouse() const {
    return *house;
}

int Hoover::getBatteryLevel() const {
    return battery;
}

int Hoover::getMaxBatterySteps() const {
    return maxBatterySteps;
}

int Hoover::getMaxSteps() const {
    return maxSteps;
}

int Hoover::getChargingStationX() const {
    return chargingStationX;
}

int Hoover::getChargingStationY() const {
    return chargingStationY;
}

// Setters (if needed)
void Hoover::setCurrentX(int x) {
    currentX = x;
}

void Hoover::setCurrentY(int y) {
    currentY = y;
}
