#pragma once
#include <iostream>
#include <string>

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
double inputPositiveDouble(std::istream& in, const std::string& prompt);
int inputInt(std::istream& in, const std::string& prompt, int lo, int hi);