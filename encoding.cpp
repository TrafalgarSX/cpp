#include <iostream>

int main()
{
  const char * gbk = "\xce\xd2\xca\xc7\xba\xba\xd7\xd6\xc6\xe6";  // GBK
  const char * utf8 = "\xe6\x88\x91\xe6\x98\xaf\xe6\xb1\x89\xe5\xad\x97\xe5\xa5\x87"; // UTF-8

  std::cout << "GBK: " << gbk << std::endl;
  std::cout << "UTF-8: " << utf8 << std::endl;

  return 0;
}
