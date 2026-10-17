#pragma once
#include <iostream>
#include <string>

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
    redirect_output_wrapper(std::ostream& src)
        : stream(src), old_buf(src.rdbuf())
    {
    }

    ~redirect_output_wrapper()
    {
        stream.rdbuf(old_buf);
    }

    void redirect(std::ostream& dest)
    {
        stream.rdbuf(dest.rdbuf());
    }
};

void clearInput(std::istream& in);
std::string inputName(std::istream& in, const std::string& prompt);
double inputPositiveDouble(std::istream& in, const std::string& prompt);
int inputInt(std::istream& in, const std::string& prompt, int lo, int hi);