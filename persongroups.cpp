#include "persongroups.h"

PersonGroups::PersonGroups()
{

}

ListNodePosi(PersonGroup) PersonGroups::findByID(int id)
{
    PersonGroup e;
    e.id = id;
    return this->_elem.find(e);
}
