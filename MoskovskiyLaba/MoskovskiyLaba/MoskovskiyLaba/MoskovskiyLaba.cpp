#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <format>
#include "Checks.h"
#include "GasNet.h"

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

int main()
{
    redirect_output_wrapper cerr_out(cerr);
    string timestamp = std::format("{:%d_%m_%Y %H_%M_%OS}", chrono::system_clock::now());
    ofstream logfile("log_" + timestamp + ".txt");
    if (logfile)
        cerr_out.redirect(logfile);

    GasNetwork network;

    while (true)
    {
        launching();
        int choice = inputInt(cin, "Enter a number to execute a command: ", 0, 6);

        switch (choice)
        {
        case 0:
            return 0;
        case 1:
            network.addPipe();
            break;
        case 2:
            network.addCS();
            break;
        case 3:
            network.viewAll();
            break;
        case 4:
            network.findObject();
            break;
        case 5:
            network.save();
            break;
        case 6:
            network.load();
            break;
        }
    }
}