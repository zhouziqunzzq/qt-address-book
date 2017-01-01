#ifndef ADDRESS_H
#define ADDRESS_H
#include <string>
class Person;

class Address
{
public:
    Address();
    Address(std::string con, std::string pro, std::string city, std::string detail);
    friend class Person;
    friend class IOHelper;
    friend class PersonInfoDialog;
    const Address& operator=(Address *addr);
private:
    std::string country;
    std::string province;
    std::string city;
    std::string detail;
};

#endif // ADDRESS_H
