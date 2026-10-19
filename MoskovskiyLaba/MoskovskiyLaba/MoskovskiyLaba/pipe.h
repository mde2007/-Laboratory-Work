#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Pipe
{
    int id;
public:
    static int MaxID;

    std::string name;
    double length = 0;
    float  diameter = 0;
    bool   working = true;

    Pipe();

    int getId() const;
    void ToggleStatus();

    friend std::ostream& operator << (std::ostream& out, const Pipe& p);
    friend std::istream& operator >> (std::istream& in, Pipe& p);
    friend std::ofstream& operator << (std::ofstream& out, const Pipe& p);
    friend std::ifstream& operator >> (std::ifstream& in, Pipe& p);
};