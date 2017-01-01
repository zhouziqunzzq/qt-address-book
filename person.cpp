#include "person.h"
// : id(0), groupID(0), name(""), company(""), address(), birthday(), memo("")
Person::Person()
{
    id = 0;
    groupID = 0;
    name = "";
    memo = "";
}

bool Person::operator<=(const Person &p) const
{
    return StringHelper::UTF8ToGBK(this->name) <= StringHelper::UTF8ToGBK(p.name);
}

bool Person::operator==(const Person &p) const
{
    return this->id == p.id;
}
