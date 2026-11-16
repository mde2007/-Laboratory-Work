#include "GasNet.h"
#include "Checks.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

static double activePercent(const CS& s)
{
    if (s.getWorkshops() == 0) return 0.0;
    return 100.0 * s.getActiveWorkshops() / s.getWorkshops();
}

static int inputPercent()
{
    return inputInt(cin, "Enter the percent of active workshops (0-100): ", 0, 100);
}

static vector<int> chooseTargets(const vector<int>& found, int maxId, const string& idPrompt)
{
    int pick = inputInt(cin, "1 - choose all, 2 - select by id: ", 1, 2);
    if (pick == 1)
        return found;
    return { inputInt(cin, idPrompt, 1, maxId) };
}

static int inputEditOrDelete()
{
    return inputInt(cin, "1 - edit, 2 - delete: ", 1, 2);
}

static Pipe readPipe(ifstream& fin)
{
    string line;
    if (!getline(fin, line)) throw runtime_error("Unexpected end of file");
    int id = stoi(line);
    if (id <= 0) throw runtime_error("Invalid pipe id");

    Pipe p(id);
    fin >> p;
    if (!fin) throw runtime_error("Unexpected end of file");
    return p;
}

static CS readCS(ifstream& fin)
{
    string line;
    if (!getline(fin, line)) throw runtime_error("Unexpected end of file");
    int id = stoi(line);
    if (id <= 0) throw runtime_error("Invalid CS id");

    CS s(id);
    fin >> s;
    if (!fin) throw runtime_error("Unexpected end of file");
    return s;
}


template<typename Obj, typename Pred>
vector<int> GasNetwork::findByFilter(const unordered_map<int, Obj>& items, Pred f)
{
    vector<int> res;
    for (const auto& [id, obj] : items)
        if (f(obj))
            res.push_back(id);
    sort(res.begin(), res.end());
    return res;
}

template<typename Obj>
void GasNetwork::printFound(const unordered_map<int, Obj>& items, const vector<int>& ids)
{
    cout << "\n";
    if (ids.empty())
    {
        cout << "Nothing found.\n\n";
        return;
    }
    cout << "Found: " << ids.size() << "\n\n";
    for (int id : ids)
        cout << items.at(id);
}

template<typename Obj>
vector<int> GasNetwork::searchByName(const unordered_map<int, Obj>& items)
{
    string name = inputName(cin, "Enter the name (or part of it): ");
    return findByFilter(items, [&](const Obj& o) {
        return o.getName().find(name) != string::npos;
        });
}

void GasNetwork::addPipe()
{
    Pipe p;
    cin >> p;

    if (stations.size() >= 2)
    {
        int from = inputInt(cin, "Enter the id of the entry CS (0 - do not connect): ", 0, CS::getMaxID());
        if (from != 0)
        {
            int to = inputInt(cin, "Enter the id of the exit CS: ", 1, CS::getMaxID());
            if (checkEnds(from, to))
                p.connect(from, to);
            else
                cout << "The pipe will be created without connection.\n";
        }
    }
    else
    {
        cout << "Fewer than 2 CS exist, the pipe is created without connection.\n";
    }

    int id = p.getId();
    pipes.emplace(id, p);
    cout << "Pipe added with id = " << id << "\n";
}

void GasNetwork::addCS()
{
    CS s;
    cin >> s;
    int id = s.getId();
    stations.emplace(id, s);
    cout << "CS added with id = " << id << "\n";
}

void GasNetwork::viewAll() const
{
    cout << "\n";
    if (pipes.empty() && stations.empty())
    {
        cout << "You haven't created a single component\n\n";
        return;
    }

    if (!pipes.empty())
    {
        for (const auto& [id, pipe] : pipes)
            cout << pipe;
    }
    else
    {
        cout << "You haven't created a single pipe.\n\n";
    }

    if (!stations.empty())
    {
        for (const auto& [id, s] : stations)
            cout << s;
    }
    else
    {
        cout << "The Compression:\n";
        cout << "You haven't created a single cs.\n\n";
    }
}

void GasNetwork::editPipe(int id, bool working)
{
    auto it = pipes.find(id);
    if (it == pipes.end()) {
        cout << "Pipe with id " << id << " not found.\n";
        return;
    }

    Pipe& p = it->second;

    cout << "Pipe with id=" << id
        << "\nlenght=" << p.getLength()
        << "\ndiameter=" << p.getDiameter()
        << "\nstatus: " << (p.isWorking() ? "working" : "broken") << "\n";

    if (p.isWorking() != working)
        p.ToggleStatus();

    cout << "Status set to: "
        << (p.isWorking() ? "working" : "broken") << "\n\n";
}

void GasNetwork::editCS(int id, int percent)
{
    auto it = stations.find(id);
    if (it == stations.end()) {
        cout << "CS with id " << id << " not found.\n";
        return;
    }

    CS& s = it->second;
    s.setActiveWorkshops(static_cast<int>(lround(s.getWorkshops() * percent / 100.0)));

    cout << "CS with id=" << id
        << "\nname= " << s.getName()
        << "\nworkshops= " << s.getWorkshops()
        << "\nactive workshops= " << s.getActiveWorkshops()
        << " (" << percent << "% requested)"
        << "\nstatus: " << s.getStatus() << "\n\n";
}

void GasNetwork::deletePipe(int id)
{
    if (pipes.erase(id) == 0)
        cout << "Pipe with id " << id << " not found.\n";
    else
        cout << "Pipe with id " << id << " deleted.\n";
}

void GasNetwork::deleteCS(int id)
{
    if (stations.erase(id) == 0)
        cout << "CS with id " << id << " not found.\n";
    else
        cout << "CS with id " << id << " deleted.\n";
}

vector<int> GasNetwork::searchPipesByRepair() const
{
    bool repair = inputInt(cin, "Find pipes: 1 - under repair (broken), 0 - working: ", 0, 1) == 1;
    return findByFilter(pipes, [&](const Pipe& p) {
        return (!p.isWorking()) == repair;
        });
}

vector<int> GasNetwork::searchCSByActivePercent() const
{
    int percent = inputInt(cin, "Enter the percent of working workshops (0-100): ", 0, 100);
    int mode = inputInt(cin, "1 - greater than or equal to, 2 - less than or equal to: ", 1, 2);

    return findByFilter(stations, [&](const CS& s) {
        double p = activePercent(s);
        return (mode == 1) ? (p >= percent) : (p <= percent);
        });
}

vector<int> GasNetwork::findPipes() const
{
    if (pipes.empty())
    {
        cout << "\nYou haven't created a single pipe.\n\n";
        return {};
    }

    cout << "\nSearch pipes by:\n1 - name\n2 - under repair status\n";
    int mode = inputInt(cin, "Your choice: ", 1, 2);

    vector<int> ids = (mode == 1) ? searchByName(pipes) : searchPipesByRepair();
    printFound(pipes, ids);
    return ids;
}

vector<int> GasNetwork::findCS() const
{
    if (stations.empty())
    {
        cout << "\nYou haven't created a single Compression station.\n\n";
        return {};
    }

    cout << "\nSearch Compression stations by:\n1 - name\n2 - percent of working workshops\n";
    int mode = inputInt(cin, "Your choice: ", 1, 2);

    vector<int> ids = (mode == 1) ? searchByName(stations) : searchCSByActivePercent();
    printFound(stations, ids);
    return ids;
}

void GasNetwork::batchPipes()
{
    vector<int> ids = findPipes();
    if (ids.empty()) return;

    vector<int> targets = chooseTargets(ids, Pipe::getMaxID(), "Select pipe ID: ");
    if (inputEditOrDelete() == 1)
    {
        bool working = inputInt(cin, "Select the new status, where 1 - working, 0 - broken: ", 0, 1) == 1;
        for (int id : targets)
            editPipe(id, working);
    }
    else
        for (int id : targets)
            deletePipe(id);
}

void GasNetwork::batchCS()
{
    vector<int> ids = findCS();
    if (ids.empty()) return;

    vector<int> targets = chooseTargets(ids, CS::getMaxID(), "Select CS ID: ");
    if (inputEditOrDelete() == 1)
    {
        int percent = inputPercent();
        for (int id : targets)
            editCS(id, percent);
    }
    else
        for (int id : targets)
            deleteCS(id);
}

void GasNetwork::findObject()
{
    cout << "\n";
    if (pipes.empty() && stations.empty())
    {
        cout << "You haven't created a single component\n\n";
        return;
    }
    cout << "Chose, what you want to find:\n1 - pipe\n2 - CS\n";
    if (inputInt(cin, "Your choice: ", 1, 2) == 1)
        batchPipes();
    else
        batchCS();
}


void GasNetwork::save() const
{
    cout << "\n";
    string filename = inputFileName(cin, "Enter the file name to save to: ");

    ofstream fout(filename);
    if (!fout.is_open())
    {
        cout << "\nError: could not open file " << filename << "\n\n";
        return;
    }

    for (const auto& [id, p] : pipes)
        fout << p;
    for (const auto& [id, s] : stations)
        fout << s;

    cout << "\nWriting to the file " << filename << " is complete.\n\n";
}

void GasNetwork::load()
{
    cout << "\n";
    string filename = inputFileName(cin, "Enter the file name to load from: ");

    ifstream fin(filename);
    if (!fin.is_open())
    {
        cout << "\nError: could not open file " << filename << "\n\n";
        return;
    }

    pipes.clear();
    stations.clear();

    int maxPipeId = 0;
    int maxCsId = 0;
    bool error = false;

    try
    {
        string marker;
        while (getline(fin, marker))
        {
            if (marker == "P")
            {
                Pipe p = readPipe(fin);
                if (!pipes.emplace(p.getId(), p).second)
                    throw runtime_error("Duplicate pipe id");
                maxPipeId = max(maxPipeId, p.getId());
            }
            else if (marker == "C")
            {
                CS s = readCS(fin);
                if (!stations.emplace(s.getId(), s).second)
                    throw runtime_error("Duplicate CS id");
                maxCsId = max(maxCsId, s.getId());
            }
        }
    }
    catch (const exception&)
    {
        error = true;
    }

    if (error)
    {
        pipes.clear();
        stations.clear();
        maxPipeId = 0;
        maxCsId = 0;
        cout << "\nError: something went wrong while reading the file.\n\n";
    }
    else
    {
        cout << "\nReading from the file " << filename << " is complete.\n\n";
    }

    Pipe::setMaxID(maxPipeId);
    CS::setMaxID(maxCsId);
}

bool GasNetwork::checkEnds(int from, int to) const
{
    if (!stations.count(from))
    {
        cout << "CS with id " << from << " not found.\n";
        return false;
    }
    if (!stations.count(to))
    {
        cout << "CS with id " << to << " not found.\n";
        return false;
    }
    if (from == to)
    {
        cout << "Entry and exit CS must be different.\n";
        return false;
    }
    return true;
}

int GasNetwork::findFreePipe(int diameter) const
{
    int best = 0;
    for (const auto& [id, p] : pipes)
        if (!p.isConnected() && p.getDiameter() == diameter)
            if (best == 0 || id < best)
                best = id;
    return best;
}

void GasNetwork::connect()
{
    cout << "\n";
    if (stations.size() < 2)
    {
        cout << "You need at least 2 Compression stations to connect.\n\n";
        return;
    }

    int from = inputInt(cin, "Enter the id of the entry CS: ", 1, CS::getMaxID());
    int to = inputInt(cin, "Enter the id of the exit CS: ", 1, CS::getMaxID());
    if (!checkEnds(from, to))
    {
        cout << "\n";
        return;
    }

    int diameter = inputDiameter(cin, "Enter the pipeline diameter (500, 700, 1000, 1400): ");

    int pipeId = findFreePipe(diameter);
    if (pipeId == 0)
    {
        cout << "No free pipe with diameter " << diameter
            << ". Add a pipe first (menu 1).\n\n";
        return;
    }

    pipes.at(pipeId).connect(from, to);
    cout << "\nPipe " << pipeId << " connects CS " << from << " -> CS " << to << ".\n\n";
}   