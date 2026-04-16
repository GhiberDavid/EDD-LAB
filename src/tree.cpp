#include "tree.h"
#include <fstream>
#include <sstream>
#include <iostream>

Tree::Tree() {
    root = nullptr;
}

Tree::~Tree() {
}

void Tree::setRoot(Person* node) {
    root = node;
}

Person* Tree::getRoot() {
    return root;
}

void loadFromCSV(Tree& tree, const string& filename) {
    ifstream file(filename);
    string line;

    getline(file, line); // header

    while (getline(file, line)) {
        Person* p = createPersonFromCSV(line);

        tree.insert(p);
    }

    file.close();
}

Person* Tree::findById(Person* node, int id) {
    if (node == nullptr) return nullptr;

    if (node->id == id)
        return node;

    Person* leftResult = findById(node->left, id);
    if (leftResult != nullptr) return leftResult;

    return findById(node->right, id);
}

void Tree::insert(Person* node) {

    if (root == nullptr) {
        root = node;
        return;
    }

    Person* boss = findById(root, node->id_boss);

    if (boss != nullptr) {
        if (boss->left == nullptr) {
            boss->left = node;
        }
        else if (boss->right == nullptr) {
            boss->right = node;
        } else {
            //Ambos hijos ocupados - buscar un lugar
            cout << "Advertencia: " << boss->name << " ya tiene dos hijos" << endl;

        }
    }
}


Person* createPersonFromCSV(string line) {
    stringstream ss(line);
    string value;

    Person* p = new Person();

    getline(ss, value, ','); p->id = stoi(value);
    getline(ss, p->name, ',');
    getline(ss, p->last_name, ',');
    getline(ss, value, ','); p->gender = value[0];
    getline(ss, value, ','); p->age = stoi(value);
    getline(ss, value, ','); p->id_boss = stoi(value);
    getline(ss, value, ','); p->is_dead = stoi(value);
    getline(ss, value, ','); p->in_jail = stoi(value);
    getline(ss, value, ','); p->was_boss = stoi(value);
     getline(ss, value, ','); p->is_boss = stoi(value);

    return p;
}

void loadFromCSV(Tree& tree, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir " << filename << endl;
        return;
    }
    
    string line;
    getline(file, line); // header
    
    while (getline(file, line)) {
        Person* p = createPersonFromCSV(line);
        tree.insert(p);
    }
    
    file.close();
    cout << "Datos cargados correctamente desde " << filename << endl;
}