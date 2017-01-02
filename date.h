#ifndef DATE_H
#define DATE_H
class Person;

class Date
{
public:
    Date();
    Date(int y, int m, int d);
    friend class Person;
    friend class IOHelper;
    friend class PersonInfoDialog;
    friend class MainWindow;
    const Date& operator=(Date* d);
private:
    int year;
    int month;
    int day;
};

#endif // DATE_H
