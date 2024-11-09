#include <iostream>
#include <exportvec.h>

MY_DLL_API void test_new()
{
    std::cout << "test_new" << std::endl;
    int* p = new int(10);
    delete p;
}


namespace samename{
    int i = 7;
    int j = 8;
    int x = 10;
}