#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <iostream>

class GPU
{
public:
    DXGI_ADAPTER_DESC GetGPUInfo();
};

