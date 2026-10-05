#include <iostream>
#include <fstream>
#include <string>
using namespace std;


struct Pipe {
    string Pipe_Name;
    double Pipe_lenght;
    float Pipe_diameter;
    bool Pipe_attribute;
};

struct CS {
    string CS_name;
    int workshops;
    int active_workshops;
    char station_class;
};

void launching() {
    cout << "App menu:\n";
    cout << "1 - Add pipe\n2 - Add Compression station\n3 - View all items\n4 - Edit pipe\n5 - Edit Compression station\n6 - Save\n7 - Download\n0 - Exit\n";

}

bool error_check(string c) {
    if (cin.good()) {
        return true;
    }

    cout << "Error, try again.\n";
    cin.clear();
    cin.ignore(100, '\n');
    cout << c;
    return false;
}

bool Is_Correct_type(string c) {
    if (cin.peek() == '\n') {
        cin.ignore(100, '\n');
        return true;
    }

    cout << "Error, the input doesn't match the type. Try again.\n";
    cin.clear();
    cin.ignore(100, '\n');
    cout << c;
    return false;
}

bool Is_Positive_mumber(double n, string c) {
    if (n > 0) {
        return true;
    }

    cout << "Error, the number must be positive. Try again.\n";
    cout << c;
    return false;
}

bool Is_Not_Empty(string str, string c) {
    if (!str.empty()) {
        return true;
    }

    cout << "Error, the name must not be empty. Try again.\n";
    cout << c;
    return false;
}

bool Is_Available_command(int n, string c) {
    if (n >= 0 && n <= 7) {
        return true;
    }

    cout << "Error, there is no such command. Choose a number from 0 to 7.\n";
    cout << c;
    return false;
}


bool Is_Status(char c, string prompt) {
    if (c == '0' || c == '1') {
        return true;
    }

    cout << "Error, the status must be 0 or 1. Try again.\n";
    cout << prompt;
    return false;
}

void Add_pipe(Pipe& p) {
    cout << "\n";
    string comand;

    comand = "Enter the pipeline Name: ";
    cout << comand;
    getline(cin >> ws, p.Pipe_Name);
    while (!Is_Not_Empty(p.Pipe_Name, comand)) {
        getline(cin, p.Pipe_Name);
    }

    comand = "Enter the pipeline lenght in km: ";
    cout << comand;
    cin >> p.Pipe_lenght;
    while (!error_check(comand) || !Is_Correct_type(comand) || !Is_Positive_mumber(p.Pipe_lenght, comand)) {
        cin >> p.Pipe_lenght;
    }

    comand = "Enter the pipeline diametr in mm: ";
    cout << comand;
    cin >> p.Pipe_diameter;
    while (!error_check(comand) || !Is_Correct_type(comand) || !Is_Positive_mumber(p.Pipe_diameter, comand)) {
        cin >> p.Pipe_diameter;
    }

    comand = "Select the pipline status, where 1 - working, 0 - broken: ";
    cout << comand;
    cin >> p.Pipe_attribute;
    while (!error_check(comand) || !Is_Correct_type(comand)) {
        cin >> p.Pipe_attribute;
    }

    cout << "\n";
}


void Add_compression(CS& s) {
    cout << "\n";
    string comand;

    comand = "Enter the Compression station Name: ";
    cout << comand;
    getline(cin >> ws, s.CS_name);
    while (!Is_Not_Empty(s.CS_name, comand)) {
        getline(cin, s.CS_name);
    }

    comand = "Enter the Compression station number of workshops: ";
    cout << comand;
    cin >> s.workshops;
    while (!error_check(comand) || !Is_Correct_type(comand) || !Is_Positive_mumber(s.workshops, comand)) {
        cin >> s.workshops;
    }

    comand = "Enter the Compression station number of active workshops: ";
    cout << comand;
    while (true) {
        cin >> s.active_workshops;
        if (!error_check(comand) || !Is_Correct_type(comand)) {
            continue;
        }
        if (s.active_workshops < 0 || s.active_workshops > s.workshops) {
            cout << "Error, the number of active workshops must be from 0 to " << s.workshops << ". Try again.\n";
            cout << comand;
            continue;
        }
        break;
    }

    comand = "Select the Compression station, where 1 - working, 0 - broken: ";
    cout << comand;
    cin >> s.station_class;
    while (!error_check(comand) || !Is_Correct_type(comand) || !Is_Status(s.station_class, comand)) {
        cin >> s.station_class;
    }

    cout << "\n";
}

void print_p(const Pipe& p) {
    cout << "The pipeline:" << endl;
    cout << "The pipeline Name: " << p.Pipe_Name << endl;
    cout << "The pipeline lenght in km: " << p.Pipe_lenght << endl;
    cout << "The pipeline diametr in mm: " << p.Pipe_diameter << endl;
    cout << "The pipline status, where 1 - working, 0 - broken: " << p.Pipe_attribute << endl << "\n";
}

void print_s(const CS& s) {
    cout << "The Compression:" << endl;
    cout << "The Compression station Name: " << s.CS_name << endl;
    cout << "The Compression station number of workshops: " << s.workshops << endl;
    cout << "The Compression station number of active workshops: " << s.active_workshops << endl;
    cout << "The Compression station status, where 1 - working, 0 - broken: " << s.station_class << endl << "\n";
}

void view_all(bool is_pipe, bool is_cs, Pipe& p, CS& s) {
    cout << "\n";
    if (!is_pipe && !is_cs) {
        cout << "You haven't created a single component" << endl << "\n";
        return;
    }

    if (is_pipe) {
        print_p(p);
    }
    else {
        cout << "The pipeline:" << endl;
        cout << "You haven't created a single pipe." << endl << "\n";
    }

    if (is_cs) {
        print_s(s);
    }
    else {
        cout << "The Compression:" << endl;
        cout << "You haven't created a single cs." << endl << "\n";
    }
}

void edit_pipe(bool is_pipe, Pipe& p) {
    if (!is_pipe) {
        cout << "\n";
        cout << "You haven't created a single pipe." << endl;
    }
    else {
        cout << "\n";
        string comand = "Select the pipline status, where 1 - working, 0 - broken: ";
        cout << comand;
        cin >> p.Pipe_attribute;
        while (!error_check(comand) || !Is_Correct_type(comand)) {
            cin >> p.Pipe_attribute;
        }
        cout << "\n";
    }
}

void edit_cs(bool is_cs, CS& s) {
    if (!is_cs) {
        cout << "\n";
        cout << "You haven't created a single Compression station \n";
    }
    else {
        string comand = "Select the Compression station status, where 1 - working, 0 - broken: ";
        cout << comand;
        cin >> s.station_class;
        while (!error_check(comand) || !Is_Correct_type(comand) || !Is_Status(s.station_class, comand)) {
            cin >> s.station_class;
        }
        cout << "\n";
    }
}

void save_pipe(Pipe& p, ofstream& fout) {
    if (fout.is_open()) {
        fout << "P" << p.Pipe_Name << endl << p.Pipe_lenght << endl << p.Pipe_diameter << endl << p.Pipe_attribute << endl;
    }
}

void save_cs(CS& s, ofstream& fout) {
    if (fout.is_open()) {
        fout << "C" << s.CS_name << endl << s.workshops << endl << s.active_workshops << endl << s.station_class << endl;
    }
}
void save_to_file(Pipe& p, CS& s, bool is_pipe, bool is_cs) {
    ofstream fout;
    fout.open("output.txt", ios::out);
    if (is_pipe) {
        save_pipe(p, fout);
    }
    if (is_cs) {
        save_cs(s, fout);
    }
    cout << "\nWriting from the file is complete.\n\n";

}

void get_pipe(Pipe& p,ifstream & fin) {
    string name;
    getline(fin, name);
    p.Pipe_Name = name;

    string str;

    getline(fin, str);
    p.Pipe_lenght = stod(str);

    getline(fin, str);
    p.Pipe_diameter = stof(str);

    getline(fin, str);
    p.Pipe_attribute = stoi(str);
}

void get_cs(CS& s, ifstream& fin) {
    string name;
    getline(fin, name);
    s.CS_name = name;

    string str;

    getline(fin, str);
    s.workshops = stoi(str);

    getline(fin, str);
    s.active_workshops = stoi(str);

    getline(fin, str);
    s.station_class = str[0];
}

void get_from_file(Pipe& p, CS& s, bool& is_pipe, bool& is_cs) {
    ifstream fin("output.txt");
    if (!fin.is_open()) {
        cout << "\nError: could not open file output.txt\n\n";
        return;
    }

    bool error = false;
    try {
        while (fin.peek() != -1) {
            char atr = fin.get();
            if (atr == 'P') {
                get_pipe(p, fin);
                is_pipe = true;
            }
            else {
                get_cs(s, fin);
                is_cs = true;
            }
        }

        if (fin.bad()) {
            error = true;
        }
    }
    catch (const exception& e) {
        cout << "\nRead error: " << "\n";
        error = true;
    }

    fin.close();

    if (error) {
        cout << "\nError: something went wrong while reading the file.\n\n";
        return;
    }

    cout << "\nReading from the file is complete.\n\n";
}


int main()
{
    Pipe p;
    CS s;
    bool pipe_create = false;
    bool cs_create = false;
    while (true) {
        launching();
        int user_choise;
        string comand = "Enter a number to execute a command: ";
        cout << comand;
        cin >> user_choise;
        while (!error_check(comand) || !Is_Correct_type(comand) || !Is_Available_command(user_choise, comand)) {
            cin >> user_choise;
        }
        switch (user_choise) {
        case 0:
            return 0;
        case 1:
            Add_pipe(p);
            pipe_create = true;
            break;
        case 2:
            Add_compression(s);
            cs_create = true;
            break;
        case 3:
            view_all(pipe_create, cs_create, p, s);
            break;
        case 4:
            edit_pipe(pipe_create, p);
            break;
        case 5:
            edit_cs(cs_create, s);
            break;
        case 6:
            save_to_file(p, s, pipe_create, cs_create);
            break;
        case 7:
            get_from_file(p, s, pipe_create, cs_create);
            break;
        }

    }
}