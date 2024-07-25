#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include <string>
#include <vector>
#include "Hoover.h"
#include "House.h"
#include "sensors.h"
#include "CleaningAlgorithm.h"

class MySimulator {
public:;

    void readHouseFile(const std::string& houseFilePath);
    void setAlgorithm(CleaningAlgorithm& algo);
    void run();
private:
    std::vector<std::vector<int>> parseHouseLayout(std::ifstream &inputFile, int rows, int cols);
    House* house;
    Hoover* hoover;
    int maxSteps;
    int maxBattery;
    std::string houseFilePath;
};

#endif // MYSIMULATOR_H
