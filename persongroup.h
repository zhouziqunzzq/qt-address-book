#ifndef PERSONGROUP_H
#define PERSONGROUP_H
#include "group.h"

class PersonGroup : public Group{
public:
    PersonGroup() : Group() {}
    PersonGroup(int id, std::string name) : Group(id, 0, name) {}
    friend class PersonGroups;
private:
};


#endif // PERSONGROUP_H
