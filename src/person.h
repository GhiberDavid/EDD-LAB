#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

struct Person {
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_boss;

    bool is_dead;
    bool in_jail;
    bool was_boss;
    bool is_boss;

    Person* left;
    Person* right;
};

#endif