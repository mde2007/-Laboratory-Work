#include "pipe.h"
#include "Checks.h"

int Pipe::MaxID = 0;

Pipe::Pipe() : id(++MaxID) {}
Pipe::Pipe(int id_) : id(id_) {}

int Pipe::getId() const { return id; }

void Pipe::ToggleStatus() { working = !working; }

std::ostream& operator << (std::ostream& out, const Pipe& p)
{
    out << "The pipeline:" << std::endl;
    out << "Id:       " << p.id << std::endl;
    out << "Name:     " << p.name << std::endl;
    out << "Length:   " << p.length << " km" << std::endl;
    out << "Diameter: " << p.diameter << " mm" << std::endl;
    out << "Status:   " << (p.working ? "working" : "broken") << std::endl << std::endl;
    return out;
}

std::istream& operator >> (std::istream& in, Pipe& p)
{
    std::cout << "\n";
    p.name = inputName(in, "Enter the pipeline Name: ");
    p.length = inputPositiveDouble(in, "Enter the pipeline length in km: ");
    p.diameter = static_cast<float>(inputPositiveDouble(in, "Enter the pipeline diameter in mm: "));
    p.working = inputInt(in, "Select the pipeline status, where 1 - working, 0 - broken: ", 0, 1) == 1;
    std::cout << "\n";
    return in;
}

std::ofstream& operator << (std::ofstream& out, const Pipe& p)
{
    out << "P\n"
        << p.id << "\n"
        << p.name << "\n"
        << p.length << "\n"
        << p.diameter << "\n"
        << p.working << "\n";
    return out;
}

std::ifstream& operator >> (std::ifstream& in, Pipe& p)
{
    std::string line;

    if (!std::getline(in, p.name)) return in;

    if (!std::getline(in, line)) return in;
    p.length = std::stod(line);

    if (!std::getline(in, line)) return in;
    p.diameter = std::stof(line);

    if (!std::getline(in, line)) return in;
    p.working = (std::stoi(line) != 0);

    return in;
}