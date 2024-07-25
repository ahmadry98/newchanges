#include "CleaningAlgorithm.h"
#include <algorithm>
#include <list>
CleaningAlgorithm::CleaningAlgorithm(int n):num(n),i(0),moves(new Step[num]) {
    std::srand(std::time(nullptr)); // Seed for random number generation
    for(int ind=0;ind<n;ind++){

        moves[ind]=Step::Stay;
    }

}

CleaningAlgorithm::CleaningAlgorithm() {
}


Step CleaningAlgorithm::nextMove(MyDirtSensor& dirtSensor, MyWallSensor& wallSensor, MyBatterySensor& batterySensor) {
    possibleMoves.clear(); // Clear previous possible moves
    Step move;
    // Check all four directions and add to possible moves if there's no wall
    if (!wallSensor.isWall(Direction::North)) possibleMoves.push_back(Step::North);
    if (!wallSensor.isWall(Direction::East)) possibleMoves.push_back(Step::East);
    if (!wallSensor.isWall(Direction::South)) possibleMoves.push_back(Step::South);
    if (!wallSensor.isWall(Direction::West)) possibleMoves.push_back(Step::West);
    if(dirtSensor.getAllDirtLevel()==0){
        if(i==0) {
            return Step::Stay;
        }
        Step s = moves[i-1];
        //Direction d = moves[i-1];
        moves[i-1]= Step::Stay;
        i = i-1;
        if(s==Step::North)return Step::South;
        if(s==Step::South)return Step::North;
        if(s==Step::West)return Step::East;
        if(s==Step::East)return Step::West;
        return Step::Stay;
    }
    // Always add STAY as a possible move
    possibleMoves.push_back(Step::Stay);

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
        moves[i-1]= Step::Stay;
        i = i-1;
        if(d==Step::North)return Step::South;
        if(d==Step::South)return Step::North;
        if(d==Step::West)return Step::East;
        if(d==Step::East)return Step::West;
        return Step::Stay;
    }
    /*
        // Example: Logic to determine the direction to return to docking station
        // For simplicity, we assume it always tries to head towards (0, 0)
        if (previousDirection == NORTH && !wallSensor.isWall(NORTH)) return NORTH;
        if (previousDirection == EAST && !wallSensor.isWall(EAST)) return EAST;
        if (previousDirection == SOUTH && !wallSensor.isWall(SOUTH)) return SOUTH;
        if (previousDirection == WEST && !wallSensor.isWall(WEST)) return WEST;
        // If previous direction is STAY, return STAY to simulate staying at docking
        return STAY;
    }*/

    // Check if there is dirt at the current position
    bool hasDirt = dirtSensor.getDirtLevel() > 0;

    // If staying, only stay if there is dirt
    if (nextDirection == Step::Stay && !hasDirt) {
        // Choose another random direction that is not STAY
        possibleMoves.erase(std::remove(possibleMoves.begin(), possibleMoves.end(), Step::Stay), possibleMoves.end());
        if (!possibleMoves.empty()) {
            randomIndex = std::rand() % possibleMoves.size();
            move = possibleMoves[randomIndex];
            moves[i]=move;
            i++;
            return move;
        }
    }

    // Otherwise, return the randomly chosen next move
    move = nextDirection;
    moves[i]=move;
    i++;
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
