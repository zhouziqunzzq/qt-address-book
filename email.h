#ifndef EMAIL_H
#define EMAIL_H
#include <string>

class Person;

class Email
{
public:
    Email();
    Email(std::string addr, int id);
    bool validateEmail();
    friend class Person;
    friend class IOHelper;
    friend class PersonInfoDialog;
    friend class MainWindow;
private:
    std::string email;
    int groupID;
};

#endif // EMAIL_H
