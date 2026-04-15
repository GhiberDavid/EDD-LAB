#ifndef TREE_H
#define TREE_H

#include "person.h"
#include <string>

using namespace std;

class Tree {
private:
    Person* root;

public:
    Tree();
    void setRoot(Person* node);
    Person* getRoot();

    void insert(Person* node);
    Person* findById(Person* node, int id);
};


void loadFromCSV(Tree& tree, const string& filename);

#endif