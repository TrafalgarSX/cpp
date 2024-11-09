#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <string>

#if defined(DLL_EXPORTS)
#define MYFUNC_API __declspec(dllexport)
#else
#define MYFUNC_API __declspec(dllimport)
#endif

MYFUNC_API void initApp();

int main(int argc, const char** argv) {
    // load dll
#if 0
    HMODULE hModule = LoadLibrary("websocketServerDll.dll");
    if (hModule == NULL)
    {
        std::cout << "LoadLibrary failed" << std::endl;
        return 0;
    }
#endif

    std::thread t(initApp);
    std::cout << "DLL_PROCESS_DETACH" << std::endl;
    std::cout << "websocket server is running" << std::endl;
    std::cout << "test test" << std::endl;

    for (int i = 0; i< 100; ++i) {
        std::cout << "Hello, world!\n" ; 
       // sleep
       std::this_thread::sleep_for(std::chrono::seconds(5)); // sleep for 1 second 
    }
    t.join();

    return 0;
}