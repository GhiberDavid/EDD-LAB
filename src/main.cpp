#include <iostream>
#include "tree.h"

using namespace std;

void mostrarMenu() {
    cout << "\n=== MENU PRINCIPAL ===\n";
    cout << "1. Mostrar linea de sucesion\n";
    cout << "2. Simular muerte del jefe\n";
    cout << "3. Simular encarcelamiento del jefe\n";
    cout << "4. Simular envejecimiento del jefe (+70 años)\n";
    cout << "5. Actualizar datos de un miembro\n";
    cout << "6. Salir\n";
    cout << "Opcion: ";
}

int main() {
    cout << "=== Sistema de sucesion de la mafia ===\n";

    Tree tree;
    loadFromCSV(tree, "bin/data.csv");

    if (tree.getRoot() == nullptr) {
        cout << "Error: No se pudo cargar el arbol\n";
        return 1;
    } 

    cout << "\nJefe actual: " << tree.getRoot()->name << " " 
         << tree.getRoot()->last_name << " (Edad: " << tree.getRoot()->age << ")\n";
    
    int opcion;
    do {
        mostrarMenu();
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                tree.mostrarLineaSucesion();
                break;
            case 2:
                tree.getRoot()->is_dead = true;
                cout << "\n El jefe ha muerto...\n";
                tree.verificarYActualizarJefe();
                break;
            case 3:
                tree.getRoot()->in_jail = true;
                cout << "\n El jefe ha sido encarcelado...\n";
                tree.verificarYActualizarJefe();
                break;
            case 4:
                tree.getRoot()->age = 70;
                cout << "\n El jefe ha cumplido 70 años...\n";
                tree.verificarYActualizarJefe();
                break;
            case 5:
                cout << "Funcionalidad en desarrollo...\n";
                break;
            case 6:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opcion invalida\n";
        }
    } while(opcion != 6);

    return 0;
}