#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <string>

#include <chrono>
#include <iostream>
#include <regex>
#include <vector>

#include <pdh.h>
#include <pdhmsg.h>
#include <strsafe.h>
#include <tchar.h>

#pragma comment(lib, "pdh.lib")

class GPU
{
public:
    static DXGI_ADAPTER_DESC GetGPUInfo();
    static double GetGPUUsage();
};



