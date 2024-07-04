#include "Application.h"
#include "imgui.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <psapi.h>
#include <string>
#include "RAM.h"
#include "CPU.h"
#include "GPU.h"

#include <thread>
#include <chrono>

namespace ResourcesManager {

    using namespace std;

    void DisplayDataInConsole();
    void ShowAllProcessMemory();
    void ShowGeneralTab();
    string ConvertToMB(DWORDLONG bytes);

    bool show_demo_window;
    bool show_another_window;
    float clear_color;

    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

    double cpuUsage = 0;
    double memoryUsage = 0;
    double gpuUsage = 0;
    double cpuVel = 0;



    static int time = 1000;

    DWORDLONG totalMemory;
    DWORDLONG freeMemory;

    string totalMemorystring;
    string freeMemorystring;

    RAM ram;
    CPU cpu;

    void RenderUI() {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ShowGeneralTab();

        ImGui::ShowDemoWindow();
    }

    string ConvertToMB(DWORDLONG bytes)
    {
        double mb = static_cast<double>(bytes) / (1024.0);
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%.2f MB", mb);
        return std::string(buffer);
    }

    void ShowGeneralTab() {
        
        ImGui::Begin("General");

        ImGui::Text("Resource Manager");


        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = now - lastUpdateTime;


        if (elapsedSeconds.count() >= time / static_cast<double>(1000)) {
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsedTime = end - now;

            ram.UpdateValues();
            cpu.UpdateValues(elapsedTime);

            cpuUsage = cpu.load;
            cpuVel = cpu.frecuencyInMHz;
            gpuUsage = GPU::GetGPUUsage();

            memoryUsage = static_cast<double>(ram.totalRamInMb - ram.freeRamInMb) / ram.totalRamInMb;

            lastUpdateTime = now;
        }



        // Sección de Consumo
        if (ImGui::BeginTable("Resource Consumption", 3))
        {
            ImGui::TableNextColumn();
            ImGui::Text("RAM");
            ImGui::ProgressBar(memoryUsage, ImVec2(0.0f, 0.0f)); // Consumo RAM
            ImGui::Text("%.2f%%", memoryUsage * 100);

            ImGui::TableNextColumn();
            ImGui::Text("CPU");
            ImGui::ProgressBar(cpuUsage, ImVec2(0.0f, 0.0f)); // Consumo CPU
            ImGui::Text("%.2f%%", cpuUsage * 100);


            ImGui::TableNextColumn();


            ImGui::Text("GPU");
            ImGui::ProgressBar(gpuUsage, ImVec2(0.0f, 0.0f)); // Consumo GPU
            ImGui::Text("%.2f%%", gpuUsage * 100);

            ImGui::EndTable();
        }


        // Sección de Valores Totales
        if (ImGui::BeginTable("Total Values", 3))
        {
            ImGui::TableNextColumn();
            string totalMemoryStr = "Total RAM: " +  std::to_string(ram.totalRamInMb) + " MB";
            ImGui::Text(totalMemoryStr.c_str());
            string freeMemoryStr = "Free RAM: " + std::to_string(ram.freeRamInMb) + " MB";
            ImGui::Text(freeMemoryStr.c_str());
            string usedMemoryStr = "Used RAM: " + std::to_string(ram.usedRamInMb) + " MB";
            ImGui::Text(usedMemoryStr.c_str());

            ImGui::TableNextColumn();

            string cpuVelString = "Velocity: " + std::to_string(int(cpuVel)) + " MHz";
            ImGui::Text(cpuVelString.c_str());

            ImGui::TableNextColumn();

            DXGI_ADAPTER_DESC gpuInfo = GPU::GetGPUInfo();

            string vram = "VRAM: " + std::to_string(gpuInfo.DedicatedVideoMemory / 1024 / 1024) + " MB";
            ImGui::Text(vram.c_str());

            ImGui::EndTable();
        }

        ImGui::InputInt("Update time in ms", &time);


        ImGui::End();

    }





    void ShowAllProcessMemory() {
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        unsigned int i;

        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        {
            return;
        }

        // Calculate how many process identifiers were returned.

        cProcesses = cbNeeded / sizeof(DWORD);

        // Print the memory usage for each process

        for (i = 0; i < cProcesses; i++)
        {
            //RAM::PrintMemoryInfo(aProcesses[i]);
        }

    }

}
