#include <iostream>
#include <exportvec.h>
#include <staticlib.h>

#if 1
void* operator new(std::size_t size)
{
    std::cout << "alloc in main program" << size << std::endl;
    if (void* mem = std::malloc(size)) 
        return mem;
    else
        throw std::bad_alloc(); // 如果分配失败，抛出 std::bad_alloc 异常
}

void operator delete(void* mem) noexcept
{
    std::cout << "free in main program" << std::endl;
    std::free(mem);
}
#endif

void test_usevec()
{
    std::vector<int> vec1 = createvec();

    for(auto i : vec1)
    {
        std::cout << i << "\t";
    }
    std::cout << std::endl;
}

void test_usevec2()
{
    std::vector<int> vec2 = createvec2();

    for(auto i : vec2)
    {
        std::cout << i << "\t";
    }
    std::cout << std::endl;
}

void test_changevec()
{
    std::vector<int> vec3;

    // vec3.reserve(3);  // 提前分配空间， 就不会在 DLL 中分配空间，从而不会出错

    changevec(vec3);
    for(auto i : vec3)
    {
        std::cout << i << "\t";
    }
    std::cout << std::endl;
}

namespace mainexe {
    void test_new();

}

void test_program_new()
{
    int* p = new int(10);
    delete p;

    mainexe::test_new();


    std::cout << "------------" << std::endl;

    test_new();

    std::cout << "------------" << std::endl;
    staticlib_new();
}


namespace samename {
    extern int i, j;
    extern int x;

}

void test_program_namespace()
{

    std::cout << samename::i << samename::j << std::endl;

    std::cout << samename::x << std::endl;

}

int main()
{
    test_program_namespace();

    // test_usevec();
    // test_usevec2();
    // test_changevec();
    std::cout << "end" << std::endl;

    return 0;
}

// 两个问题：
/*
1. 重定义 全局的 operator new  会影响所有的代码吗， 比如动态库重定义了全局的 operator new
会影响到使用动态库的代码吗？ 会影响其他动态库吗？   静态库呢？  如果是主程序重定义了，那他的影响
范围是什么？

一个模块定义了 全居的 operator new 会影响一个模块内的， 但是不会影响其他模块， 也不会影响主程序

额外问题：

不同动态库中的 同名namespace 会合并吗？

2. /MT /MD 的区别， 为什么如果使用 /MT 编译dll ，会让每个动态库有自己的堆？  一个程序每个动态库
都有自己的堆吗？  done

*/