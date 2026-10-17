#include "Checks.h"

void clearInput(std::istream& in)
{
    in.clear();
    in.ignore(1000, '\n');
}

std::string inputName(std::istream& in, const std::string& prompt)
{
    std::string name;
    std::cout << prompt;
    std::getline(in >> std::ws, name);
    while (name.empty())
    {
        std::cout << "Error, the name must not be empty. Try again.\n";
        std::cout << prompt;
        std::getline(in >> std::ws, name);
    }
    std::cerr << name << std::endl;
    return name;
}

double inputPositiveDouble(std::istream& in, const std::string& prompt)
{
    double x;
    while (true)
    {
        std::cout << prompt;
        in >> x;
        if (in.fail() || in.peek() != '\n')
        {
            std::cout << "Error, the input doesn't match the type. Try again.\n";
            clearInput(in);
        }
        else if (x <= 0)
        {
            std::cout << "Error, the number must be positive. Try again.\n";
            in.ignore(1000, '\n');
        }
        else
        {
            in.ignore(1000, '\n');
            std::cerr << x << std::endl;
            return x;
        }
    }
}

int inputInt(std::istream& in, const std::string& prompt, int lo, int hi)
{
    int x;
    while (true)
    {
        std::cout << prompt;
        in >> x;
        if (in.fail() || in.peek() != '\n')
        {
            std::cout << "Error, the input doesn't match the type. Try again.\n";
            clearInput(in);
        }
        else if (x < lo || x > hi)
        {
            std::cout << "Error, the number must be from " << lo << " to " << hi << ". Try again.\n";
            in.ignore(1000, '\n');
        }
        else
        {
            in.ignore(1000, '\n');
            std::cerr << x << std::endl;
            return x;
        }
    }
}