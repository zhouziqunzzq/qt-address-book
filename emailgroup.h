#ifndef EMAILGROUP_H
#define EMAILGROUP_H
#include "group.h"

class EmailGroup : public Group{
public:
    EmailGroup() : Group() {}
    EmailGroup(int id, std::string name) : Group(id, name) {}
    friend class EmailGroups;
    friend class PersonInfoDialog;
private:
};
#endif // EMAILGROUP_H
