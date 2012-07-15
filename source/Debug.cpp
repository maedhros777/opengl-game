#include "Debug.h"
#include "Logger.h"
#include <iostream>
#include <cstdlib>

void __assert(bool assertion, const char *text, int line, const char *file)
{
    if (!assertion)
    {
        char msg[100];
        sprintf(msg, "An assertion failed in \"%s\" on line %d: %s\n", file, line, text);
        logger.log(LOG_FILE | LOG_USER, msg);
        exit(1);
    }
}

void __assert(bool assertion, const char *text, const char *description, int line, const char *file)
{
    if (!assertion)
    {
        char msg[100];
        sprintf(msg, "An assertion failed in \"%s\" on line %d: %s\n\nDescription: %s", file, line, text, description);
        logger.log(LOG_FILE | LOG_USER, msg);
        exit(1);
    }
}

void __assert(const char *description, int line, const char *file)
{
    char msg[100];
    sprintf(msg, "An assertion failed in \"%s\" on line %d.\n\nDescription: %s", file, line, description);
    logger.log(LOG_FILE | LOG_USER, msg);
    exit(1);
}
