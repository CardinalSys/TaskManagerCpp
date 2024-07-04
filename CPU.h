#pragma once
#include <string>
#include <chrono>
#include <iostream>
class CPU
{
public:
    static float GetCPULoad();
    static long long GetTicks();
    static std::string GetInfo();
    static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
};

