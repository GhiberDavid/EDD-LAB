#include "tree.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>  // Para remove_if
#include <cctype>     // Para isspace

using namespace std;

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
    
    try {
        // ID
        getline(ss, value, ',');
        if (value.empty()) value = "0";
        p->id = stoi(value);
        
        // Name
        getline(ss, p->name, ',');
        
        // Last name
        getline(ss, p->last_name, ',');
        
        // Gender
        getline(ss, value, ',');
        if (value.empty()) {
            p->gender = 'H';
        } else {
            p->gender = value[0];
        }
        
        // Age
        getline(ss, value, ',');
        if (value.empty()) value = "0";
        p->age = stoi(value);
        
        // ID Boss (acepta números negativos)
        getline(ss, value, ',');
        if (value.empty()) value = "0";

        // Eliminar espacios en blanco si es que existen
        value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
        p->id_boss = stoi(value);  // funcionando con -1
        
        // is_dead
        getline(ss, value, ',');
        if (value.empty()) value = "0";
        p->is_dead = stoi(value);
        
        // in_jail
        getline(ss, value, ',');
        if (value.empty()) value = "0";
        p->in_jail = stoi(value);
        
        // was_boss
        getline(ss, value, ',');
        if (value.empty()) value = "0";
        p->was_boss = stoi(value);
        
        // is_boss
        getline(ss, value, ',');
        if (value.empty()) value = "0";
        p->is_boss = stoi(value);
        
    } catch (const exception& e) {
        cout << "Error al parsear linea: " << line << endl;
        cout << "Error: " << e.what() << " - Valor problematico: " << value << endl;
        delete p;
        return nullptr;
    }
    
    // Inicializar punteros
    p->left = nullptr;
    p->right = nullptr;
    
    return p;
}

void loadFromCSV(Tree& tree, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir " << filename << endl;
        return;
    }
    
    string line;
    
    // Leer header (primera línea)
    if (!getline(file, line)) {
        cout << "Error: Archivo CSV vacio" << endl;
        file.close();
        return;
    }
    
    // Leer datos línea por línea
    int lineCount = 0;
    while (getline(file, line)) {
        lineCount++;
        
        // Saltar líneas vacías
        if (line.empty()) {
            continue;
        }
        
        // Eliminar espacios en blanco al inicio y final
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos) continue;
        line = line.substr(start);
        
        // Crear persona desde CSV
        Person* p = createPersonFromCSV(line);
        if (p != nullptr) {
            tree.insert(p);
        } else {
            cout << "Advertencia: Error en linea " << lineCount << endl;
        }
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
    
    // Encontrar al jefe actual (usando this-> para claridad)
    Person* jefeActual = this->encontrarJefeActual(root);
    
    // Recorrer todo el árbol desde la raíz estructural
    mostrarLineaSucesionRec(root, jefeActual);
}

Person* Tree::encontrarJefeActual(Person* nodo) {
    if (nodo == nullptr) return nullptr;
    if (nodo->is_boss) return nodo;
    
    Person* izquierdo = encontrarJefeActual(nodo->left);
    if (izquierdo != nullptr) return izquierdo;
    
    return encontrarJefeActual(nodo->right);
}

void Tree::mostrarLineaSucesionRec(Person* nodo, Person* jefeActual) {
    if (nodo == nullptr) return;

    //Recorrido in-order: izquierdo -> actual -> derecho

    //Primero se muestran los subordinados del hijo izquierdo (rama izquierda)
    mostrarLineaSucesionRec(nodo->left, jefeActual);

    // Mostrando el nodo actual si está vivo
    if (!nodo->is_dead) {
        cout << "ID: " << nodo->id
        << " | Nombre: " << nodo->name << " " << nodo->last_name
        << " | Edad: " << nodo->age;
        
        if (nodo == jefeActual && nodo->is_boss) {
            cout << " | * JEFE ACTUAL *";
        }
        if (nodo->in_jail) {
            cout << " | [ENCARCELADO]";
        }
        cout << endl;
    }

    //Luego se muestran los subordinados del hijo derecho (rama derecha)
    mostrarLineaSucesionRec(nodo->right, jefeActual);
}

bool Tree::verificarYActualizarJefe() {
    if (root == nullptr) return false;
    
    // Buscar quién es el jefe actual
    Person* jefeActual = this->encontrarJefeActual(root);
    if (jefeActual == nullptr) return false;
    
    // Verificar si el jefe actual debe ser reemplazado
    bool necesitaReemplazo = false;
    string razon;
    
    if (jefeActual->is_dead) {
        necesitaReemplazo = true;
        razon = "ha muerto";
    } else if (jefeActual->in_jail) {
        necesitaReemplazo = true;
        razon = "ha sido encarcelado";
    } else if (jefeActual->age >= 70) {
        necesitaReemplazo = true;
        razon = "ha superado los 70 años";
    }
    
    if (necesitaReemplazo) {
        cout << "\nEl jefe " << jefeActual->name << " " << jefeActual->last_name 
             << " " << razon << ". Buscando sucesor...\n";
        return asignarNuevoJefe();
    }
    
    return true;
}

void Tree::buscarSucesorEnArbol(Person* nodo, Person*& sucesor) {
    if (nodo == nullptr || sucesor != nullptr) return;
    
    buscarSucesorEnArbol(nodo->left, sucesor);
    
    // Verificando si este nodo es candidato
    if (sucesor == nullptr && !nodo->is_dead) {  // Nota: No se excluyen encarcelados aquí
        sucesor = nodo;
        return;
    }
    
    buscarSucesorEnArbol(nodo->right, sucesor);
}

bool Tree::asignarNuevoJefe() {
    Person* jefeActual = this->encontrarJefeActual(root);
    if (jefeActual == nullptr) return false;
    
    // Buscando sucesor con todas las opciones habilitadas
    Person* sucesor = encontrarSucesor(jefeActual, true, true, true);
    
    if (sucesor != nullptr) {
        jefeActual->is_boss = false;
        jefeActual->was_boss = true;
        sucesor->is_boss = true;
        
        cout << "Nuevo jefe asignado: " << sucesor->name << " " 
             << sucesor->last_name << " (ID: " << sucesor->id << ")\n";
        
        if (sucesor->in_jail) {
            cout << "Nota: El nuevo jefe está encarcelado.\n";
        }
        
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

Person* Tree::findParent(Person* actual, Person* hijo) {
    if (actual == nullptr) return nullptr;
    
    // Verificando si el actual es padre directo del hijo
    if (actual->left == hijo || actual->right == hijo) {
        return actual;
    }
    
    // Buscando recursivamente en el subárbol izquierdo
    Person* izquierdo = findParent(actual->left, hijo);
    if (izquierdo != nullptr) return izquierdo;
    
    // Buscando recursivamente en el subárbol derecho
    return findParent(actual->right, hijo);
}

Person* Tree::obtenerPadre(Person* nodo) {
    if (nodo == nullptr || root == nullptr) return nullptr;
    
    // El nodo raíz no tiene padre
    if (nodo == root) return nullptr;
    
    // Buscando el padre usando findParent
    return findParent(root, nodo);
}

Person* Tree::obtenerOtroSucesor(Person* nodo) {
    Person* padre = obtenerPadre(nodo);
    if (padre == nullptr) return nullptr;
    
    // Si el nodo es hijo izquierdo, devolver el derecho (si existe)
    if (padre->left == nodo) {
        return padre->right;
    }
    // Si es hijo derecho, devolver el izquierdo
    else if (padre->right == nodo) {
        return padre->left;
    }
    
    return nullptr;
}

Person* Tree::buscarSucesorEnArbolCompleto(Person* nodo) {
    if (nodo == nullptr) return nullptr;
    
    Person* sucesor = nullptr;
    buscarSucesorEnArbol(nodo, sucesor);
    return sucesor;
}

Person* Tree::encontrarSucesor(Person* nodo, bool buscarEnOtroSucesor, 
                               bool busquedaAscendente, bool incluirEncarcelados) {
    if (nodo == nullptr) return nullptr;
    
    Person* sucesor = nullptr;
    
    // NIVEL 1: Buscando en su propio árbol (hijos y descendientes)
    sucesor = buscarSucesorEnArbolCompleto(nodo);
    if (sucesor != nullptr) {
        cout << "Sucesor encontrado en su propio árbol: " << sucesor->name << endl;
        return sucesor;
    }
    
    // NIVEL 2: Buscando en el otro sucesor (hermano)
    if (buscarEnOtroSucesor && sucesor == nullptr) {
        Person* otroSucesor = obtenerOtroSucesor(nodo);
        if (otroSucesor != nullptr) {
            cout << "Buscando en el otro sucesor: " << otroSucesor->name << endl;
            sucesor = buscarSucesorEnArbolCompleto(otroSucesor);
            
            if (sucesor == nullptr && esDisponible(otroSucesor, incluirEncarcelados)) {
                sucesor = otroSucesor;
                cout << "El otro sucesor se convierte en jefe\n";
            }
            if (sucesor != nullptr) return sucesor;
        }
    }
    
    // NIVEL 3: Búsqueda ascendente (subir niveles de jefes)
    if (busquedaAscendente && sucesor == nullptr) {
        cout << "Iniciando búsqueda ascendente...\n";
        sucesor = buscarSucesorAscendente(nodo);
        if (sucesor != nullptr) return sucesor;
    }
    
    // NIVEL 4: Buscando jefe con dos sucesores
    if (sucesor == nullptr) {
        cout << "Buscando jefe con dos sucesores disponibles...\n";
        Person* jefeConDos = buscarJefeConDosSucesores(nodo);
        if (jefeConDos != nullptr) {
            cout << "Jefe encontrado: " << jefeConDos->name << endl;
            sucesor = buscarSucesorEnArbolCompleto(jefeConDos);
            if (sucesor != nullptr) return sucesor;
        }
    }
    
    // NIVEL 5: Incluyendo encarcelados como última opción
    if (incluirEncarcelados && sucesor == nullptr) {
        cout << "Ultimo recurso: Buscando sucesor incluyendo encarcelados...\n";
        sucesor = buscarSucesorIncluyendoEncarcelados(nodo);
        if (sucesor != nullptr) {
            cout << "Sucesor encontrado (encarcelado pero vivo): " << sucesor->name << endl;
            return sucesor;
        }
        
        // Si no hay en el árbol actual, buscar en toda la estructura
        sucesor = buscarSucesorIncluyendoEncarcelados(root);
        if (sucesor != nullptr) return sucesor;
    }
    
    return nullptr;
}

// Verificando si un nodo está disponible (vivo y opcionalmente fuera de cárcel)
bool Tree::esDisponible(Person* nodo, bool incluirEncarcelados) {
    if (nodo == nullptr) return false;
    if (nodo->is_dead) return false;
    if (!incluirEncarcelados && nodo->in_jail) return false;
    return true;
}

// Obteneniendo el jefe (padre) de un nodo
Person* Tree::obtenerJefe(Person* nodo) {
    return obtenerPadre(nodo);
}

// Contar cuántos sucesores disponibles tiene un nodo
int Tree::contarSucesoresDisponibles(Person* nodo, bool incluirEncarcelados) {
    if (nodo == nullptr) return 0;
    
    int contador = 0;
    
    // Contando en el subárbol izquierdo
    if (nodo->left != nullptr && esDisponible(nodo->left, incluirEncarcelados)) {
        contador++;
    }
    
    // Contando en el subárbol derecho
    if (nodo->right != nullptr && esDisponible(nodo->right, incluirEncarcelados)) {
        contador++;
    }
    
    return contador;
}

// Buscando el jefe más cercano con dos sucesores disponibles
Person* Tree::buscarJefeConDosSucesores(Person* nodo) {
    if (nodo == nullptr) return nullptr;
    
    // Verificando si el nodo actual tiene dos sucesores disponibles
    if (contarSucesoresDisponibles(nodo, false) >= 2) {
        return nodo;
    }
    
    // Buscando en el padre (ascendente)
    Person* padre = obtenerPadre(nodo);
    if (padre != nullptr) {
        return buscarJefeConDosSucesores(padre);
    }
    
    return nullptr;
}

// Buscando sucesor incluyendo encarcelados (último recurso)
Person* Tree::buscarSucesorIncluyendoEncarcelados(Person* nodo) {
    if (nodo == nullptr) return nullptr;
    
    Person* sucesor = nullptr;
    
    // Función auxiliar para buscar incluyendo encarcelados
    auto buscarConEncarcelados = [&](Person* actual, Person*& encontrado, auto&& buscarRef) -> void {
        if (actual == nullptr || encontrado != nullptr) return;
        
        buscarRef(actual->left, encontrado, buscarRef);
        
        if (encontrado == nullptr && esDisponible(actual, true)) {  // true = incluir encarcelados
            encontrado = actual;
            return;
        }
        
        buscarRef(actual->right, encontrado, buscarRef);
    };
    
    buscarConEncarcelados(nodo, sucesor, buscarConEncarcelados);
    return sucesor;
}

// Búsqueda ascendente: subir niveles de jefes hasta encontrar un sucesor
Person* Tree::buscarSucesorAscendente(Person* nodo) {
    if (nodo == nullptr) return nullptr;
    
    // Primero buscar en el propio árbol del nodo
    Person* sucesor = buscarSucesorEnArbolCompleto(nodo);
    if (sucesor != nullptr) return sucesor;
    
    // Si no hay, subir al jefe (padre)
    Person* jefe = obtenerPadre(nodo);
    if (jefe != nullptr) {
        cout << "   Subiendo al jefe: " << jefe->name << endl;
        
        // Buscar en el otro sucesor del jefe
        Person* otroSucesor = obtenerOtroSucesor(jefe);
        if (otroSucesor != nullptr && otroSucesor != nodo) {
            cout << "   Buscando en el otro sucesor del jefe: " << otroSucesor->name << endl;
            sucesor = buscarSucesorEnArbolCompleto(otroSucesor);
            if (sucesor != nullptr) return sucesor;
        }
        
        // Buscar en el árbol del jefe
        sucesor = buscarSucesorEnArbolCompleto(jefe);
        if (sucesor != nullptr) return sucesor;
        
        // Continuar subiendo
        return buscarSucesorAscendente(jefe);
    }
    
    return nullptr;
}