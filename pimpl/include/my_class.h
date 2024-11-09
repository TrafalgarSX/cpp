#ifndef _MY_CLASS_
#define _MY_CLASS_

#include <string>
#include <memory>


class User{
public:
    User();
    ~User();

    User(const User&);
    User& operator=(const User&);

    User(User&&) noexcept;
    User& operator=(User&&) noexcept;

    void doSomething();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};


#endif