#pragma once
#include <string>
class CPU
{
public:
    static float GetCPULoad();
    static std::string GetInfo();
    static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
};

