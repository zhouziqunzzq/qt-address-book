#include "telephone.h"

Telephone::Telephone()
{
    this->tel = "";
    this->groupID = 0;
}

Telephone::Telephone(std::string t, int id)
{
    this->tel = t;
    this->groupID = id;
}
