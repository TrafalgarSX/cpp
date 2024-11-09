
#include "dll.h"
#include "staticlib.h"

#include <fmt/core.h>

void changevec(std::vector<int> &vec)
{
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
}

void printAddress()
{
    myClass.m_name = __FILE__;
    fmt::print("m_count: {}\n", MyClass::m_count);
    fmt::print("myClass address = {}\n",  static_cast<void*>(&myClass));
    fmt::print("myClass.m_name = {}\n", myClass.m_name);

    single &s = single::get_instance();
    fmt::print("single address = {}\n",  static_cast<void*>(&s));
}