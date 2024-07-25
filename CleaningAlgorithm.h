
#ifndef CLEANINGALGORITHM_H
#define CLEANINGALGORITHM_H

#include "Sensors.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "enums.h"
#include "abstract_algorithm.h"
class CleaningAlgorithm : AbstractAlgorithm{
private:

    int num,i;
    Step* moves;
    std::size_t maxSteps;
    MyBatterySensor* batteryMeter;
    MyDirtSensor*  dirtSensor;
    MyWallSensor* WallSensor;
    std::vector<std::vector<int>> HouseScan;
public:
    CleaningAlgorithm(int n);

    // Method to compute the next move based on sensor inputs
    Step nextMove(MyDirtSensor& dirtSensor, MyWallSensor& wallSensor, MyBatterySensor& batterySensor);

    CleaningAlgorithm();

    std::vector<Step> possibleMoves;
    Step previousDirection;  // To remember the previous move

    bool shouldReturnToDocking(MyBatterySensor& batterySensor);

    void setMaxSteps(std::size_t maxSteps) override;

    void setWallsSensor(const WallsSensor& wallsSensor) override;

    void setDirtSensor(const DirtSensor& dirtSensor) override ;

    void setBatteryMeter(const BatteryMeter& batteryMeter) override;
    Step nextStep() override;
    ~CleaningAlgorithm() override;
};

#endif // CLEANINGALGORITHM_H
