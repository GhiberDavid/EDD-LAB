#include <iostream>
#include "tree.h"

using namespace std;

int main() {
    cout << "=== Sistema de sucesion de la mafia ===\n" << endl;

    Tree tree;

    loadFromCSV(tree, "bin/data.csv");

    //Verificando que se cargó la raíz
       if (tree.getRoot() != nullptr) {
        cout << "\nJefe actual: " << tree.getRoot()->name << " " 
             << tree.getRoot()->last_name << endl;
    } else {
        cout << "Error: No se pudo cargar el arbol" << endl;
        return 1;
    }

    //Mostrando línea de sucesión
    tree.mostrarLineaSucesion();

    return 0;
}