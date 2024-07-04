#pragma once
#include <string>
#include <chrono>
#include <iostream>
class CPU
{
public:
    double load = 0;
    double frecuencyInMHz = 0;
    void UpdateValues(std::chrono::duration<double> elapsedTime);
};

