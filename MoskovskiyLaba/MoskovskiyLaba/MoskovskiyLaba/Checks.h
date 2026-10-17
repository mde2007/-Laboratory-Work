#pragma once
#include <iostream>
#include <string>

void clearInput(std::istream& in);
std::string inputName(std::istream& in, const std::string& prompt);
double inputPositiveDouble(std::istream& in, const std::string& prompt);
int inputInt(std::istream& in, const std::string& prompt, int lo, int hi);