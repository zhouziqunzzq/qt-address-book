#include "iohelper.h"
#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "listNode.h"
#include <QString>
#include <vector>

using namespace tinyxml2;

std::string IOHelper::getXMLElementTextSafe(XMLElement* e)
{
    return e->GetText() == 0 ? "" : e->GetText();
}

IOHelper::IOHelper()
{
    this->personsFileName = "persons.xml";
    this->settingsFileName = "settings.xml";
    this->persongroupsFileName = "persongroups.xml";
    this->telephonegroupsFileName = "telephonegroups.xml";
    this->emailgroupsFileName = "emailgroups.xml";
}
/*Persons*/
bool IOHelper::readPersons(Persons &persons) //读入联系人
{
    XMLDocument *doc = new XMLDocument;
    if(doc->LoadFile(this->personsFileName.c_str()) != XMLError::XML_SUCCESS)
        return false;
    if(!doc->NoChildren())
    {
        XMLNode *root = doc->FirstChild();
        if(!root->NoChildren())
        {
            int i = 0;
            for(XMLElement *personElem = root->FirstChildElement("Person");;++i, personElem = personElem->NextSiblingElement("Person"))
            {
                Person p;
                //id
                p.id = personElem->IntAttribute("id");
                persons.maxID = p.id > persons.maxID ? p.id : persons.maxID;
                //name
                p.name = getXMLElementTextSafe(personElem->FirstChildElement("name"));
                //groupID
                p.groupID = QString::fromStdString(getXMLElementTextSafe(personElem->FirstChildElement("groupID"))).toInt();
                //telephone
                Telephone tel;
                XMLElement *telsElem = personElem->FirstChildElement("telephones");
                if(telsElem != NULL && !telsElem->NoChildren())
                {
                    for(XMLElement *telElem = telsElem->FirstChildElement("telephone");;telElem = telElem->NextSiblingElement("telephone"))
                    {
                        tel.groupID = telElem->IntAttribute("groupID");
                        tel.tel = getXMLElementTextSafe(telElem);
                        p.telephone.push_back(tel);
                        //end
                        if(telElem == telsElem->LastChildElement("telephone"))
                            break;
                    }
                }
                //email
                Email email;
                XMLElement *emailsElem = personElem->FirstChildElement("emails");
                if(emailsElem != NULL && !emailsElem->NoChildren())
                {
                    for(XMLElement *emailElem = emailsElem->FirstChildElement("email");;emailElem = emailElem->NextSiblingElement("email"))
                    {
                        email.groupID = emailElem->IntAttribute("groupID");
                        email.email = getXMLElementTextSafe(emailElem);
                        p.email.push_back(email);
                        //end
                        if(emailElem == emailsElem->LastChildElement("email"))
                            break;
                    }
                }
                //company
                p.company = getXMLElementTextSafe(personElem->FirstChildElement("company"));
                //address
                Address addr;
                XMLElement *addrElem = personElem->FirstChildElement("address");
                addr.country = getXMLElementTextSafe(addrElem->FirstChildElement("country"));
                addr.province = getXMLElementTextSafe(addrElem->FirstChildElement("province"));
                addr.city = getXMLElementTextSafe(addrElem->FirstChildElement("city"));
                addr.detail = getXMLElementTextSafe(addrElem->FirstChildElement("detail"));
                addr.postal = getXMLElementTextSafe(addrElem->FirstChildElement("postal"));
                p.address = addr;
                //birthday
                Date birthday;
                XMLElement *birthdayElem = personElem->FirstChildElement("birthday");
                birthday.year = QString::fromStdString(getXMLElementTextSafe(birthdayElem->FirstChildElement("year"))).toInt();
                birthday.month = QString::fromStdString(getXMLElementTextSafe(birthdayElem->FirstChildElement("month"))).toInt();
                birthday.day = QString::fromStdString(getXMLElementTextSafe(birthdayElem->FirstChildElement("day"))).toInt();
                p.birthday = birthday;
                //memo
                p.memo = getXMLElementTextSafe(personElem->FirstChildElement("memo"));
                //push
                persons._elem.insertAsLast(p);
                //end
                if(personElem == root->LastChildElement("Person"))
                    break;
            }
        }
    }
    return true;
}

bool IOHelper::savePersons(const Persons &persons) const //保存联系人
{
    XMLDocument *doc = new XMLDocument();
    XMLNode *root = doc->InsertFirstChild(doc->NewElement("Persons"));
    ListNodePosi(Person) it = persons._elem.first();
    for(int i = 0; i < persons.count(); ++i, it = it->succ)
    {
        XMLElement *personElem = doc->NewElement("Person");
        personElem->SetAttribute("id", i);
        XMLNode *personNode = root->InsertEndChild(personElem);
        //name
        XMLNode *personNameNode = personNode->InsertEndChild(doc->NewElement("name"));
        personNameNode->InsertEndChild(doc->NewText(it->data.name.c_str()));
        //groupid
        XMLNode *personGroupIDNode = personNode->InsertEndChild(doc->NewElement("groupID"));
        personGroupIDNode->InsertEndChild(doc->NewText(QString::number(it->data.groupID).toStdString().c_str()));
        //telephone
        XMLNode *telephonesNode = personNode->InsertEndChild(doc->NewElement("telephones"));
        for(std::vector<Telephone>::iterator telit = it->data.telephone.begin(); telit != it->data.telephone.end(); ++telit)
        {
            XMLElement *telephoneElem = doc->NewElement("telephone");
            telephoneElem->SetAttribute("groupID", telit->groupID);
            XMLNode *telephoneNode = telephonesNode->InsertEndChild(telephoneElem);
            telephoneNode->InsertEndChild(doc->NewText(telit->tel.c_str()));
        }
        //email
        XMLNode *emailsNode = personNode->InsertEndChild(doc->NewElement("emails"));
        for(std::vector<Email>::iterator eit = it->data.email.begin(); eit != it->data.email.end(); ++eit)
        {
            XMLElement *emailElem = doc->NewElement("email");
            emailElem->SetAttribute("groupID", eit->groupID);
            XMLNode *emailNode = emailsNode->InsertEndChild(emailElem);
            emailNode->InsertEndChild(doc->NewText(eit->email.c_str()));
        }
        //company
        XMLNode *companyNode = personNode->InsertEndChild(doc->NewElement("company"));
        companyNode->InsertEndChild(doc->NewText(it->data.company.c_str()));
        //address
        XMLNode *addressNode = personNode->InsertEndChild(doc->NewElement("address"));
        XMLNode *countryNode = addressNode->InsertEndChild(doc->NewElement("country"));
        countryNode->InsertEndChild(doc->NewText(it->data.address.country.c_str()));
        XMLNode *provinceNode = addressNode->InsertEndChild(doc->NewElement("province"));
        provinceNode->InsertEndChild(doc->NewText(it->data.address.province.c_str()));
        XMLNode *cityNode = addressNode->InsertEndChild(doc->NewElement("city"));
        cityNode->InsertEndChild(doc->NewText(it->data.address.city.c_str()));
        XMLNode *detailNode = addressNode->InsertEndChild(doc->NewElement("detail"));
        detailNode->InsertEndChild(doc->NewText(it->data.address.detail.c_str()));
        XMLNode *postalNode = addressNode->InsertEndChild(doc->NewElement("postal"));
        postalNode->InsertEndChild(doc->NewText(it->data.address.postal.c_str()));
        //birthday
        XMLNode *birthdayNode = personNode->InsertEndChild(doc->NewElement("birthday"));
        XMLNode *yearNode = birthdayNode->InsertEndChild(doc->NewElement("year"));
        yearNode->InsertEndChild(doc->NewText(QString::number(it->data.birthday.year).toStdString().c_str()));
        XMLNode *monthNode = birthdayNode->InsertEndChild(doc->NewElement("month"));
        monthNode->InsertEndChild(doc->NewText(QString::number(it->data.birthday.month).toStdString().c_str()));
        XMLNode *dayNode = birthdayNode->InsertEndChild(doc->NewElement("day"));
        dayNode->InsertEndChild(doc->NewText(QString::number(it->data.birthday.day).toStdString().c_str()));
        //memo
        XMLNode *memoNode = personNode->InsertEndChild(doc->NewElement("memo"));
        memoNode->InsertEndChild(doc->NewText(it->data.memo.c_str()));
    }
    //Write out xml file
    if(doc->SaveFile(this->personsFileName.c_str()) == XMLError::XML_SUCCESS)
        return true;
    else
        return false;
}
/*Settings*/
bool IOHelper::readSettings(Settings &settings)  //读入设置
{
    XMLDocument *doc = new XMLDocument;
    if(doc->LoadFile(this->settingsFileName.c_str()) != XMLError::XML_SUCCESS)
        return false;
    if(!doc->NoChildren())
    {
        XMLElement *settingsElem = doc->FirstChildElement("Settings");
        settings.username = getXMLElementTextSafe(settingsElem->FirstChildElement("username"));
        settings.password = getXMLElementTextSafe(settingsElem->FirstChildElement("password"));
    }
    return true;
}

bool IOHelper::saveSettings(const Settings &settings) const  //保存设置
{
    XMLDocument *doc = new XMLDocument();
    XMLNode *root = doc->InsertFirstChild(doc->NewElement("Settings"));
    XMLNode *usernameNode = root->InsertEndChild(doc->NewElement("username"));
    usernameNode->InsertEndChild(doc->NewText(settings.username.c_str()));
    XMLNode *passwordNode = root->InsertEndChild(doc->NewElement("password"));
    passwordNode->InsertEndChild(doc->NewText(settings.password.c_str()));
    if(doc->SaveFile(this->settingsFileName.c_str()) == XMLError::XML_SUCCESS)
        return true;
    else
        return false;
}
/*PersonGroups*/
bool IOHelper::readPersonGroups(PersonGroups &persongroups)  //读入联系人分组
{
    XMLDocument *doc = new XMLDocument;
    if(doc->LoadFile(this->persongroupsFileName.c_str()) != XMLError::XML_SUCCESS)
        return false;
    if(!doc->NoChildren())
    {
        XMLNode *root = doc->FirstChild();
        if(!root->NoChildren())
        {
            int i = 0;
            for(XMLElement *persongroupElem = root->FirstChildElement("PersonGroup");;++i, persongroupElem = persongroupElem->NextSiblingElement("PersonGroup"))
            {
                PersonGroup pg;
                //id
                pg.id = persongroupElem->IntAttribute("id");
                persongroups.maxID = pg.id > persongroups.maxID ? pg.id : persongroups.maxID;
                //name
                pg.name = getXMLElementTextSafe(persongroupElem->FirstChildElement("name"));
                //push
                persongroups._elem.insertAsLast(pg);
                //end
                if(persongroupElem == root->LastChildElement("PersonGroup"))
                    break;
            }
        }
    }
    return true;
}

bool IOHelper::savePersonGroups(const PersonGroups &persongroups) const  //保存联系人分组
{
    XMLDocument *doc = new XMLDocument();
    XMLNode *root = doc->InsertFirstChild(doc->NewElement("PersonGroups"));
    ListNodePosi(PersonGroup) it = persongroups._elem.first();
    for(int i = 0; i < persongroups.count(); ++i, it = it->succ)
    {
        XMLElement *persongroupElem = doc->NewElement("PersonGroup");
        persongroupElem->SetAttribute("id", i);
        XMLNode *persongroupNode = root->InsertEndChild(persongroupElem);
        //name
        XMLNode *nameNode = persongroupNode->InsertEndChild(doc->NewElement("name"));
        nameNode->InsertEndChild(doc->NewText(it->data.name.c_str()));
    }
    //Write out xml file
    if(doc->SaveFile(this->persongroupsFileName.c_str()) == XMLError::XML_SUCCESS)
        return true;
    else
        return false;
}
/*TelephoneGroups*/
bool IOHelper::readTelephoneGroups(TelephoneGroups &telephonegroups) //读入电话分组
{
    XMLDocument *doc = new XMLDocument;
    if(doc->LoadFile(this->telephonegroupsFileName.c_str()) != XMLError::XML_SUCCESS)
        return false;
    if(!doc->NoChildren())
    {
        XMLNode *root = doc->FirstChild();
        if(!root->NoChildren())
        {
            int i = 0;
            for(XMLElement *telephonegroupElem = root->FirstChildElement("TelephoneGroup");;++i, telephonegroupElem = telephonegroupElem->NextSiblingElement("TelephoneGroup"))
            {
                TelephoneGroup tg;
                //id
                tg.id = telephonegroupElem->IntAttribute("id");
                telephonegroups.maxID = tg.id > telephonegroups.maxID ? tg.id : telephonegroups.maxID;
                //name
                tg.name = getXMLElementTextSafe(telephonegroupElem->FirstChildElement("name"));
                //push
                telephonegroups._elem.insertAsLast(tg);
                //end
                if(telephonegroupElem == root->LastChildElement("TelephoneGroup"))
                    break;
            }
        }
    }
    return true;
}

bool IOHelper::saveTelephoneGroups(const TelephoneGroups &telephonegroups) const   //保存电话分组
{
    XMLDocument *doc = new XMLDocument();
    XMLNode *root = doc->InsertFirstChild(doc->NewElement("TelephoneGroups"));
    ListNodePosi(TelephoneGroup) it = telephonegroups._elem.first();
    for(int i = 0; i < telephonegroups.count(); ++i, it = it->succ)
    {
        XMLElement *telephonegroupElem = doc->NewElement("TelephoneGroup");
        telephonegroupElem->SetAttribute("id", i);
        XMLNode *telephonegroupNode = root->InsertEndChild(telephonegroupElem);
        //name
        XMLNode *nameNode = telephonegroupNode->InsertEndChild(doc->NewElement("name"));
        nameNode->InsertEndChild(doc->NewText(it->data.name.c_str()));
    }
    //Write out xml file
    if(doc->SaveFile(this->telephonegroupsFileName.c_str()) == XMLError::XML_SUCCESS)
        return true;
    else
        return false;
}
/*EmailGroups*/
bool IOHelper::readEmailGroups(EmailGroups &emailgroups) //读入email分组
{
    XMLDocument *doc = new XMLDocument;
    if(doc->LoadFile(this->emailgroupsFileName.c_str()) != XMLError::XML_SUCCESS)
        return false;
    if(!doc->NoChildren())
    {
        XMLNode *root = doc->FirstChild();
        if(!root->NoChildren())
        {
            int i = 0;
            for(XMLElement *emailgroupElem = root->FirstChildElement("EmailGroup");;++i, emailgroupElem = emailgroupElem->NextSiblingElement("EmailGroup"))
            {
                EmailGroup eg;
                //id
                eg.id = emailgroupElem->IntAttribute("id");
                emailgroups.maxID = eg.id > emailgroups.maxID ? eg.id : emailgroups.maxID;
                //name
                eg.name = getXMLElementTextSafe(emailgroupElem->FirstChildElement("name"));
                //push
                emailgroups._elem.insertAsLast(eg);
                //end
                if(emailgroupElem == root->LastChildElement("EmailGroup"))
                    break;
            }
        }
    }
    return true;
}

bool IOHelper::saveEmailGroups(const EmailGroups &emailgroups) const //保存email分组
{
    XMLDocument *doc = new XMLDocument();
    XMLNode *root = doc->InsertFirstChild(doc->NewElement("EmailGroups"));
    ListNodePosi(EmailGroup) it = emailgroups._elem.first();
    for(int i = 0; i < emailgroups.count(); ++i, it = it->succ)
    {
        XMLElement *emailgroupElem = doc->NewElement("EmailGroup");
        emailgroupElem->SetAttribute("id", i);
        XMLNode *emailgroupNode = root->InsertEndChild(emailgroupElem);
        //name
        XMLNode *nameNode = emailgroupNode->InsertEndChild(doc->NewElement("name"));
        nameNode->InsertEndChild(doc->NewText(it->data.name.c_str()));
    }
    //Write out xml file
    if(doc->SaveFile(this->emailgroupsFileName.c_str()) == XMLError::XML_SUCCESS)
        return true;
    else
        return false;
}
