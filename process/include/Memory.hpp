#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>
#include <iostream>
#include <vector>
#pragma comment(lib, "psapi")

class Memory
{
public:
    std::vector<int> GetProcessId(const char* processName);
    int GetModuleBase(HANDLE processHandle, std::string &sModuleName);
    BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
    BOOL GetDebugPrivileges(void);
    int ReadInt(HANDLE processHandle, long long address);
    int GetPointerAddress(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);
    int ReadPointerInt(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);
    float ReadFloat(HANDLE processHandle, int address);
    float ReadPointerFloat(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);
    char* ReadText(HANDLE processHandle, long long address);
    char* ReadPointerText(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);

    bool WriteInt(HANDLE processHandle, long long address, int value);
    bool WriteText(HANDLE processHandle, long long address, const char* value);
};