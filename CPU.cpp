#include <Windows.h>
#include <intrin.h>
#include "CPU.h"

float GetCPULoad();
long long GetTicks();
std::string GetInfo();
float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
float GetFrecuency(std::chrono::duration<double> elapsedTime);
long long firstTick = 0;
float lastCpuVel = 0;

void CPU::UpdateValues(std::chrono::duration<double> elapsedTime) {
    frecuencyInMHz = GetFrecuency(elapsedTime);
    load = GetCPULoad();
}

float GetFrecuency(std::chrono::duration<double> elapsedTime) {

    if (firstTick == 0) {
        firstTick = GetTicks();
    }
    else {

        float cpuVel = ((GetTicks() - firstTick) / elapsedTime.count()) / 1e12;

        lastCpuVel = cpuVel;

        firstTick = 0;

        return cpuVel;
    }

    return lastCpuVel;
}



static unsigned long long FileTimeToInt64(const FILETIME& ft) { return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime); }



float GetCPULoad()
{
    FILETIME idleTime, kernelTime, userTime;
    return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}

std::string GetInfo()
{
    int CPUInfo[4] = { -1 };
    unsigned   nExIds, i = 0;
    char CPUBrandString[0x64];
    // Get the information associated with each extended ID.
    __cpuid(CPUInfo, 0x80000004);

    memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo)); //Clock speed

    return CPUBrandString;
}



float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;

    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;


    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

    _previousTotalTicks = totalTicks;
    _previousIdleTicks = idleTicks;
    return ret;
}

long long GetTicks() {
    long long i = __rdtsc();
    return i;
}


