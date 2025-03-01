#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h> // sleep function
#include <filesystem> // file system ob
#include <cstdlib> // exit func
#include <cmath>


using namespace std;

int main() {

    string filePath = "proc/stat";

    ifstream inputFile;
    inputFile.open(filePath);

    if (!inputFile.is_open()) {
        cerr << "Unable to open file: " << filePath << endl;
        exit(1);
    }

    string inputLine, title;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice, busyTimeSum1, idleTimeSum1;

    if (getline(inputFile, inputLine)) {
        stringstream ss(inputLine);
        ss >> title >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        cout << title << " " << user << " " << nice << " " << system << " " << idle << " " << iowait << " " << irq << " " << softirq << " " << steal << " " << guest << " " << guest_nice << endl;

        // calc busy time by summing everything except for idle time
        busyTimeSum1 = user + nice + system + iowait + irq + softirq + steal + guest + guest_nice;
        idleTimeSum1 = idle;
    } else {
        cerr << "Unable to get line of file: " << filePath << endl;
        exit(1);
    }

    // sleep for one sec and get stats again
    sleep(1);

    // move read position back to beginning
    inputFile.seekg(0);

    unsigned long long busyTimeSum2, idleTimeSum2;
    if (getline(inputFile, inputLine)) {
        stringstream ss(inputLine);
        ss >> title >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        cout << title << " " << user << " " << nice << " " << system << " " << idle << " " << iowait << " " << irq << " " << softirq << " " << steal << " " << guest << " " << guest_nice << endl;

        // calc busy time by summing everything except for idle time
        busyTimeSum2 = user + nice + system + iowait + irq + softirq + steal + guest + guest_nice;
        idleTimeSum2 = idle;
    } else {
        cerr << "Could not get line from file." << endl;
        exit(1);
    }

    inputFile.close();

    cout << "busyT1: " << busyTimeSum1 << endl;
    cout << "busyT2; " << busyTimeSum2 << endl;

    cout << "idleT1: " << idleTimeSum1 << endl;
    cout << "idleT2: " << idleTimeSum2 << endl;

    // calc cpu util
    unsigned long long busyTime, idleTime;
    busyTime = busyTimeSum2 - busyTimeSum1;
    idleTime = idleTimeSum2 - idleTimeSum1;
    cout << "busyT: " << busyTime << endl << "idle: " << idleTime << endl;
    double cpuUtil = (static_cast<double>(busyTime) / (busyTime + idleTime)) * 100.0;

    if (isnan(cpuUtil)) {
        cerr << "cpuUtil is NOT A NUMBER." << endl;
        exit(1);
    }

    // output formatted json
    cout << "CPU UTIL: " << cpuUtil << "%" << endl;


    return 0;
}