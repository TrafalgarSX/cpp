#include <my_class.h>

#include <memory>
#include <string>
#include <iostream>

#if 1
class User::Impl{
public:
    Impl() = default;
    ~Impl() = default;
    void doSomething(){
        std::cout << "Doing something" << std::endl;
    }
private:
};

#endif

User::User() : pImpl(std::make_unique<Impl>()){

}

User::~User() = default;

User::User(const User& other) : pImpl(std::make_unique<Impl>(*other.pImpl)){

}

User& User::operator=(const User& other){
    if (this != &other) {
        pImpl = std::make_unique<Impl>(*other.pImpl);
    }
    return *this;
}

// 这里的移动操作其实  =default就行， 因为unique_ptr 本身就支持移动操作
#if 1
User::User(User&& other) noexcept : pImpl(std::move(other.pImpl)){

}
#else
User::User(User&& other) noexcept : pImpl(other.pImpl){ // error 
   // unique_ptr has no copy constructor
   // other 的类型是 右值引用， 但是 other 本身是左值(有名字的变量)，所以不能直接move 
}
#endif

User& User::operator=(User&& other) noexcept{
    pImpl = std::move(other.pImpl);
    return *this;
}

void User::doSomething(){
    pImpl->doSomething();
}
