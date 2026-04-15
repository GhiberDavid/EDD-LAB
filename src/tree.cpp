#include "tree.h"
#include <fstream>
#include <sstream>
#include <iostream>

Tree::Tree() {
    root = nullptr;
}

void Tree::setRoot(Person* node) {
    root = node;
}

Person* Tree::getRoot() {
    return root;
}

Person* createPersonFromCSV(string line) {
    stringstream ss(line);
    string value;

    Person* p = new Person();

    getline(ss, value, ',');
    p->id = stoi(value);

    getline(ss, p->name, ',');

    getline(ss, p->last_name, ',');

    getline(ss, value, ',');
    p->gender = value[0];

    getline(ss, value, ',');
    p->age = stoi(value);

    getline(ss, value, ',');
    p->id_boss = stoi(value);

    getline(ss, value, ',');
    p->is_dead = stoi(value);

    getline(ss, value, ',');
    p->in_jail = stoi(value);

    getline(ss, value, ',');
    p->was_boss = stoi(value);

    getline(ss, value, ',');
    p->is_boss = stoi(value);

    return p;
}

void loadFromCSV(const string& filename) {
    ifstream file(filename);
    string line;

    cout << "Entrando a loadFromCSV..." << endl;

    if (!file.is_open()) {
        cout << "Error al abrir el archivo\n";
        return;
    }

    // Saltar encabezado
    getline(file, line);

    while (getline(file, line)) {
        Person* p = createPersonFromCSV(line);
        cout << "LINEA RAW: [" << line << "]" << endl;
        cout << "Leido: " << p->name << " " << p->last_name << endl;
    }

    file.close();
}
