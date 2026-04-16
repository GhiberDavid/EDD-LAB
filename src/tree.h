#ifndef TREE_H
#define TREE_H

#include "person.h"
#include <string>

using namespace std;

class Tree {
private:
    Person* root;
    void buscarSucesorEnArbol(Person* nodo, Person*& sucesor);
    Person* encontrarJefeActual(Person* nodo);
    void mostrarLineaSucesionRec(Person* nodo, Person* jefeActual);

public:
    Tree();
    ~Tree();

    Person* getRoot();
    void setRoot(Person* node);
  

    void insert(Person* node);
    Person* findById(Person* node, int id);

    void mostrarLineaSucesion();
    void mostrarLineaSucesionRec(Person* nodo);

    bool verificarYActualizarJefe();
    Person* encontrarSucesor(Person* nodo);
    bool asignarNuevoJefe();

    
    bool actualizarMiembro(int id, string name, string last_name, char gender, 
                       int age, bool is_dead, bool in_jail, 
                       bool was_boss, bool is_boss);
    void mostrarMiembro(Person* p);

    void actualizarMiembroInteractivo();
   };

Person* createPersonFromCSV(string line);
void loadFromCSV(Tree& tree, const string& filename);

#endif