#include "MySimulator.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "CleaningAlgorithm.h"


std::vector<std::vector<int>> MySimulator::parseHouseLayout(std::ifstream &inputFile, int rows, int cols) {
    // Create a layout matrix with extra padding for the casing (-1) and initialize the interior to 0
    std::vector<std::vector<int>> layoutWithCasing(rows + 2, std::vector<int>(cols + 2, -1));
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            layoutWithCasing[i][j] = 0;
        }
    }

    std::string line;
    int rowNum = 0;
    bool hasDockingStation = false;

    // Read each line from the input file
    while (std::getline(inputFile, line)) {
        if (rowNum >= rows) {
            break;
        }
        for (int colNum = 0; colNum < std::min(static_cast<int>(line.length()), cols); ++colNum) {
            char ch = line[colNum];
            if (ch == 'W') {
                layoutWithCasing[rowNum + 1][colNum + 1] = -1;  // Wall
            } else if (ch == 'D') {
                if (hasDockingStation) {
                    throw std::runtime_error("Invalid input: More than one docking station found.");
                }
                layoutWithCasing[rowNum + 1][colNum + 1] = -2;  // Docking station
                hasDockingStation = true;
            } else if (ch == ' ') {
                layoutWithCasing[rowNum + 1][colNum + 1] = 0;  // Empty space
            } else if (isdigit(ch)) {
                layoutWithCasing[rowNum + 1][colNum + 1] = ch - '0';  // Dirt level
            }
        }
        rowNum++;
    }
    if (!hasDockingStation) {
        throw std::runtime_error("Invalid input: No docking station found.");
    }

    return layoutWithCasing;
}
void MySimulator::readHouseFile(const std::string& houseFilePath) {
    std::ifstream inputFile(houseFilePath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Error opening file: " + houseFilePath);
    }

    std::string houseDescription, maxStepsLine, maxBatteryLine, rowsLine, colsLine;
    std::getline(inputFile, houseDescription);
    std::getline(inputFile, maxStepsLine);
    std::getline(inputFile, maxBatteryLine);
    std::getline(inputFile, rowsLine);
    std::getline(inputFile, colsLine);

    maxSteps = std::stoi(maxStepsLine.substr(maxStepsLine.find('=') + 1));
    maxBattery = std::stoi(maxBatteryLine.substr(maxBatteryLine.find('=') + 1));
    int rows = std::stoi(rowsLine.substr(rowsLine.find('=') + 1));
    int cols = std::stoi(colsLine.substr(colsLine.find('=') + 1));

    std::vector<std::vector<int>> layout = parseHouseLayout(inputFile, rows, cols);

    inputFile.close();

    int startX = 0, startY = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (layout[i][j] == -2) {
                startX = i;
                startY = j;
                break;
            }
        }
    }

    int totalDirt = 0;
    for (const auto &row : layout) {
        for (int cell : row) {
            if (cell != -1 && cell != -2) {
                totalDirt += cell;
            }
        }
    }

    house = new House(rows, cols, totalDirt, layout);
    hoover = new Hoover(startX, startY, maxBattery, maxSteps, startX, startY, house);
    this->houseFilePath=houseFilePath;
}



void MySimulator::run() {
    int totald = house->getTotalDirt();
    std::vector<Step> allmoves(maxSteps);
    CleaningAlgorithm algorithm(maxSteps);
    int i = 0;

    while (hoover->getBatteryLevel() > 0 && house->getTotalDirt() > 0 && i < hoover->getMaxSteps()) {
        Step move = algorithm.nextMove(*new MyDirtSensor(hoover), *new MyWallSensor(hoover), *new MyBatterySensor(hoover));
        allmoves[i] = move;
        hoover->move(move);
        i++;
    }

    while (house->getTotalDirt() == 0) {
        if (hoover->getCurrentX() == hoover->getChargingStationX() &&
            hoover->getCurrentY() == hoover->getChargingStationY()) {
            break;
        } else {
            Step move = algorithm.nextMove(*new MyDirtSensor(hoover), *new MyWallSensor(hoover), *new MyBatterySensor(hoover));
            allmoves[i] = move;
            hoover->move(move);
            i++;
        }
    }

    algorithm.writeHouseScanToFile("house_scan.txt");
    std::string baseFileName = std::filesystem::path(this->houseFilePath).stem().string();
    std::string outputFileName = "output_" + baseFileName + ".txt";
    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFileName << std::endl;
        return;
    }

    outputFile << "Total dirt in the beginning: " << totald << std::endl;
    outputFile << "Total dirt left: " << house->getTotalDirt() << std::endl;
    outputFile << "Hoover battery level: " << hoover->getBatteryLevel() << std::endl;
    outputFile << "Hoover position: (" << hoover->getCurrentX() << ", " << hoover->getCurrentY() << ")" << std::endl;

    if (hoover->getBatteryLevel() == 0 && house->getTotalDirt() > 0) {
        outputFile << "The hoover has run out of battery and failed to clean the house." << std::endl;
    } else if (house->getTotalDirt() == 0 && hoover->getCurrentX() == hoover->getChargingStationX() &&
               hoover->getCurrentY() == hoover->getChargingStationY()) {
        outputFile << "The hoover has successfully cleaned the house and returned to the docking station." << std::endl;
    } else if (house->getTotalDirt() > 0 && i == hoover->getMaxSteps()) {
        outputFile << "The hoover hasn't successfully cleaned the house because it ran out of moves." << std::endl;
    } else {
        outputFile << "The hoover has cleaned the house but is not at the docking station." << std::endl;
    }

    outputFile << "The hoover took the path:" << std::endl;
    for (int j = 0; j < i; j++) {
        switch (allmoves[j]) {
            case Step::North:
                outputFile << j + 1 << ": NORTH";
                break;
            case Step::East:
                outputFile << j + 1 << ": EAST";
                break;
            case Step::West:
                outputFile << j + 1 << ": WEST";
                break;
            case Step::South:
                outputFile << j + 1 << ": SOUTH";
                break;

            case Step::Stay:
                outputFile << j + 1 << ": STAY";
                break;

        }
        if (j < i - 1) {
            outputFile << ", ";
        } else {
            outputFile << std::endl;
        }
    }

    outputFile << "House layout:" << std::endl;
    house->displayHouse(outputFile);

    outputFile.close();




}

//MySimulator::MySimulator() {
  //  this->houseFilePath= nullptr;
//}

void MySimulator::setAlgorithm(CleaningAlgorithm& algo) {
    algo.setMaxSteps(maxSteps);
    MyWallSensor wallSensor(hoover);
    MyDirtSensor dirtSensor(hoover);
    MyBatterySensor batterySensor(hoover);
    algo.setWallsSensor(wallSensor);
    algo.setDirtSensor(dirtSensor);
    algo.setBatteryMeter(batterySensor);
}
