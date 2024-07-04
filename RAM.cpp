#include "RAM.h"


#define DIV 1024

#define WIDTH 7



static MEMORYSTATUSEX GetMemoryInfo() {
    MEMORYSTATUSEX statex{};
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    return statex;
}

DWORDLONG RAM::GetTotalMemory()
{
    MEMORYSTATUSEX statex = GetMemoryInfo();

    return statex.ullTotalPhys / DIV;
}

DWORDLONG RAM::GetFreeMemory() {

    MEMORYSTATUSEX statex = GetMemoryInfo();

    return statex.ullAvailPhys / DIV;
}

void RAM::PrintMemoryInfo(DWORD processID)
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;
    std::string name = "null";

    // Print information about the memory usage of the process.

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);
    if (NULL == hProcess)
        return;

    DWORD buffSize = 1024;
    CHAR buffer[1024];
    if (QueryFullProcessImageNameA(hProcess, 0, buffer, &buffSize))
    {
        // Convert buffer to std::string and find the last token.
        string fullPath(buffer);
        size_t pos = fullPath.find_last_of("/\\");
        if (pos != string::npos)
        {
            name = fullPath.substr(pos + 1);
        }
        else
        {
            name = fullPath;
        }

    }
    else
    {
    }

    int size = 0;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
    {
        size = pmc.WorkingSetSize / 1024;
    }

    cout << processID << " " << name << " " << size << "kb" << endl;

    CloseHandle(hProcess);
}
