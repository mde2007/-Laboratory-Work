#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <format>
#include <stdexcept>
#include <unordered_map>
#include "Checks.h"
#include "pipe.h"
#include "CS.h"

using namespace std;

void launching()
{
    cout << "App menu:\n";
    cout << "1 - Add pipe\n";
    cout << "2 - Add Compression station\n";
    cout << "3 - View all items\n";
    cout << "4 - Find object\n";
    cout << "5 - Save to file\n";
    cout << "6 - Download from file\n";
    cout << "0 - Exit\n";
}

void Add_pipe(unordered_map<int, Pipe>& pipes) {
    Pipe p;
    cin >> p;
    int id = p.getId();
    pipes.emplace(id, p);
    cout << "Pipe added with id = " << p.getId() << "\n";
}
void Add_CS(unordered_map<int, CS>& cs) {
    CS s;
    cin >> s;
    int id = s.getId();
    cs.emplace(id, s);
    cout << "CS added with id = " << s.getId() << "\n";
}
void viewAll(const unordered_map<int, Pipe>& pipes,
    const unordered_map<int, CS>& cs) {
    cout << "\n";
    if (pipes.empty() && cs.empty())
    {
        cout << "You haven't created a single component\n\n";
        return;
    }

    if (!pipes.empty()) {
        for (const auto& [id, pipe] : pipes)
        {
            cout << pipe;
        }
    }
    else
    {
        cout << "You haven't created a single pipe.\n\n";
    }

    if (!cs.empty()) {
        for (const auto& [id, s] : cs)
        {
            cout << s;
        }
    }
    else
    {
        cout << "The Compression:\n";
        cout << "You haven't created a single cs.\n\n";
    }
}

void editPipe(const int id, unordered_map<int, Pipe>& pipes, bool working)
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

void editCS(const int id, unordered_map<int, CS>& cs, int percent)
{
    auto it = cs.find(id);
    if (it == cs.end()) {
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

void deletePipe(const int id, unordered_map<int, Pipe>& pipes)
{
    if (pipes.erase(id) == 0)
        cout << "Pipe with id " << id << " not found.\n";
    else
        cout << "Pipe with id " << id << " deleted.\n";
}

void deleteCS(const int id, unordered_map<int, CS>& cs)
{
    if (cs.erase(id) == 0)
        cout << "CS with id " << id << " not found.\n";
    else
        cout << "CS with id " << id << " deleted.\n";
}

void saveToFile(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& cs)
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
    for (const auto& [id, s] : cs)
        fout << s;

    cout << "\nWriting to the file " << filename << " is complete.\n\n";
}

Pipe readPipe(ifstream& fin)
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

CS readCS(ifstream& fin)
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

void loadFromFile(unordered_map<int, Pipe>& pipes, unordered_map<int, CS>& cs)
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
    cs.clear();

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
                if (!cs.emplace(s.getId(), s).second)
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
        cs.clear();
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

template<typename Obj, typename Pred>
vector<int> FindByFilter(const unordered_map<int, Obj>& items, Pred f)
{
    vector<int> res;
    for (const auto& [id, obj] : items)
        if (f(obj))
            res.push_back(id);
    sort(res.begin(), res.end());
    return res;
}

template<typename Obj>
void printFound(const unordered_map<int, Obj>& items, const vector<int>& ids)
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

double activePercent(const CS& s)
{
    if (s.getWorkshops() == 0) return 0.0;
    return 100.0 * s.getActiveWorkshops() / s.getWorkshops();
}

template<typename Obj>
vector<int> SearchByName(const unordered_map<int, Obj>& items)
{
    string name = inputName(cin, "Enter the name (or part of it): ");
    return FindByFilter(items, [&](const Obj& o) {
        return o.getName().find(name) != string::npos;
        });
}

vector<int> SearchPipesByRepair(const unordered_map<int, Pipe>& pipes)
{
    bool repair = inputInt(cin, "Find pipes: 1 - under repair (broken), 0 - working: ", 0, 1) == 1;
    return FindByFilter(pipes, [&](const Pipe& p) {
        return (!p.isWorking()) == repair;
        });
}

vector<int> SearchCSByActivePercent(const unordered_map<int, CS>& cs)
{
    int percent = inputInt(cin, "Enter the percent of working workshops (0-100): ", 0, 100);
    int mode = inputInt(cin, "1 - greater than or equal to, 2 - less than or equal to: ", 1, 2);

    return FindByFilter(cs, [&](const CS& s) {
        double p = activePercent(s);
        return (mode == 1) ? (p >= percent) : (p <= percent);
        });
}


vector<int> find_pipe(const unordered_map<int, Pipe>& pipes)
{
    if (pipes.empty())
    {
        cout << "\nYou haven't created a single pipe.\n\n";
        return {};
    }

    cout << "\nSearch pipes by:\n1 - name\n2 - under repair status\n";
    int mode = inputInt(cin, "Your choice: ", 1, 2);

    vector<int> ids = (mode == 1) ? SearchByName(pipes) : SearchPipesByRepair(pipes);
    printFound(pipes, ids);
    return ids;
}

vector<int> find_cs(const unordered_map<int, CS>& cs)
{
    if (cs.empty())
    {
        cout << "\nYou haven't created a single Compression station.\n\n";
        return {};
    }

    cout << "\nSearch Compression stations by:\n1 - name\n2 - percent of working workshops\n";
    int mode = inputInt(cin, "Your choice: ", 1, 2);

    vector<int> ids = (mode == 1) ? SearchByName(cs) : SearchCSByActivePercent(cs);
    printFound(cs, ids);
    return ids;
}

int inputPercent()
{
    return inputInt(cin, "Enter the percent of active workshops (0-100): ", 0, 100);
}

vector<int> chooseTargets(const vector<int>& found, int maxId, const string& idPrompt)
{
    int pick = inputInt(cin, "1 - choose all, 2 - select by id: ", 1, 2);
    if (pick == 1)
        return found;
    return { inputInt(cin, idPrompt, 1, maxId) };
}

int inputEditOrDelete()
{
    return inputInt(cin, "1 - edit, 2 - delete: ", 1, 2);
}

void batchPipes(unordered_map<int, Pipe>& pipes)
{
    vector<int> ids = find_pipe(pipes);
    if (ids.empty()) return;

    vector<int> targets = chooseTargets(ids, Pipe::getMaxID(), "Select pipe ID: ");
    if (inputEditOrDelete() == 1)
    {
        bool working = inputInt(cin, "Select the new status, where 1 - working, 0 - broken: ", 0, 1) == 1;
        for (int id : targets)
            editPipe(id, pipes, working);
    }
    else
        for (int id : targets)
            deletePipe(id, pipes);
}

void batchCS(unordered_map<int, CS>& cs)
{
    vector<int> ids = find_cs(cs);
    if (ids.empty()) return;

    vector<int> targets = chooseTargets(ids, CS::getMaxID(), "Select CS ID: ");
    if (inputEditOrDelete() == 1)
    {
        int percent = inputPercent();
        for (int id : targets) editCS(id, cs, percent);
    }
    else
        for (int id : targets) deleteCS(id, cs);
}

void find_object(unordered_map<int, Pipe>& pipes, unordered_map<int, CS>& cs)
{
    cout << "\n";
    if (pipes.empty() && cs.empty())
    {
        cout << "You haven't created a single component\n\n";
        return;
    }
    cout << "Chose, what you want to find:\n1 - pipe\n2 - CS\n";
    if (inputInt(cin, "Your choice: ", 1, 2) == 1)
        batchPipes(pipes);
    else
        batchCS(cs);
}

int main()
{
    redirect_output_wrapper cerr_out(cerr);
    string timestamp = std::format("{:%d_%m_%Y %H_%M_%OS}", chrono::system_clock::now());
    ofstream logfile("log_" + timestamp + ".txt");
    if (logfile)
        cerr_out.redirect(logfile);

    unordered_map<int, Pipe> pipes;
    unordered_map<int, CS> cs;

    while (true)
    {
        launching();
        int choice = inputInt(cin, "Enter a number to execute a command: ", 0, 6);

        switch (choice)
        {
        case 0:
            return 0;
        case 1:
            Add_pipe(pipes);
            break;
        case 2:
            Add_CS(cs);
            break;
        case 3:
            viewAll(pipes, cs);
            break;
        case 4:
            find_object(pipes, cs);
            break;
        case 5:
            saveToFile(pipes, cs);
            break;
        case 6:
            loadFromFile(pipes, cs);
            break;
        }
    }
}