#include <iostream>
#include <fstream>
using namespace std;

int main() {

    ifstream inputFile;

    inputFile.open("/proc/stat");

    string line;
    while(getline(inputFile, line)) {
        cout << "line: " << line << endl;
    }

    inputFile.close();

    return 0;
}