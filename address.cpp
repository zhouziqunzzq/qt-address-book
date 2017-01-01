#include "address.h"
#include <cstring>

Address::Address()
{

}

Address::Address(std::string con, std::string pro, std::string city, std::string detail)
{
    this->country = con;
    this->province = pro;
    this->city = city;
    this->detail = detail;
}

const Address& Address::operator=(Address *addr)
{
    this->country = addr->country;
    this->province = addr->province;
    this->city = addr->city;
    this->detail = addr->detail;
    return *this;
}
