#ifndef PERSONS_H
#define PERSONS_H
#include "elements.h"
#include "person.h"

class Persons : public Elements<Person>{
public:
    Persons();
    friend class MainWindow;
    friend class IOHelper;
    void updateID();
    ListNodePosi(Person) findByID(int id);
private:
};

#endif // PERSONS_H
