#include "Checks.h"

redirect_output_wrapper::redirect_output_wrapper(std::ostream& src)
    : stream(src), old_buf(src.rdbuf())
{
}

redirect_output_wrapper::~redirect_output_wrapper()
{
    stream.rdbuf(old_buf);
}

void redirect_output_wrapper::redirect(std::ostream& dest)
{
    stream.rdbuf(dest.rdbuf());
}

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

std::string inputFileName(std::istream& in, const std::string& prompt)
{
    const std::string forbidden = "\\/:*?\"<>|";
    std::string name;
    while (true)
    {
        std::cout << prompt;
        std::getline(in >> std::ws, name);

        if (name.empty())
            std::cout << "Error, the file name must not be empty. Try again.\n";
        else if (name.find_first_of(forbidden) != std::string::npos)
            std::cout << "Error, the file name must not contain any of: " << forbidden << " Try again.\n";
        else
            break;
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
char inputCSStatus(std::istream& in, const std::string& prompt)
{
    while (true)
    {
        std::cout << prompt;
        char value{};
        in >> value;

        if (in.fail())
        {
            std::cout << "Invalid input. Enter a, b, c or d.\n";
            clearInput(in);
            continue;
        }

        if (in.peek() != '\n')
        {
            std::cout << "Enter exactly one character: a, b, c or d.\n";
            clearInput(in);
            continue;
        }

        in.ignore(1000, '\n');

        if (value < 'a' || value > 'd')
        {
            std::cout << "Invalid status. Allowed values are a, b, c, d.\n";
            continue;
        }

        std::cerr << value << std::endl;
        return value;
    }
}
