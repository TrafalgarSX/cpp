
#ifndef _DRIVER_LOADER_H_
#define _DRIVER_LOADER_H_
#include <Windows.h>
#include <stdio.h>


int Create_Service(const char* ServiceName, const char* ExecutablePath);
int Start_Service(const char* ServiceName);
int Stop_Service(const char* ServiceName);
int Delete_Service(const char* ServiceName);
HANDLE Get_IO_Handle(char* DeviceName);
void Close_IO_Handle(HANDLE hIO);
int Write_IO_Handle(HANDLE hDevice, const char* Buffer, const int WriteSize);
#endif // _DRIVER_LOADER_H_
