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
    cout << "1 - Add pipe\n2 - Add Compression station\n3 - View all items\n4 - Edit pipe\n5 - Edit Compression station\n6 - Save\n7 - Download\n0 - Exit\nEnter a number to execute a command: ";

}

void Add_pipe(Pipe& p) {
    cout << "\n";
    cout << "Enter the pipeline Name: ";
    cin >> p.Pipe_Name;
    cout << "Enter the pipeline lenght in km: ";
    cin >> p.Pipe_lenght;
    cout << "Enter the pipeline diametr in mm: ";
    cin >> p.Pipe_diameter;
    cout << "Select the pipline status, where 1 - working, 0 - broken: ";
    cin >> p.Pipe_attribute;
    cout << "\n";
}

void Add_compression(CS& s) {
    cout << "\n";
    cout << "Enter the Compression station Name: ";
    cin >> s.CS_name;
    cout << "Enter the Compression station lenght in km: ";
    cin >> s.workshops;
    cout << "Enter the Compression station diametr in mm: ";
    cin >> s.active_workshops;
    cout << "Select the Compression station, where 1 - working, 0 - broken: ";
    cin >> s.station_class;
    cout << "\n";
}

void view_all(bool is_pipe, bool is_cs, Pipe& p, CS& s) {
    if (!is_pipe && !is_cs) {
        cout << "\n";
        cout << "You haven't created a single component" << endl << "\n";
    }
    else {
        if (is_pipe) {
            cout << "\n";
            cout << "The pipeline:" << endl;
            cout << "The pipeline Name: " << p.Pipe_Name << endl;
            cout << "The pipeline lenght in km: " << p.Pipe_lenght << endl;
            cout << "The pipeline diametr in mm: " << p.Pipe_diameter << endl;
            cout << "The pipline status, where 1 - working, 0 - broken: " << p.Pipe_attribute << endl << "\n";
        }
        else {
            cout << "\n";
            cout << "The pipeline:" << endl;
            cout << "You haven't created a single pipe." << endl << "\n";
        }
        if (is_cs) {
            cout << "The Compression:" << endl;
            cout << "The Compression station Name: " << s.CS_name << endl;
            cout << "The pipeline lenght in km: " << s.workshops << endl;
            cout << "The pipeline diametr in mm: " << s.active_workshops << endl;
            cout << "The pipline status, where 1 - working, 0 - broken: " << s.station_class << endl << "\n";
        }
        else {
            cout << "The Compression:" << endl;
            cout << "You haven't created a single cs." << endl << "\n";
        }
    }
}

void edit_pipe(bool is_pipe, Pipe& p) {
    if (!is_pipe) {
        cout << "\n";
        cout << "You haven't created a single pipe." << endl;
    }
    else {
        cout << "\n";
        cout << "Select the pipline status, where 1 - working, 0 - broken: ";
        cin >> p.Pipe_attribute;
        cout << "\n";
    }
}

void edit_cs(bool is_cs, CS& s) {
    if (!is_cs) {
        cout << "\n";
        cout << "You haven't created a single Compression station \n";
    }
    else {
        cout << "Select the Compression station status, where 1 - working, 0 - broken: ";
        cin >> s.station_class;
        cout << "\n";
    }
}
void save_to_file(Pipe& p, CS& s, bool is_pipe, bool is_cs){
    ofstream fout;
    fout.open("output.txt", ios::out);
    if (fout.is_open()) {
        if (is_pipe) {
            fout <<"P" << p.Pipe_Name << endl << p.Pipe_diameter << endl << p.Pipe_lenght << endl << p.Pipe_attribute << endl;
        }
        if (is_cs) {
            fout << "C" << s.CS_name << endl << s.workshops << endl << s.active_workshops << endl << s.station_class << endl;
        }
        fout.close();
    }
    cout << "\nWriting from the file is complete.\n\n";

}

void get_from_file(Pipe& p, CS& s, bool& is_pipe, bool& is_cs) {
    ifstream fin("output.txt");
    if (fin.is_open()) {
        while (fin.peek() != -1) {
            char atr = fin.get();

            if (atr == 'P') {
                is_pipe = true;
                string name;
                getline(fin, name);
                p.Pipe_Name = name;

                for (int i = 0; i < 3; i++) {
                    string str;
                    getline(fin, str);

                    switch (i) {
                    case 0: {
                        double x = stoi(str);
                        p.Pipe_lenght = x;
                        break;
                    }
                    case 1: {
                        float x = stoi(str);
                        p.Pipe_diameter = x;
                        break;
                    }
                    case 2: {
                        bool x = stoi(str);
                        p.Pipe_attribute = x;
                        break;
                    }
                    }
                }
            }
            else {
                is_cs = true;
                string name;
                getline(fin, name);
                s.CS_name = name;

                for (int i = 0; i < 3; i++) {
                    string str;
                    getline(fin, str);

                    switch (i) {
                    case 0: {
                        int x = stoi(str);
                        s.workshops = x;
                        break;
                    }
                    case 1: {
                        int x = stoi(str);
                        s.active_workshops = x;
                        break;
                    }
                    case 2: {
                        s.station_class = str[0]; 
                        break;
                    }
                    }
                }
            }
        }
        fin.close();
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
        cin >> user_choise;
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


