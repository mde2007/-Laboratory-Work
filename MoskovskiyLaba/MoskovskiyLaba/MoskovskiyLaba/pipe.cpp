#include "pipe.h"
#include "Checks.h"

int Pipe::MaxID = 0;

Pipe::Pipe() : id(++MaxID) {}
Pipe::Pipe(int id_) : id(id_) {}

int Pipe::getId() const { return id; }

void Pipe::ToggleStatus() { working = !working; }

int Pipe::getMaxID() { return MaxID; }
void Pipe::setMaxID(int value) { MaxID = value; }

const std::string& Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::isWorking() const { return working; }

int Pipe::getStartCS() const { return startCS; }
int Pipe::getEndCS() const { return endCS; }
bool Pipe::isConnected() const { return startCS != 0 && endCS != 0; }
void Pipe::connect(int from, int to) { startCS = from; endCS = to; }
void Pipe::disconnect() { startCS = 0; endCS = 0; }

std::ostream& operator<<(std::ostream& out, const Pipe& p)
{
    out << "The pipeline:" << std::endl;
    out << "Id:       " << p.id << std::endl;
    out << "Name:     " << p.name << std::endl;
    out << "Length:   " << p.length << " km" << std::endl;
    out << "Diameter: " << p.diameter << " mm" << std::endl;
    out << "Status:   " << (p.working ? "working" : "broken") << std::endl << std::endl;
    if (p.isConnected())
        out << "Connected: CS " << p.startCS << " -> CS " << p.endCS << std::endl;
    else
        out << "Connected: no" << std::endl;
    out << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, Pipe& p)
{
    std::cout << "\n";
    p.name = inputName(in, "Enter the pipeline Name: ");
    p.length = inputPositiveDouble(in, "Enter the pipeline length in km: ");
    p.diameter = inputDiameter(in, "Enter the pipeline diameter (500, 700, 1000, 1400): ");
    p.working = inputNumber<int>(in, "Select the pipeline status, where 1 - working, 0 - broken: ", 0, 1) == 1;
    std::cout << "\n";
    return in;
}

std::ofstream& operator<<(std::ofstream& out, const Pipe& p)
{
    out << "P\n"
        << p.id << "\n"
        << p.name << "\n"
        << p.length << "\n"
        << p.diameter << "\n"
        << p.working << "\n"
        << p.startCS << "\n"
        << p.endCS << "\n";
    return out;
}

std::ifstream& operator>>(std::ifstream& in, Pipe& p)
{
    std::string line;

    if (!std::getline(in, p.name)) return in;

    if (!std::getline(in, line)) return in;
    p.length = std::stod(line);

    if (!std::getline(in, line)) return in;
    p.diameter = std::stoi(line);

    if (!std::getline(in, line)) return in;
    p.working = (std::stoi(line) != 0);

    if (!std::getline(in, line)) return in;
    p.startCS = std::stoi(line);

    if (!std::getline(in, line)) return in;
    p.endCS = std::stoi(line);

    return in;
}