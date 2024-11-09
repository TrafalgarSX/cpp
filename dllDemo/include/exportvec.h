#pragma once

#include <vector>
#include <iostream>

#ifdef EXPORTING_MY_DLL
#define MY_DLL_API __declspec(dllexport)
#else
#define MY_DLL_API __declspec(dllimport)
#endif


MY_DLL_API std::vector<int>  createvec();

MY_DLL_API std::vector<int> createvec2();

MY_DLL_API void changevec(std::vector<int>& test);

MY_DLL_API void test_new();

#if 1
namespace samename{

}
#endif