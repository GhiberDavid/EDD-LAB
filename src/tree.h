#ifndef TREE_H
#define TREE_H

#include "person.h"

class Tree {
private:
    Person* root;

public:
    Tree();
    void setRoot(Person* node);
    Person* getRoot();
};

#endif