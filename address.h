#ifndef ADDRESS_H
#define ADDRESS_H
#include <string>
class Person;

class Address
{
public:
    Address();
    Address(std::string con, std::string pro, std::string city, std::string detail, std::string postal);
    friend class Person;
    friend class IOHelper;
    const Address& operator=(Address *addr);
private:
    std::string country;
    std::string province;
    std::string city;
    std::string detail;
    std::string postal;
};

#endif // ADDRESS_H
