#include "person.h"

Person::Person()
{
    id = 0;
    groupID = -1;   //未分组
    company = "";
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
