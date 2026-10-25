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

void editPipe(const int id, unordered_map<int, Pipe>& pipes)
{
    auto it = pipes.find(id);
    if (it == pipes.end()) {
        cout << "Pipe with id " << id << " not found.\n";
        return;
    }

    Pipe& p = it->second;

    cout << "Pipe with id=" << id
        << "\nlenght=" << p.length
        << "\ndiameter=" << p.diameter
        << "\nstatus: " << (p.working ? "working" : "broken") << "\n";

    p.working = !p.working;

    cout << "Status changed: \n"
        << (p.working ? "working" : "broken") << "\n";
}

void EditPipeMenu(unordered_map<int, Pipe>& pipes)
{
    cout << "\n";
    if (pipes.empty()) {
        cout << "There are no pipes creaeted.\n";
        return;
    }

    int id = inputInt(cin, "Select pipe ID: ", 1, Pipe::MaxID);
    editPipe(id, pipes);
}

void editCS(const int id, unordered_map<int, CS>& cs, int percent)
{
    auto it = cs.find(id);
    if (it == cs.end()) {
        cout << "CS with id " << id << " not found.\n";
        return;
    }

    CS& s = it->second;
    s.activeWorkshops = static_cast<int>(lround(s.workshops * percent / 100.0));

    cout << "CS with id=" << id
        << "\nname= " << s.name
        << "\nworkshops= " << s.workshops
        << "\nactive workshops= " << s.activeWorkshops
        << " (" << percent << "% requested)"
        << "\nstatus: " << s.status << "\n\n";
}

void EditCSMenu(unordered_map<int, CS>& cs)
{
    cout << "\n";
    if (cs.empty()) {
        cout << "There are no Compression stations created.\n";
        return;
    }

    int id = inputInt(cin, "Select CS ID: ", 1, CS::MaxID);
    int percent = inputInt(cin, "Enter the percent of active workshops (0-100): ", 0, 100);
    editCS(id, cs, percent);
}

void deletePipe(const int id, unordered_map<int, Pipe>& pipes)
{
    if (pipes.erase(id) == 0)
        cout << "Pipe with id " << id << " not found.\n";
    else
        cout << "Pipe with id " << id << " deleted.\n";
}

void DeletePipeMenu(unordered_map<int, Pipe>& pipes)
{
    cout << "\n";
    if (pipes.empty()) {
        cout << "There are no pipes created.\n";
        return;
    }

    int id = inputInt(cin, "Select pipe ID: ", 1, Pipe::MaxID);
    deletePipe(id, pipes);
}

void deleteCS(const int id, unordered_map<int, CS>& cs)
{
    if (cs.erase(id) == 0)
        cout << "CS with id " << id << " not found.\n";
    else
        cout << "CS with id " << id << " deleted.\n";
}

void DeleteCSMenu(unordered_map<int, CS>& cs)
{
    cout << "\n";
    if (cs.empty()) {
        cout << "There are no Compression stations created.\n";
        return;
    }

    int id = inputInt(cin, "Select CS ID: ", 1, CS::MaxID);
    deleteCS(id, cs);
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
                string line;
                if (!getline(fin, line)) throw runtime_error("Unexpected end of file");
                int id = stoi(line);
                if (id <= 0) throw runtime_error("Invalid pipe id");

                Pipe p(id);
                fin >> p;
                if (!fin) throw runtime_error("Unexpected end of file");
                if (!pipes.emplace(id, p).second)
                    throw runtime_error("Duplicate pipe id");
                maxPipeId = max(maxPipeId, id);
            }
            else if (marker == "C")
            {
                string line;
                if (!getline(fin, line)) throw runtime_error("Unexpected end of file");
                int id = stoi(line);
                if (id <= 0) throw runtime_error("Invalid CS id");

                CS s(id);  
                fin >> s;
                if (!fin) throw runtime_error("Unexpected end of file");
                if (!cs.emplace(id, s).second)
                    throw runtime_error("Duplicate CS id");
                maxCsId = max(maxCsId, id);
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

    Pipe::MaxID = maxPipeId;
    CS::MaxID = maxCsId;
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
    if (s.workshops == 0) return 0.0;
    return 100.0 * s.activeWorkshops / s.workshops;
}

template<typename Obj>
vector<int> SearchByName(const unordered_map<int, Obj>& items)
{
    string name = inputName(cin, "Enter the name: ");
    return FindByFilter(items, [&](const Obj& o) {
        return o.name == name;
        });
}

vector<int> SearchPipesByRepair(const unordered_map<int, Pipe>& pipes)
{
    bool repair = inputInt(cin, "Find pipes: 1 - under repair (broken), 0 - working: ", 0, 1) == 1;
    return FindByFilter(pipes, [&](const Pipe& p) {
        return (!p.working) == repair;
        });
}

vector<int> SearchCSByActivePercent(const unordered_map<int, CS>& cs)
{
    int percent = inputInt(cin, "Enter the percent of working workshops (0-100): ", 0, 100);
    return FindByFilter(cs, [&](const CS& s) {
        return activePercent(s) >= percent;
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

    vector<int> targets = chooseTargets(ids, Pipe::MaxID, "Select pipe ID: ");
    int cmd = inputEditOrDelete();
    for (int id : targets)
        cmd == 1 ? editPipe(id, pipes) : deletePipe(id, pipes);
}

void batchCS(unordered_map<int, CS>& cs)
{
    vector<int> ids = find_cs(cs);
    if (ids.empty()) return;

    vector<int> targets = chooseTargets(ids, CS::MaxID, "Select CS ID: ");
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