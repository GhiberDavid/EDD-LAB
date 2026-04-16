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

Person* Tree::findById(Person* node, int id) {
    if (node == nullptr) return nullptr;

    if (node->id == id) return node;

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

void Tree::mostrarLineaSucesion() {
    cout << "\n=== LINEA DE SUCESION (Miembros vivos) ===\n";
    if (root == nullptr) {
        cout << "El arbol esta vacio.\n";
        return;
    }
    mostrarLineaSucesionRec(root);
}

void Tree::mostrarLineaSucesionRec(Person* nodo) {
    if (nodo == nullptr) return;

    //Recorrido in-order: izquierdo -> actual -> derecho

    //Primero se muestran los subordinados del hijo izquierdo (rama izquierda)
    mostrarLineaSucesionRec(nodo->left);

    // Mostrando el nodo actual si está vivo
    if (!nodo->is_dead) {
        cout << "ID: " << nodo->id
        << " | Nombre: " << nodo->name << " " << nodo->last_name
        << " | Edad: " << nodo->age;
        
        if (nodo->is_boss) {
            cout << " | * JEFE ACTUAL *";
        }
        if (nodo->in_jail) {
            cout << " | [ENCARCELADO]";
        }
        cout << endl;
    }

    //Luego se muestran los subordinados del hijo derecho (rama derecha)
    mostrarLineaSucesionRec(nodo->right);
}

bool Tree::verificarYActualizarJefe() {
    if (root == nullptr) return false;
    
    // Verificar si el jefe actual debe ser reemplazado
    bool necesitaReemplazo = false;
    string razon;
    
    if (root->is_dead) {
        necesitaReemplazo = true;
        razon = "ha muerto";
    } else if (root->in_jail) {
        necesitaReemplazo = true;
        razon = "esta en la carcel";
    } else if (root->age >= 70) {
        necesitaReemplazo = true;
        razon = "supero los 70 años";
    }
    
    if (necesitaReemplazo) {
        cout << "\nEl jefe " << root->name << " " << root->last_name 
             << " " << razon << ". Buscando sucesor...\n";
        return asignarNuevoJefe();
    }
    
    return true;
}

void Tree::buscarSucesorEnArbol(Person* nodo, Person*& sucesor) {
    if (nodo == nullptr || sucesor != nullptr) return;
    
    // Recorrido in-order para encontrar el primer disponible
    buscarSucesorEnArbol(nodo->left, sucesor);
    
    // Verificar si este nodo es candidato (vivo, fuera de cárcel, y no es el jefe actual)
    if (sucesor == nullptr && !nodo->is_dead && !nodo->in_jail && nodo != root) {
        sucesor = nodo;
        return;
    }
    
    buscarSucesorEnArbol(nodo->right, sucesor);
}

Person* Tree::encontrarSucesor(Person* nodo) {
    if (nodo == nullptr) return nullptr;
    
    Person* sucesor = nullptr;
    
    // Primero buscar en su subárbol izquierdo (hijos y descendientes)
    buscarSucesorEnArbol(nodo->left, sucesor);
    
    // Si no se encuentra, buscar en el subárbol derecho
    if (sucesor == nullptr) {
        buscarSucesorEnArbol(nodo->right, sucesor);
    }
    
    return sucesor;
}

bool Tree::asignarNuevoJefe() {
    Person* sucesor = encontrarSucesor(root);
    
    if (sucesor != nullptr) {
        // Quitar la marca de jefe al actual
        root->is_boss = false;
        root->was_boss = true;
        
        // Asignar nuevo jefe
        sucesor->is_boss = true;
        
        cout << "Nuevo jefe asignado: " << sucesor->name << " " 
             << sucesor->last_name << " (ID: " << sucesor->id << ")\n";
        
        // Actualizar la raíz del árbol
        root = sucesor;
        
        return true;
    } else {
        cout << "No se encontro un sucesor valido para el jefe.\n";
        return false;
    }
}