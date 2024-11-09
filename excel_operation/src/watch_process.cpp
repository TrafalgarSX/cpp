#include <windows.h>
#include <iostream>
#include <string>

int main() {
    std::string exe = "test_xlsreader.exe";

    LPSTR lpCmdLine = const_cast<LPSTR>(exe.c_str());

    // 启动进程
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcess(NULL, lpCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        // 等待进程结束
        WaitForSingleObject(pi.hProcess, INFINITE);

        // 获取退出代码
        DWORD exitCode;
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            std::cout << "Process exited with code: " << exitCode << std::endl;
        } else {
            std::cerr << "Failed to get exit code" << std::endl;
        }

        // 关闭句柄
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::cerr << "Failed to create process" << std::endl;
    }

    return 0;
}