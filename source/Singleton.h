#ifndef SINGLETON_H
#define SINGLETON_H
#include "Debug.h"
#include <cstdlib>

template <class DERIVED>
class Singleton //Credit for this class goes to "Game Programming Gems", section 1.3 by Scott Bilas
{
private:
    static DERIVED *instance;
public:
    Singleton()
    {
        ASSERT(!instance);
        int offset = (size_t)(DERIVED *)1 - (size_t)(Singleton<DERIVED> *)(DERIVED *)1;
        instance = (DERIVED *)((size_t)this + offset);
    }

    virtual ~Singleton()
    {
        ASSERT(instance);
        instance = 0;
    }

    static DERIVED &getInstance()
    {
        ASSERT(instance);
        return *instance;
    }

    static DERIVED *getInstancePtr()
    {
        ASSERT(instance);
        return instance;
    }

    static bool valid()
    {
        return (instance != 0);
    }
};

template <class DERIVED>
DERIVED *Singleton<DERIVED>::instance = 0;

#endif
