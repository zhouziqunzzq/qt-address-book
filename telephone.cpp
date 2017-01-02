#include "telephone.h"

Telephone::Telephone()
{
    this->tel = "";
    this->groupID = -1;
}

Telephone::Telephone(std::string t, int id)
{
    this->tel = t;
    this->groupID = id;
}
