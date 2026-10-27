#pragma once
#include <iostream>
#include <fstream>
#include <string>

class CS
{
    int id;

    static int MaxID;

    std::string name;
    int workshops = 0;
    int activeWorkshops = 0;
    char status = 'a';

public:
    CS();
    explicit CS(int id);

    int getId() const;

    static int getMaxID();
    static void setMaxID(int value);

    const std::string& getName() const;
    int getWorkshops() const;
    int getActiveWorkshops() const;
    char getStatus() const;

    void setActiveWorkshops(int value);

    friend std::ostream& operator<<(std::ostream& out, const CS& s);
    friend std::istream& operator>>(std::istream& in, CS& s);
    friend std::ofstream& operator<<(std::ofstream& out, const CS& s);
    friend std::ifstream& operator>>(std::ifstream& in, CS& s);
};