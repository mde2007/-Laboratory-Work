#pragma once
#include <iostream>
#include <fstream>
#include <string>

class CS
{
    int id;
public:
    static int MaxID;

    std::string name;
    int  workshops = 0;
    int  activeWorkshops = 0;
    bool working = true;

    CS();

    int getId() const;
    void ToggleStatus();

    friend std::ostream& operator << (std::ostream& out, const CS& s);
    friend std::istream& operator >> (std::istream& in, CS& s);
    friend std::ofstream& operator << (std::ofstream& out, const CS& s);
    friend std::ifstream& operator >> (std::ifstream& in, CS& s);
};