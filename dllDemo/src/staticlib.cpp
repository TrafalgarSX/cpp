
#include <iostream>


void staticlib_new()
{
    std::cout << "alloc in staticlib" << std::endl;
    int* p = new int(10);
    delete p;
}

namespace samename{
    int i = 4;
}
