#include "email.h"

Email::Email()
{
    this->email = "";
    this->groupID = -1;
}

Email::Email(std::string addr, int id)
{
    this->email = addr;
    this->groupID = id;
}

bool Email::validateEmail()
{
    unsigned i,j,m;
    m=email.length();
    i=email.find("@");
    j=email.find(".");
    if(i==std::string::npos||j==std::string::npos||i>j||i>=m||j>=m||m>=30)
    {
        return true;
    }
    else
    {
        return false;
    }
}
