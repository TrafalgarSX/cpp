#include <iostream>
#include <fmt/core.h>

int main(int argc, const char** argv) {
    fmt::print("Hello, World!\n");

    if(int a = 5; a > 0)
    {
        fmt::print("a is greater than 0\n");
    }
    else
    {
        fmt::print("a is less than 0\n");
    }
    return 0;
}