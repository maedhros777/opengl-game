#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdio>
#include <map>
#include "Singleton.h"
#include "Error.h"
#include "Utils.h"

class Settings : public Singleton<Settings>
{
private:
    std::map<std::string, std::string> str_map;
    std::map<std::string, int> int_map;
    std::map<std::string, float> float_map;
public:
    ERR_RET load(const std::string &filename);
    std::string getStr(const std::string &name);
    int getInt(const std::string &name);
    float getFloat(const std::string &name);

    void set(const std::string &setting, const std::string &str)
    {
        str_map[setting] = str;
    }

    void set(const std::string &setting, int val)
    {
        int_map[setting] = val;
    }

    void set(const std::string &setting, float val)
    {
        float_map[setting] = val;
    }
};

#define settings Settings::getInstance()
#define settingsPtr Settings::getInstancePtr()

#endif
