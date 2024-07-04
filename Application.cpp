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

    std::chrono::time_point<std::chrono::steady_clock> lastUpdateSlowTime;

    double cpuUsage = 0;
    double memoryUsage = 0;
    double gpuUsage = 0;
    double cpuVel = 0;

    long long firstTick = 0;

    DWORDLONG totalMemory;
    DWORDLONG freeMemory;

    string totalMemorystring;
    string freeMemorystring;

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

        // Encabezado
        ImGui::Text("Resource Manager");




        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = now - lastUpdateSlowTime;
        //SlowTimer
        if (elapsedSeconds.count() >= 1) {
            auto end = std::chrono::steady_clock::now();
            cpuUsage = CPU::GetCPULoad();


            gpuUsage = GPU::GetGPUUsage();

            totalMemory = RAM::GetTotalMemory();
            freeMemory = RAM::GetFreeMemory();

            totalMemorystring = ConvertToMB(totalMemory);
            freeMemorystring = ConvertToMB(freeMemory);

            memoryUsage = static_cast<double>(totalMemory - freeMemory) / totalMemory;

            if (firstTick == 0) {
                firstTick = CPU::GetTicks();
            }
            else {

                std::chrono::duration<double> elapsedTime = end - now;
                cpuVel = ((CPU::GetTicks() - firstTick) / elapsedTime.count()) / 1e12;

                firstTick = 0;

            }

            lastUpdateSlowTime = now;
        }



        string gpuUsageString = std::to_string(gpuUsage * 100);

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
            string cpuUsageString = "Usage: " + std::to_string(cpuUsage * 100);
            ImGui::Text(cpuUsageString.c_str());
            string cpuVelString = "Velocity: " + std::to_string(cpuVel) + " MHz";
            ImGui::Text(cpuVelString.c_str());

            ImGui::TableNextColumn();


            ImGui::Text("GPU");
            ImGui::ProgressBar(gpuUsage, ImVec2(0.0f, 0.0f)); // Consumo GPU
            ImGui::Text(gpuUsageString.c_str());

            ImGui::EndTable();
        }


        // Sección de Valores Totales
        if (ImGui::BeginTable("Total Values", 3))
        {
            ImGui::TableNextColumn();
            string totalMemoryStr = "Total RAM: " + totalMemorystring;
            ImGui::Text(totalMemoryStr.c_str());
            string freeMemoryStr = "Free RAM: " + freeMemorystring;
            ImGui::Text(freeMemoryStr.c_str());

            ImGui::TableNextColumn();
            string cpuBrand = CPU::GetInfo();
            ImGui::Text(cpuBrand.c_str());


            ImGui::TableNextColumn();

            DXGI_ADAPTER_DESC gpuInfo = GPU::GetGPUInfo();

            string vram = "Video GPU memory: " + std::to_string(gpuInfo.DedicatedVideoMemory / 1024 / 1024) + " MB";
            ImGui::Text(vram.c_str());

            ImGui::EndTable();
        }

        ImGui::End();

    }



    void DisplayDataInConsole() {
        DWORDLONG totalMemory = RAM::GetTotalMemory();

        while (true) {
            DWORDLONG freeMemory = RAM::GetFreeMemory();

            cout << "Total memory: " << totalMemory << " KB" << endl;
            cout << "Free memory: " << freeMemory << " KB" << endl;
            cout << "Available memory: " << totalMemory - freeMemory << " KB" << endl;

            std::this_thread::sleep_for(std::chrono::seconds(1));
            system("CLS");
        }
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
            RAM::PrintMemoryInfo(aProcesses[i]);
        }

    }

}
