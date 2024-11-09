#include <iostream>

#if 1
#include <winsock2.h>
#include <ws2tcpip.h>
#else 
#define WIN32_LEAN_AND_MEAN  // Say this...

#include <windows.h>         // And now we can include that.
#include <winsock2.h>        // And this.
#endif

int main(int argc, const char** argv) {
    return 0;
}