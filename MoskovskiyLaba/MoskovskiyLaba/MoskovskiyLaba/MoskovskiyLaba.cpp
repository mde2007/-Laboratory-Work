#include <iostream>
#include <fstream>
#include <string>
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
    const unordered_map<int, CS>& cs){
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

int main()
{
    unordered_map<int, Pipe> pipes;
    unordered_map<int, CS> cs;

    while (true)
    {
        launching();
        int choice = inputInt(cin, "Enter a number to execute a command: ", 0, 7);

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
        }
    }
}