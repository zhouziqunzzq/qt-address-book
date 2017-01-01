#ifndef TELEPHONE_H
#define TELEPHONE_H
#include <string>
class Person;

class Telephone
{
public:
    Telephone();
    Telephone(std::string t, int id);
    friend class Person;
    friend class IOHelper;
    friend class PersonInfoDialog;
private:
    std::string tel;
    int groupID;
};

#endif // TELEPHONE_H
