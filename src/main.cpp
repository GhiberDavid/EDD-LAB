#include <iostream>
#include "tree.h"

using namespace std;

void loadFromCSV(const string& filename);

int main() {
    cout << "Sistema de sucesion de la mafia\n";

    Tree tree;

   if (tree.getRoot() != nullptr) {
    cout << "Root: " << tree.getRoot()->name << endl;
} else {
    cout << "Root es NULL (arbol no construido)" << endl;
}

    cout << "Root: " << tree.getRoot()->name << endl;
    loadFromCSV(tree, "data.csv");

    return 0;
}