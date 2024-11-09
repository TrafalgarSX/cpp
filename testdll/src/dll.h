#ifndef _DLL_H_
#define _DLL_H_

#include <vector>

__declspec(dllexport) void changevec(std::vector<int> &vec);

__declspec(dllexport) void printAddress();

#endif // _DLL_H_