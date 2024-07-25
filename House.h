#ifndef HOUSE_H
#define HOUSE_H

#include <vector>
#include <fstream>

class House {
private:
    int length;                         // Length of the house
    int width;                          // Width of the house
    int totalDirt;                      // Total amount of dirt in the house
    std::vector<std::vector<int>> layout;   // Matrix representing the house layout

public:
    // Constructor
    House(int l, int w, int dirt, const std::vector<std::vector<int>>& matrix);

    // Function to display the house layout
    void displayHouse() const;
    void displayHouse(std::ofstream& outputFile) const;

    // Getter functions
    int getLength() const;
    int getWidth() const;
    int getTotalDirt() const;
    const std::vector<std::vector<int>>& getLayout() const;
    int getDirtLevel(int x, int y) const {
        return layout[x][y];
    }

    bool isWall(int x, int y) const {
        return layout[x][y] == -1;
    }

    void cleanDirt(int x, int y) {
        if (layout[x][y] > 0) {
            layout[x][y]--;
            totalDirt--;
        }
    }

};

#endif // HOUSE_H