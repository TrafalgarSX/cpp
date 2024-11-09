#include "staticlib.h"
#include <fmt/core.h>
#include "testsingle.h"

void print_single_address()
{
    single &s = single::get_instance();
    fmt::print("testsignle single address = {}\n",  static_cast<void*>(&s));
}