#include "CS.h"
#include "Checks.h"

int CS::MaxID = 0;

CS::CS() : id(++MaxID) {}

int CS::getId() const { return id; }

void CS::ToggleStatus() { working = !working; }

std::ostream& operator << (std::ostream& out, const CS& s)
{
    out << "The Compression station:" << std::endl;
    out << "Id:               " << s.id << std::endl;
    out << "Name:             " << s.name << std::endl;
    out << "Workshops:        " << s.workshops << std::endl;
    out << "Active workshops: " << s.activeWorkshops << std::endl;
    out << "Status:           " << (s.working ? "working" : "broken") << std::endl << std::endl;
    return out;
}

std::istream& operator >> (std::istream& in, CS& s)
{
    std::cout << "\n";
    s.name = inputName(in, "Enter the Compression station Name: ");
    s.workshops = inputInt(in, "Enter the Compression station number of workshops: ", 1, 1000);
    s.activeWorkshops = inputInt(in, "Enter the Compression station number of active workshops: ", 0, s.workshops);
    s.working = inputInt(in, "Select the Compression station status, where 1 - working, 0 - broken: ", 0, 1) == 1;
    std::cout << "\n";
    return in;
}

std::ofstream& operator << (std::ofstream& out, const CS& s)
{
    out << "C\n"
        << s.id << "\n"
        << s.name << "\n"
        << s.workshops << "\n"
        << s.activeWorkshops << "\n"
        << s.working << "\n";
    return out;
}

std::ifstream& operator >> (std::ifstream& in, CS& s)
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
    s.working = (std::stoi(line) != 0);

    return in;
}