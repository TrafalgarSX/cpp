#include "staticlib.h"
#include <fmt/core.h>
#include <iostream>


MyClass::MyClass() {
    fmt::print("MyClass::MyClass()\n");
    m_count++;
}

MyClass::~MyClass() {
    fmt::print("MyClass::~MyClass()\n");
}

int MyClass::m_count = 0;

MyClass myClass;

#ifndef IN_HEADER

single &single::get_instance()
{
    static single instance;
    return instance;
}


#endif 