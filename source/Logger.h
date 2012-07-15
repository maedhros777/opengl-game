#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include "Debug.h"
#include "Singleton.h"
#include "Utils.h"
#include <iostream>

const unsigned int LOG_FILE = 1;
const unsigned int LOG_USER = 2;
const std::string LOG_FILE_NAME = "gameengine.txt";

class Logger : public Singleton<Logger>
{
private:
    FILE *file;
public:
    void log(unsigned char, const std::string &) const;
    void logln(unsigned char flags, const std::string &msg) const
    {
        log(flags, msg + "\n");
    }

    Logger()
    {
        file = file_open(LOG_FILE_NAME, "a");
    }

    ~Logger()
    {
        fclose(file);
    }
};

#define logger Logger::getInstance()
#define loggerPtr Logger::getInstancePtr()

#endif
