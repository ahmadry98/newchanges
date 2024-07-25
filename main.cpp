#include <iostream>
#include "MySimulator.h"
#include "CleaningAlgorithm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return 1;
    }

    std::string houseFilePath = argv[1];
    MySimulator simulator;
    std :: cout <<"we are here"<<std :: endl;
    simulator.readHouseFile(houseFilePath);

    CleaningAlgorithm algo;
    simulator.setAlgorithm(algo);

    simulator.run();

    return 0;
}

// getting command line arguments for the house file


