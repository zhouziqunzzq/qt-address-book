#ifndef GROUP_H
#define GROUP_H

#include <string>

class Group{
protected:
    int id; //组编号
    std::string name;   //组名
public:
    Group() : id(0), name("") {}
    Group(int id, std::string name) : id(id), name(name) {}
    friend class IOHelper;
    friend class MainWindow;
    bool operator==(const Group &g) const {
        return this->id == g.id;
    }
};

#endif // GROUP_H
