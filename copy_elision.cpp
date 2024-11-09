#include <iostream>

int n = 0;

struct C {
  explicit C(int) 
  {
    std::cout << "C(int) called" << std::endl;
  }
  C(const C&) // the copy constructor has a visible side effect
  { 
    ++n; 
    std::cout << "C(const C&) called" << std::endl;
  }  
}; // it modifies an object with static storage duration                      

C fun()
{
  return C(42);
}

int main() {
  C c1(42);      // direct-initialization, calls C::C(int)
  C c2 = C(42);  // copy-initialization, calls C::C(const C&)

  C c3 = fun();  // copy-initialization, calls C::C(const C&)

  std::cout << n << std::endl;  // prints 0 if the copy was elided, 1 otherwise
}
