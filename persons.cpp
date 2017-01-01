#include "persons.h"

Persons::Persons()
{

}

void Persons::updateID()
{
    ListNodePosi(Person) it = this->_elem.first();
    for(long long int i = 0; i < this->count(); ++i, it = it->succ)
        it->data.id = i;
}

ListNodePosi(Person) Persons::findByID(int id)
{
    Person p;
    p.id = id;
    return this->_elem.find(p);
}
