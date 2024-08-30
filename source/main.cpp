//
// Created by Vlad on 26.08.2024.
//
#include <thread>
#include <Windows.h>
#include <gl/GL.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <MinHook.h>


LPVOID oWglSwapBuffers = nullptr;
LONG_PTR oWndProc;
bool imGuiInitialized = false;

LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);


    typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
    return CallWindowProc((WNDPROC)oWndProc, hWnd, uMsg, wParam, lParam);
}


BOOL __stdcall HookedSwapBuffers(const HDC hdc)
{
    if (!imGuiInitialized)
    {
        oWndProc = SetWindowLongPtr(WindowFromDC(hdc), GWLP_WNDPROC, (LONG_PTR)WndProc);

        ImGui::CreateContext();
        ImGui_ImplWin32_Init(WindowFromDC(hdc));
        ImGui_ImplOpenGL3_Init();
        ImGui::StyleColorsDark();

        imGuiInitialized = true;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::GetBackgroundDrawList()->AddText({}, ImColor(255, 255, 255), "Hello!");

    ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    typedef BOOL(__stdcall* WglSwapBuffers_t)(HDC);
    return reinterpret_cast<WglSwapBuffers_t>(oWglSwapBuffers)(hdc);
}

namespace seville
{
    void MainThread(HMODULE dllHandle)
    {
        MH_Initialize();
        MessageBox(0, "Inkected", "x", 0);
        MH_CreateHook(SwapBuffers, HookedSwapBuffers, &oWglSwapBuffers);
        MH_EnableHook(MH_ALL_HOOKS);
    }
}

// ReSharper disable once CppDFAConstantFunctionResult
extern "C" BOOL WINAPI DllMain(const HMODULE dllHandle, const DWORD callReason, LPVOID)
{
    if (callReason != DLL_PROCESS_ATTACH)
        return false;

    DisableThreadLibraryCalls(dllHandle);
    std::thread(seville::MainThread, dllHandle).detach();

    return true;
}
