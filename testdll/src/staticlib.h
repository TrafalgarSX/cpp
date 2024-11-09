#ifndef _STATICLIB_H_
#define _STATICLIB_H_

#include <string>

class MyClass
{
public:
    MyClass();
    MyClass(MyClass &&) = default;
    MyClass(const MyClass &) = default;
    MyClass &operator=(MyClass &&) = default;
    MyClass &operator=(const MyClass &) = default;
    ~MyClass();

public:
    std::string m_name;
    static int m_count;
};

extern MyClass myClass;

// #define IN_HEADER

#ifdef IN_HEADER
class single{
public:
    static single &get_instance()
    {
        static single instance;
        return instance;
    }
private:
    single() = default;
    single(const single &) = delete;
    single(single &&) = delete;
    single &operator=(const single &) = delete;
    single &operator=(single &&) = delete;
};

#else  

class single{
public:
    static single &get_instance();

private:
    single() = default;
    single(const single &) = delete;
    single(single &&) = delete;
    single &operator=(const single &) = delete;
    single &operator=(single &&) = delete;
};

#endif

inline int testfunc()
{
    return 0;
}

#endif //_STATICLIB_H_