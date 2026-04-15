#include <iostream>
#include "tree.h"

using namespace std;

void loadFromCSV(const string& filename);

int main() {
    cout << "Sistema de sucesion de la mafia\n";

    loadFromCSV("./bin/data.csv");

    return 0;
}