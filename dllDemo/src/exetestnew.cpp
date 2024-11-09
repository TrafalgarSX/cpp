
#include <iostream>


namespace mainexe 
{
    void test_new()
    {
        std::cout << "test_new" << std::endl;
        int* p = new int(10);
        delete p;
    }
    int i;
}

namespace samename{
    // int i;
    int j = 5;
}