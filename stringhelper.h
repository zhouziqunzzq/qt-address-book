#ifndef MYSTRINGHELPER_H
#define MYSTRINGHELPER_H
#include <string>

class StringHelper
{
public:
    StringHelper();
    static std::string UTF8ToGBK(const std::string& strUTF8);    //转换UTF编码到GBK实现按拼音排序
};

#endif // MYSTRINGHELPER_H
