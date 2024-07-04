#pragma once
#include <windows.h>
#include <psapi.h>
#include <iostream>
using namespace std;
class RAM
{
public:
    unsigned int totalRamInMb;
    unsigned int freeRamInMb;
    unsigned int usedRamInMb;

    void UpdateValues();
};

