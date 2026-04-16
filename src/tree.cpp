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

void Tree::mostrarMiembro(Person* p) {
    if (p == nullptr) return;
    
    cout << "\n=== DATOS DEL MIEMBRO ===\n";
    cout << "ID: " << p->id << endl;
    cout << "Nombre: " << p->name << " " << p->last_name << endl;
    cout << "Genero: " << p->gender << endl;
    cout << "Edad: " << p->age << endl;
    cout << "ID Jefe: " << p->id_boss << endl;
    cout << "Vivo: " << (p->is_dead ? "No" : "Si") << endl;
    cout << "En carcel: " << (p->in_jail ? "Si" : "No") << endl;
    cout << "Fue jefe: " << (p->was_boss ? "Si" : "No") << endl;
    cout << "Es jefe: " << (p->is_boss ? "Si" : "No") << endl;
}

bool Tree::actualizarMiembro(int id, string name, string last_name, char gender,
                             int age, bool is_dead, bool in_jail,
                             bool was_boss, bool is_boss) {
    Person* persona = findById(root, id);
    
    if (persona == nullptr) {
        cout << "Error: No se encontro un miembro con ID " << id << endl;
        return false;
    }
    
    cout << "\n--- Datos actuales ---\n";
    mostrarMiembro(persona);
    
    cout << "\n--- Actualizando datos ---\n";

    //No permitir marcar manualmenet a alguien como jefe
    if (is_boss && persona!= root) {
        cout << "No se puede asignar manualmente el puesto de jefe.\n";
        cout << "El jefe solo puede ser designado por el sistema de sucesion.\n";
        is_boss = false;
    }
    
    // No se permite quitar la marca de jefe al jefe actual (solo el sistema puede)
    if (!is_boss && persona == root && persona->is_boss) {
        cout << "No se puede destituir al jefe manualmente.\n";
        cout << "Solo puede ser reemplazado por muerte, cárcel o edad.\n";
        is_boss = true;  // Manteniendo como jefe
    }

    // Actualizando campos (excepto id e id_boss)
    persona->name = name;
    persona->last_name = last_name;
    persona->gender = gender;
    persona->age = age;
    persona->is_dead = is_dead;
    persona->in_jail = in_jail;
    persona->was_boss = was_boss;
    persona->is_boss = is_boss;
    
    cout << "Miembro actualizado correctamente\n";
    
    // Verificando si el jefe actual fue modificado
    if (persona == root && (is_dead || in_jail || age>= 70)) {
        cout << "Se actualizo al jefe actual. Verificando reemplazo...\n";
        verificarYActualizarJefe();
    }
    
    return true;
}

void Tree::actualizarMiembroInteractivo() {
    int id;
    cout << "Ingrese el ID del miembro a actualizar: ";
    cin >> id;
    
    Person* p = findById(root, id);
    if (p == nullptr) {
        cout << "ID no encontrado\n";
        return;
    }
    
    string name, last_name;
    char gender;
    int age;
    int is_dead, in_jail, was_boss, is_boss;
    
    cout << "\n--- Ingrese nuevos datos ---\n";
    
    cout << "Nombre [" << p->name << "]: ";
    cin.ignore();
    getline(cin, name);
    if (name.empty()) name = p->name;
    
    cout << "Apellido [" << p->last_name << "]: ";
    getline(cin, last_name);
    if (last_name.empty()) last_name = p->last_name;
    
    cout << "Genero (H/M) [" << p->gender << "]: ";
    cin >> gender;
    if (gender != 'H' && gender != 'M') gender = p->gender;
    
    cout << "Edad [" << p->age << "]: ";
    cin >> age;
    if (age < 0) age = p->age;
    
    cout << "Vivo (1=no, 0=si) [" << p->is_dead << "]: ";
    cin >> is_dead;
    if (is_dead != 0 && is_dead != 1) is_dead = p->is_dead;
    
    cout << "En carcel (1=si, 0=no) [" << p->in_jail << "]: ";
    cin >> in_jail;
    if (in_jail != 0 && in_jail != 1) in_jail = p->in_jail;
    
    cout << "Fue jefe (1=si, 0=no) [" << p->was_boss << "]: ";
    cin >> was_boss;
    if (was_boss != 0 && was_boss != 1) was_boss = p->was_boss;
    
    //El sistema decide quien es el jefe, no el usuario
    cout << "\n Nota: EL puesto de jefe es asignado automaticamente por el sistema.\n";

    actualizarMiembro(id, name, last_name, gender, age, 
                      is_dead == 1, in_jail == 1, 
                      was_boss == 1, false); //is_boss colocado siempre false al actualizar manualmente
}