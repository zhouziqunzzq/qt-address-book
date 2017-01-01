#include "date.h"
#include <cstring>

Date::Date()
{
    this->year = 0;
    this->month = 0;
    this->day = 0;
}

Date::Date(int y, int m, int d)
{
    this->year = y;
    this->month = m;
    this->day = d;
}

const Date& Date::operator=(Date* d)
{
    memcpy(this, d, sizeof(Date));
    return *this;
}
