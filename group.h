#ifndef GROUP_H
#define GROUP_H

#include <string>

class Group{
protected:
    int id; //组编号
    int count;  //元素数量
    std::string name;   //组名
public:
    Group() : id(-1), name(""), count(0) {}
    Group(int id, int c, std::string name) : id(id), count(c), name(name) {}
    friend class IOHelper;
    friend class MainWindow;
    friend class PersonInfoDialog;
    friend class PersonGroupInfo;
    friend class PersonGroupsDialog;
    friend class TelephoneInfoDialog;
    friend class TelephoneGroupInfo;
    friend class TelephoneGroupsDialog;
    friend class EmailInfoDialog;
    friend class EmailGroupInfo;
    friend class EmailGroupsDialog;
    bool operator==(const Group &g) const {
        return this->id == g.id;
    }
};

#endif // GROUP_H
