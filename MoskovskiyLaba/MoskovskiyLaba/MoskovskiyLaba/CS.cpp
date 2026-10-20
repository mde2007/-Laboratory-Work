#include "CS.h"
#include "Checks.h"
#include <stdexcept>

int CS::MaxID = 0;

CS::CS() : id(++MaxID) {}

int CS::getId() const { return id; }

std::ostream& operator<<(std::ostream& out, const CS& s)
{
    out << "The Compression station:\n"
        << "Id:               " << s.id << '\n'
        << "Name:             " << s.name << '\n'
        << "Workshops:        " << s.workshops << '\n'
        << "Active workshops: " << s.activeWorkshops << '\n'
        << "Status:           " << s.status << "\n\n";
    return out;
}

std::istream& operator>>(std::istream& in, CS& s)
{
    std::cout << "\n";
    s.name = inputName(in, "Enter the Compression station Name: ");
    s.workshops = inputInt(in, "Enter the Compression station number of workshops: ", 1, 1000);
    s.activeWorkshops = inputInt(in, "Enter the Compression station number of active workshops: ", 0, s.workshops);

    s.status = inputCSStatus(in, "Enter the Compression station status (a, b, c, d): ");
    return in;
}

std::ofstream& operator<<(std::ofstream& out, const CS& s)
{
    out << "C\n" << s.id << '\n' << s.name << '\n'
        << s.workshops << '\n' << s.activeWorkshops << '\n'
        << s.status << '\n';
    return out;
}

std::ifstream& operator>>(std::ifstream& in, CS& s)
{
    std::string line;

    if (!std::getline(in, line)) return in;
    s.id = std::stoi(line);

    if (!std::getline(in, s.name)) return in;

    if (!std::getline(in, line)) return in;
    s.workshops = std::stoi(line);

    if (!std::getline(in, line)) return in;
    s.activeWorkshops = std::stoi(line);

    if (!std::getline(in, line)) return in;
    if (line.size() != 1 || line[0] < 'a' || line[0] > 'd')
        throw std::runtime_error("Invalid CS status in file");
    s.status = line[0];
    return in;
}
