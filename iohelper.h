#ifndef IOHELPER_H
#define IOHELPER_H
#include <string>
#include "tinyxml2.h"
//需要存储的类：EmailGroups, PersonGroups, TelephoneGroups, Persons, Settings
#include "persons.h"
#include "settings.h"
#include "persongroup.h"
#include "persongroups.h"
#include "telephone.h"
#include "telephonegroup.h"
#include "telephonegroups.h"
#include "email.h"
#include "emailgroup.h"
#include "emailgroups.h"

using namespace tinyxml2;

class IOHelper  //程序数据文件输入输出工具类
{
public:
    IOHelper();
    /*Persons*/
    bool readPersons(Persons &persons); //读入联系人
    bool savePersons(const Persons &persons) const; //保存联系人
    /*Settings*/
    bool readSettings(Settings &settings);  //读入设置
    bool saveSettings(const Settings &settings) const;  //保存设置
    /*PersonGroups*/
    bool readPersonGroups(PersonGroups &persongroups);  //读入联系人分组
    bool savePersonGroups(const PersonGroups &persongroups) const;  //保存联系人分组
    /*TelephoneGroups*/
    bool readTelephoneGroups(TelephoneGroups &telephonegroups); //读入电话分组
    bool saveTelephoneGroups(const TelephoneGroups &telephonegroups) const;   //保存电话分组
    /*EmailGroups*/
    bool readEmailGroups(EmailGroups &emailgroups); //读入email分组
    bool saveEmailGroups(const EmailGroups &emailgroups) const; //保存email分组
private:
    //std::string dataPath;   //数据文件存储路径
    std::string personsFileName;
    std::string settingsFileName;
    std::string persongroupsFileName;
    std::string telephonegroupsFileName;
    std::string emailgroupsFileName;
    std::string getXMLElementTextSafe(XMLElement* e);
};

#endif // IOHELPER_H
