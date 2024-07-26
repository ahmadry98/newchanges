#include "CleaningAlgorithm.h"
#include <algorithm>
#include <list>
#include <unordered_map>
#include <iostream>
#include <iomanip>

CleaningAlgorithm::CleaningAlgorithm(int n):num(n),i(0),moves(new Step[num]) {
    std::srand(std::time(nullptr)); // Seed for random number generation
    for(int ind=0;ind<n;ind++){

        moves[ind]=Step::Finish;
    }
    HouseScan = std::vector<std::vector<int>>(1000, std::vector<int>(1000, -3));
    HouseScan[500][500] = -2;
    CurrX=500;
    CurrY=500;
    //bfsQueue.push({500, 500});  // Starting from the center (docking station)
}

CleaningAlgorithm::CleaningAlgorithm() {
}


Step CleaningAlgorithm::nextMove(MyDirtSensor& dirtSensor, MyWallSensor& wallSensor, MyBatterySensor& batterySensor) {
    possibleMoves.clear(); // Clear previous possible moves
    Step move;
    int currentX = CurrX;
    int currentY = CurrY;
    //auto [currentX, currentY] = bfsQueue.front();
    if (!wallSensor.isWall(Direction::North) )
    {
        possibleMoves.push_back(Step::North);
    }
    else{
        HouseScan[currentX - 1][currentY] = -1;
    }
    if (!wallSensor.isWall(Direction::East) )
    {
        possibleMoves.push_back(Step::East);
    }
    else{
        HouseScan[currentX ][currentY + 1] = -1;
    }
    if (!wallSensor.isWall(Direction::South) )
    {
        possibleMoves.push_back(Step::South);
    }
    else{
        HouseScan[currentX+ 1][currentY ] = -1;
    }
    if (!wallSensor.isWall(Direction::West) )
    {
        possibleMoves.push_back(Step::West);
    }
    else{
        HouseScan[currentX][currentY-1 ] = -1;
    }
    if(dirtSensor.getAllDirtLevel()==0){
        if(i==0) {
            return Step::Stay;
        }
        Step s = moves[i-1];
        //Direction d = moves[i-1];
        moves[i-1]= Step::Finish;
        i = i-1;
        if(s==Step::North)return Step::South;
        if(s==Step::South)return Step::North;
        if(s==Step::West)return Step::East;
        if(s==Step::East)return Step::West;
        return Step::Stay;
    }
    // Always add STAY as a possible move
    possibleMoves.push_back(Step::Stay);

        for (auto& dir : possibleMoves) {
            int newX=currentX, newY=currentY;
            if(dir == Step::North )newX=currentX-1;
            if(dir == Step::South ) newX=currentX+1;
            if (dir == Step::West ) newY=currentY-1;
           if(dir == Step::East ) newY=currentY+1;
            if (newX >= 0 && newX < 1000 && newY >= 0 && newY < 1000) {
                if (HouseScan[newX][newY] == -3) {
                    HouseScan[newX][newY] = dirtSensor.getDirtLevelwithStep(dir);
                }
            }
        }

        // Update the current position


    // Randomly select one of the possible moves
    int randomIndex = std::rand() % possibleMoves.size();
    Step nextDirection = possibleMoves[randomIndex];

    // Remember the previous direction to decide next moves
    previousDirection = nextDirection;

    // Check if should return to docking station
    if (shouldReturnToDocking(batterySensor)) {
        if(i==0) {
            return Step::Stay;
        };
        Step d = moves[i-1];
        moves[i-1]= Step::Finish;
        i = i-1;
        if(d==Step::North){return Step::South; CurrX=CurrX+1;}
        if(d==Step::South){return Step::North;CurrX=CurrX-1;}
        if(d==Step::West){return Step::East;CurrY=CurrY+1;}
        if(d==Step::East){return Step::West;CurrY=CurrY-1;}
        return Step::Stay;
    }


    bool hasDirt = dirtSensor.getDirtLevel() > 0;

    if (nextDirection == Step::Stay && !hasDirt) {
        // Choose another random direction that is not STAY
        possibleMoves.erase(std::remove(possibleMoves.begin(), possibleMoves.end(), Step::Stay), possibleMoves.end());
        if (!possibleMoves.empty()) {
            randomIndex = std::rand() % possibleMoves.size();
            move = possibleMoves[randomIndex];
            moves[i]=move;
            i++;
            if(move==Step::North){
                CurrX=CurrX-1;
            }
            if(move==Step::South){
                CurrX=CurrX+1;
            }
            if(move==Step::West){
                CurrY=CurrY-1;
            }
            if(move==Step::East){
                CurrY=CurrY+1;
            }
            return move;
        }
    }

    // Otherwise, return the randomly chosen next move
    move = nextDirection;
    moves[i]=move;
    i++;
    if(move==Step::North){
        CurrX=CurrX-1;
    }
    if(move==Step::South){
        CurrX=CurrX+1;
    }
    if(move==Step::West){
        CurrY=CurrY-1;
    }
    if(move==Step::East){
        CurrY=CurrY+1;
    }
    return move;
}

bool CleaningAlgorithm::shouldReturnToDocking(MyBatterySensor& batterySensor) {
    // Example: Naive logic to check if battery level is low and return to docking
    // For simplicity, return true if battery level is below 20%
    int batteryLevel = batterySensor.getBatteryLevel();
    return batteryLevel < 0.66 * batterySensor.getMaxBatteryLevel(); // Assuming getMaxBatteryLevel() returns max steps
}




// Set the maximum number of steps
void CleaningAlgorithm::setMaxSteps(std::size_t maxSteps) {

    this->maxSteps = maxSteps;
}

// Set the wall sensor
void CleaningAlgorithm::setWallsSensor(const WallsSensor& wallsSensor1) {
    this->WallSensor = (MyWallSensor *) &wallsSensor1;
}

// Set the dirt sensor
void CleaningAlgorithm::setDirtSensor(const DirtSensor& dirtSensor1) {
    this->dirtSensor = (MyDirtSensor *) &dirtSensor1;
}

// Set the battery meter
void CleaningAlgorithm::setBatteryMeter(const BatteryMeter& batteryMeter1) {
    this->batteryMeter = (MyBatterySensor *) &batteryMeter1;
}

Step CleaningAlgorithm::nextStep() {

    return Step::Finish;
}

CleaningAlgorithm::~CleaningAlgorithm() {
    // Clean up resources if needed
}


// Write the HouseScan matrix to a file
void CleaningAlgorithm::writeHouseScanToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for (int i = 495; i <= 515; ++i) {  // Print a 11x11 area around the docking station
        for (int j = 495; j <= 515; ++j) {
            file << std::setw(3) << HouseScan[i][j] << " ";
        }
        file << std::endl;
    }
    file.close();
}