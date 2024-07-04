#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
using namespace std;
class RAM
{
public:

    static DWORDLONG GetTotalMemory();
    static DWORDLONG GetFreeMemory();
    static void PrintMemoryInfo(DWORD processID);

    DWORDLONG TotalPhysicalMemory;
    DWORDLONG FreePhysicalMemory;
    DWORDLONG UsedPhysicalMemory;
};

