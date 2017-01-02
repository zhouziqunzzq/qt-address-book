#ifndef EMAILGROUPS_H
#define EMAILGROUPS_H
#include "elements.h"
#include "emailgroup.h"

class EmailGroups : public Elements<EmailGroup>
{
public:
    EmailGroups();
    friend class IOHelper;
    friend class MainWindow;
    friend class EmailGroupsDialog;
    ListNodePosi(EmailGroup) findByID(int id);
};

#endif // EMAILGROUPS_H
