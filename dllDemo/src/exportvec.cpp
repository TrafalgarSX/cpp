#include <iostream>
#include <vector>
#include <exportvec.h>

#if 1
void* operator new(std::size_t size)
{
    std::cout << "alloc in exportvec dll" << size << std::endl;
    if (void* mem = std::malloc(size)) 
        return mem;
    else
        throw std::bad_alloc(); // 如果分配失败，抛出 std::bad_alloc 异常
}

void operator delete(void* mem) noexcept
{
    std::cout << "free in exportvec dll" << std::endl;
    std::free(mem);
}
#endif

MY_DLL_API std::vector<int> createvec()
{
    test_new();
    std::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    return vec;
}


MY_DLL_API std::vector<int> createvec2()
{
    return {4,5,6};
}

MY_DLL_API void changevec(std::vector<int>& test)
{
    test.push_back(7);
    test.push_back(8);
    test.push_back(9);
}
