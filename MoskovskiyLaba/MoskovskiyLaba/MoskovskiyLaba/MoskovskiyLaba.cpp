#include <iostream>
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

void launching(){
    cout << "App menu:\n";
    cout << "1 - Add pipe\n2 - Add Compression station\n3 - View all items\n4 - Edit pipe\n5 - Edit Compression station\n6 - Save\n7 - Download\n0 - Exit\nEnter a number to execute a command: ";

}

void Add_pipe() {
    Pipe p;
    cout << "Enter the pipeline Name: ";
    cin >> p.Pipe_Name;
    cout << "Enter the pipeline lenght in km: ";
    cin >> p.Pipe_lenght;
    cout << "Enter the pipeline diametr in mm: ";
    cin >> p.Pipe_diameter;
    cout << "Select the pipline status, where 1 - working, 0 - broken: ";
    cin >> p.Pipe_attribute;
}

void Add_compression() {
    CS s;
    cout << "Enter the Compression station Name: ";
    cin >> s.CS_name;
    cout << "Enter the pipeline lenght in km: ";
    cin >> s.workshops;
    cout << "Enter the pipeline diametr in mm: ";
    cin >> s.active_workshops;
    cout << "Select the pipline status, where 1 - working, 0 - broken: ";
    cin >> s.station_class;
}

void view_all() {
    
}


int main()
{
    while (true) {
       launching();
       int user_choise;
       cin >> user_choise;
       if (user_choise == 0) {
           return 0;
       }
       if (user_choise == 1) {
           Add_pipe();
       }
       if (user_choise == 2) {
           Add_compression();
       }

    }
}


