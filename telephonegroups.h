#ifndef TELEPHONEGROUPS_H
#define TELEPHONEGROUPS_H
#include "elements.h"
#include "telephonegroup.h"

class TelephoneGroups : public Elements<TelephoneGroup>
{
public:
    TelephoneGroups();
    friend class IOHelper;
    friend class MainWindow;
    friend class TelephoneGroupsDialog;
    ListNodePosi(TelephoneGroup) findByID(int id);
};

#endif // TELEPHONEGROUPS_H
