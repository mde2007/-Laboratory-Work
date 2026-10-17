#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Checks.h"
#include "Pipe.h"
#include "CS.h"
#include <unordered_map>

using namespace std;

int Pipe::MaxID = 0;
int CS::MaxID = 0;

void launching()
{
    cout << "App menu:\n";
    cout << "1 - Add pipe\n";
    cout << "2 - Add Compression station\n";
    cout << "3 - View all items\n";
    cout << "4 - Edit pipe\n";
    cout << "5 - Edit Compression station\n";
    cout << "6 - Save\n";
    cout << "7 - Download\n";
    cout << "8 - Find object\n";
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

//void editPipe(const unordered_map<int, Pipe>& pipes)
//{
//    cout << "\n";
//    if (pipes.empty())
//        cout << "You haven't created a single pipe.\n";
//    else
//    {
//        p.working = inputInt(cin, "Select the pipeline status, where 1 - working, 0 - broken: ", 0, 1) == 1;
//        cout << "\n";
//    }
//}
//
//void editCS(const unordered_map<int, CS>& cs)
//{
//    cout << "\n";
//    if (cs.empty())
//        cout << "You haven't created a single Compression station\n";
//    else
//    {
//        s.working = inputInt(cin, "Select the Compression station status, where 1 - working, 0 - broken: ", 0, 1) == 1;
//        cout << "\n";
//    }
//}
//
//void saveToFile(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& cs)
//{
//    ofstream fout("output.txt");
//    if (!fout.is_open())
//    {
//        cout << "\nError: could not open file output.txt\n\n";
//        return;
//    }
//
//    if (!pipes.empty()) fout << p;
//    if (!cs.empty())  fout << s;
//
//    fout.close();
//    cout << "\nWriting to the file is complete.\n\n";
//}
//
//void loadFromFile(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& cs)
//{
//    ifstream fin("output.txt");
//    if (!fin.is_open())
//    {
//        cout << "\nError: could not open file output.txt\n\n";
//        return;
//    }
//
//    bool error = false;
//    try
//    {
//        while (fin.peek() != EOF)
//        {
//            char marker = fin.get();
//
//            if (marker == 'P')
//            {
//                fin >> p;
//            }
//            else if (marker == 'C')
//            {
//                fin >> s;
//            }
//        }
//        if (fin.bad()) error = true;
//    }
//    catch (const exception&)
//    {
//        error = true;
//    }
//    fin.close();
//
//    if (error)
//    {
//        cout << "\nError: something went wrong while reading the file.\n\n";
//        return;
//    }
//
//    cout << "\nReading from the file is complete.\n\n";
//}

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

double unusedPercent(const CS& s)
{
    if (s.workshops == 0) return 0.0;
    return 100.0 * (s.workshops - s.activeWorkshops) / s.workshops;
}

void find_pipe(const unordered_map<int, Pipe>& pipes)
{
    if (pipes.empty())
    {
        cout << "\nYou haven't created a single pipe.\n\n";
        return;
    }

    cout << "\nSearch pipes by:\n1 - name\n2 - under repair status\n";
    int mode = inputInt(cin, "Your choice: ", 1, 2);

    vector<int> ids;
    if (mode == 1)
    {
        string name = inputName(cin, "Enter the name: ");
        ids = FindByFilter(pipes, [&](const Pipe& p) {
            return p.name == name;
            });
    }
    else
    {
        bool repair = inputInt(cin, "Find pipes: 1 - under repair (broken), 0 - working: ", 0, 1) == 1;
        ids = FindByFilter(pipes, [&](const Pipe& p) {
            return (!p.working) == repair;
            });
    }
    printFound(pipes, ids);
}

void find_cs(const unordered_map<int, CS>& cs)
{
    if (cs.empty())
    {
        cout << "\nYou haven't created a single Compression station.\n\n";
        return;
    }

    cout << "\nSearch Compression stations by:\n1 - name\n2 - percent of unused workshops\n";
    int mode = inputInt(cin, "Your choice: ", 1, 2);

    vector<int> ids;
    if (mode == 1)
    {
        string name = inputName(cin, "Enter the name: ");
        ids = FindByFilter(cs, [&](const CS& s) {
            return s.name == name;
            });
    }
    else
    {
        cout << "\n1 - at least\n2 - at most\n3 - exactly (rounded to a whole percent)\n";
        int cmp = inputInt(cin, "Comparison: ", 1, 3);
        int percent = inputInt(cin, "Enter the percent of unused workshops (0-100): ", 0, 100);
        ids = FindByFilter(cs, [&](const CS& s) {
            double u = unusedPercent(s);
            if (cmp == 1) return u >= percent;
            if (cmp == 2) return u <= percent;
            return lround(u) == percent;
            });
    }
    printFound(cs, ids);
}

void find_object(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CS>& cs)
{
    cout << "\n";
    if (pipes.empty() && cs.empty())
    {
        cout << "You haven't created a single component\n\n";
        return;
    }

    cout << "Chose, what you want to find:\n1 - pipe\n2 - CS\n";
    int user_choise = inputInt(cin, "Your choice: ", 1, 2);
    if (user_choise == 1)
        find_pipe(pipes);
    else
        find_cs(cs);
}

int main()
{
    unordered_map<int, Pipe> pipes;
    unordered_map<int, CS> cs;

    while (true)
    {
        launching();
        int choice = inputInt(cin, "Enter a number to execute a command: ", 0, 8);

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
            //case 4:
            //    editPipe(pipes);
            //    break;
            //case 5:
            //    editCS(cs);
            //    break;
            //case 6:
            //    saveToFile(pipes, cs);
            //    break;
            //case 7:
            //    loadFromFile(pipes, cs);
            //    break;
        case 8:
            find_object(pipes, cs);
            break;
        }
    }
}