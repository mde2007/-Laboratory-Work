#pragma once
#include <iostream>
#include <string>
#include <type_traits>

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
    redirect_output_wrapper(std::ostream& src);
    ~redirect_output_wrapper();
    void redirect(std::ostream& dest);
};

void clearInput(std::istream& in);
std::string inputName(std::istream& in, const std::string& prompt);
std::string inputFileName(std::istream& in, const std::string& prompt);

template<typename T>
T inputNumber(std::istream& in, const std::string& prompt, T lo, T hi)
{
    T x{};
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
            if constexpr (std::is_same_v<T, char>)
                std::cout << "Error, the character must be from '" << lo
                << "' to '" << hi << "'. Try again.\n";
            else
                std::cout << "Error, the number must be from " << lo
                << " to " << hi << ". Try again.\n";
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

inline int inputInt(std::istream& in, const std::string& prompt, int lo, int hi)
{
    return inputNumber<int>(in, prompt, lo, hi);
}

double inputPositiveDouble(std::istream& in, const std::string& prompt);