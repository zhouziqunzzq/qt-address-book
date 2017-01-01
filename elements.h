#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "list.h"
#include <cstring>

template <class T>
class Elements{
protected:
    List<T> _elem;  //内部数据元素集合（列表类）
    int maxID;  //最大ID
    virtual ListNodePosi(T) insertAsFirst(T const &e);
    virtual ListNodePosi(T) insertAsLast(T const &e);
    virtual ListNodePosi(T) insertA(ListNodePosi(T) p, T const &e);  //作为前驱插入
    virtual ListNodePosi(T) insertB(ListNodePosi(T) p, T const &e);  //作为后继插入

    //virtual void edit(ListNodePosi(T) p, T const &e);  //修改

    //virtual ListNodePosi(T) search(T const &e);  //查找

    //virtual void deleteAt(ListNodePosi(T));  //删除

public:
    Elements() : maxID(0) {}
    virtual long long int count() const {return this->_elem.size();}    //返回元素个数
    void sort() { this->_elem.sort();}
    T remove(ListNodePosi(T) e)
    {
        return this->_elem.remove(e);
    }
};

template <class T>
ListNodePosi(T) Elements<T>::insertAsFirst(T const &e)
{
    return this->_elem.insertAsFirst(e);
}

template <class T>
ListNodePosi(T) Elements<T>::insertAsLast(T const &e)
{
    return this->_elem.insertAsLast(e);
}

template <class T>
ListNodePosi(T) Elements<T>::insertA(ListNodePosi(T) p, T const &e)
{
    return this->_elem.insertA(p, e);
}

template <class T>
ListNodePosi(T) Elements<T>::insertB(ListNodePosi(T) p, T const &e)
{
    return this->_elem.insertB(p, e);
}

#endif // ELEMENTS_H
