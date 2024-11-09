#include <iostream>
#include <fmt/core.h>
#include "staticlib.h"
#include "dll.h"
#include "testsingle.h"

int main(int argc, const char** argv) {
    myClass.m_name = __FILE__;;

    MyClass other;

    printAddress();

    fmt::print("m_count: {}\n", MyClass::m_count);
    // myclass address
    fmt::print("myClass address = {}\n",  static_cast<void*>(&myClass));
    fmt::print("myClass.m_name = {}\n", myClass.m_name);

    single &s = single::get_instance();
    fmt::print("single address = {}\n",  static_cast<void*>(&s));


    print_single_address();


    return 0;
}