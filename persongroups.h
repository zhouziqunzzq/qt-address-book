#ifndef PERSONGROUPS_H
#define PERSONGROUPS_H
#include "elements.h"
#include "persongroup.h"

class PersonGroups : public Elements<PersonGroup>
{
public:
    PersonGroups();
    friend class IOHelper;
    friend class MainWindow;
    friend class PersonGroupsDialog;
    ListNodePosi(PersonGroup) findByID(int id);
};

#endif // PERSONGROUPS_H
