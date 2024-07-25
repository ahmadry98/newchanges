#include "House.h"
#include <iostream>

// Constructor implementation
House::House(int l, int w, int dirt, const std::vector<std::vector<int>>& matrix)
        : length(l), width(w), totalDirt(dirt), layout(matrix) {
}

// Function to display the house layout
void House::displayHouse() const {
    for (int i = 0; i < length+2; ++i) {
        for (int j = 0; j < width+2; ++j) {
            std::cout << layout[i][j] << " ";
        }
        std::cout << "\n";
    }
}
void House::displayHouse(std::ofstream& outputFile) const {
    for (int i = 0; i < length+2; ++i) {
        for (int j = 0; j < width+2; ++j) {
            outputFile << layout[i][j] << " ";
        }
        outputFile << "\n";
    }
}

// Getter implementations
int House::getLength() const {
    return length;
}

int House::getWidth() const {
    return width;
}

int House::getTotalDirt() const {
    return totalDirt;
}

const std::vector<std::vector<int>>& House::getLayout() const {
    return layout;
}
