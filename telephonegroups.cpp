#include "telephonegroups.h"

TelephoneGroups::TelephoneGroups()
{

}

ListNodePosi(TelephoneGroup) TelephoneGroups::findByID(int id)
{
    TelephoneGroup e;
    e.id = id;
    return this->_elem.find(e);
}
