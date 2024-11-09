#pragma once

#include <iostream>

class Foo : public std::enable_shared_from_this<Foo> {
public:
    std::shared_ptr<Foo> getptr() {
        return shared_from_this();
    }

    void bar() {
        std::cout << "Foo::bar\n";
    }

    // 工厂函数
    static std::shared_ptr<Foo> create() {
        return std::shared_ptr<Foo>(new Foo);
    }
    Foo() = default;

private:
    // 私有化构造函数
};