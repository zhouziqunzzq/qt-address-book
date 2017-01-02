#ifndef SETTINGS_H
#define SETTINGS_H
#include <string>

class Settings  //程序设置类
{
public:
    Settings();
    friend class IOHelper;
    friend class MainWindow;
    friend class SettingsDialog;
private:
    std::string username;
    std::string password;
};

#endif // SETTINGS_H
