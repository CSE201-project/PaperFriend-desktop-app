#include "loadHabits.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> load_habits()
{
    std::string habit;
    std::vector<std::string> current_habits;
    std::ifstream myfile;
    myfile.open("habits.txt");
    if (myfile.is_open())
    {
        while (myfile.good())
        {
            std::getline(myfile, habit);
            std::cout << habit << '\n';
            current_habits.push_back(habit);
        }
    }
    else
    {
        std::cout << "Couldn't open habits file\n";
    }
    return current_habits;
}
