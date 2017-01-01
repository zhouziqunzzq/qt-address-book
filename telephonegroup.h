#ifndef TELEPHONEGROUP_H
#define TELEPHONEGROUP_H
#include "group.h"

class TelephoneGroup : public Group{
public:
    TelephoneGroup() : Group() {}
    TelephoneGroup(int id, std::string name) : Group(id, name) {}
    friend class TelephoneGroups;
    friend class PersonInfoDialog;
private:
};
#endif // TELEPHONEGROUP_H
