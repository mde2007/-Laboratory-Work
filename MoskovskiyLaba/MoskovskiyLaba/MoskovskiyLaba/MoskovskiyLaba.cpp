#include <iostream>
#include <fstream>
#include <string>
#include "Checks.h"
#include "Pipe.h"
#include "CS.h"

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

void viewAll(bool isPipe, bool isCS, const Pipe& p, const CS& s)
{
    cout << "\n";
    if (!isPipe && !isCS)
    {
        cout << "You haven't created a single component\n\n";
        return;
    }

    if (isPipe) cout << p;
    else
    {
        cout << "The pipeline:\n";
        cout << "You haven't created a single pipe.\n\n";
    }

    if (isCS) cout << s;
    else
    {
        cout << "The Compression:\n";
        cout << "You haven't created a single cs.\n\n";
    }
}

void editPipe(bool isPipe, Pipe& p)
{
    cout << "\n";
    if (!isPipe)
        cout << "You haven't created a single pipe.\n";
    else
    {
        p.working = inputInt(cin, "Select the pipeline status, where 1 - working, 0 - broken: ", 0, 1) == 1;
        cout << "\n";
    }
}

void editCS(bool isCS, CS& s)
{
    cout << "\n";
    if (!isCS)
        cout << "You haven't created a single Compression station\n";
    else
    {
        s.working = inputInt(cin, "Select the Compression station status, where 1 - working, 0 - broken: ", 0, 1) == 1;
        cout << "\n";
    }
}

void saveToFile(const Pipe& p, const CS& s, bool isPipe, bool isCS)
{
    ofstream fout("output.txt");
    if (!fout.is_open())
    {
        cout << "\nError: could not open file output.txt\n\n";
        return;
    }

    if (isPipe) fout << p;
    if (isCS)   fout << s;

    fout.close();
    cout << "\nWriting to the file is complete.\n\n";
}

void loadFromFile(Pipe& p, CS& s, bool& isPipe, bool& isCS)
{
    ifstream fin("output.txt");
    if (!fin.is_open())
    {
        cout << "\nError: could not open file output.txt\n\n";
        return;
    }

    bool error = false;
    try
    {
        while (fin.peek() != EOF)
        {
            char marker = fin.get();

            if (marker == 'P')
            {
                fin >> p;
                isPipe = true;
            }
            else if (marker == 'C')
            {
                fin >> s;
                isCS = true;
            }
        }
        if (fin.bad()) error = true;
    }
    catch (const exception&)
    {
        error = true;
    }
    fin.close();

    if (error)
    {
        cout << "\nError: something went wrong while reading the file.\n\n";
        return;
    }

    cout << "\nReading from the file is complete.\n\n";
}

int main()
{
    Pipe p;
    CS s;
    bool pipeCreated = false;
    bool csCreated = false;

    while (true)
    {
        launching();
        int choice = inputInt(cin, "Enter a number to execute a command: ", 0, 7);

        switch (choice)
        {
        case 0:
            return 0;
        case 1:
            cin >> p;
            pipeCreated = true;
            cout << "\nPipe added with id = " << p.getId() << "\n\n";
            break;
        case 2:
            cin >> s;
            csCreated = true;
            cout << "\nCS added with id = " << s.getId() << "\n\n";
            break;
        case 3:
            viewAll(pipeCreated, csCreated, p, s);
            break;
        case 4:
            editPipe(pipeCreated, p);
            break;
        case 5:
            editCS(csCreated, s);
            break;
        case 6:
            saveToFile(p, s, pipeCreated, csCreated);
            break;
        case 7:
            loadFromFile(p, s, pipeCreated, csCreated);
            break;
        }
    }
}