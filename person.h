#ifndef PERSON_H
#define PERSON_H
#include "email.h"
#include "address.h"
#include "date.h"
#include "telephone.h"
#include "stringhelper.h"
#include <string>
#include <vector>

class Person
{
public:
    Person();
    friend class MainWindow;
    friend class IOHelper;
    friend class Persons;
    friend class PersonInfoDialog;
    bool operator<=(const Person &p) const;
    bool operator==(const Person &p) const;
private:
    int id;
    std::string name;
    int groupID;
    std::vector<Telephone> telephone;
    std::vector<Email> email;
    std::string company;
    Address address;
    Date birthday;
    std::string memo;
};

#endif // PERSON_H
