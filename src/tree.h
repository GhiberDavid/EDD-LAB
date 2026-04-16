#ifndef TREE_H
#define TREE_H

#include "person.h"
#include <string>

using namespace std;

class Tree {
private:
    Person* root;
    void buscarSucesorEnArbol(Person* nodo, Person*& sucesor);

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
};

Person* createPersonFromCSV(string line);
void loadFromCSV(Tree& tree, const string& filename);

#endif