#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Pipe
{
    int id;

    static int MaxID;

    std::string name;
    double length = 0;
    float  diameter = 0;
    bool   working = true;

public:
    Pipe();
    explicit Pipe(int id);

    int getId() const;
    void ToggleStatus();

    static int getMaxID();
    static void setMaxID(int value);

    const std::string& getName() const;
    double getLength() const;
    float getDiameter() const;
    bool isWorking() const;

    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
    friend std::istream& operator>>(std::istream& in, Pipe& p);
    friend std::ofstream& operator<<(std::ofstream& out, const Pipe& p);
    friend std::ifstream& operator>>(std::ifstream& in, Pipe& p);
};