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
    cout << "App menu. Enter a number to execute a command:\n";
    cout << "1 - Add pipe\n2 - Add Compression station\n3 - View all items\n4 - Edit pipe\n5 - Edit Compression station\n6 - Save\n7 - Download\n0 - Exit\n";

}


int main()
{

    launching();
}


