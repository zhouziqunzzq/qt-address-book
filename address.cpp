#include "address.h"
#include <cstring>

Address::Address()
{

}

Address::Address(std::string con, std::string pro, std::string city, std::string detail, std::string postal)
{
    this->country = con;
    this->province = pro;
    this->city = city;
    this->detail = detail;
    this->postal = postal;
}

const Address& Address::operator=(Address *addr)
{
    this->country = addr->country;
    this->province = addr->province;
    this->city = addr->city;
    this->detail = addr->detail;
    this->postal = addr->postal;
    return *this;
}
