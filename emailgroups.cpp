#include "emailgroups.h"

EmailGroups::EmailGroups()
{

}

ListNodePosi(EmailGroup) EmailGroups::findByID(int id)
{
    EmailGroup e;
    e.id = id;
    return this->_elem.find(e);
}
