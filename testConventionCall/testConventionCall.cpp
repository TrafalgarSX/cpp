#include <iostream>

uint32_t __stdcall test_func(void* first, uint32_t second) {
    return 0;
}

uint32_t test_func_cdecl(void* first, uint32_t second)
{
    return 0;
}

int main()
{
    std::cout << typeid(test_func).name() << "\n";
    std::cout << typeid(test_func_cdecl).name() << "\n";
}