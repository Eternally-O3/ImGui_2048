// ImGui_2048.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "ImGui_2048.h"


HANDLE hMutex;
ImFont* font;
extern int Gameover_nFlage;   // 游戏结束状态
extern int RandNum_nFalge;    // 是否添加一个新的随机数标志
const char* GameInfo="";
extern bool GameStart;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    hMutex = CreateMutex(NULL, TRUE, "##ImGui_2048");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        // 已经有一个实例在运行，退出当前实例
        MessageBox(NULL, "请勿重复运行", "信息", MB_OK);
        CloseHandle(hMutex);
        return 0;
    }
    

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, hInstance, NULL, NULL, NULL, NULL, "ImGui_2048", NULL};
    //wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY)); // 设置大图标
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // 设置小图标
    RegisterClassEx(&wc);
    Window = CreateWindow(wc.lpszClassName, "ImGui_2048", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    ShowWindow(Window, SW_HIDE);
    UpdateWindow(Window);

    if (!CreateDeviceD3D(Window))
    {

        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }


    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;
    std::string FontPath = GetAppDataPath(CSIDL_FONTS);
    FontPath += "\\simhei.ttf";

    font = io.Fonts->AddFontFromFileTTF(FontPath.c_str(), 22.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    io.FontDefault = font;

    

    //io.Fonts->AddFontDefault();
    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplDX9_Init(g_pd3dDevice);
    ImGui_ImplWin32_Init(Window);

    
    Gameover_nFlage = 0; // 重置游戏状态
    StartDate(); // 初始化游戏

            MSG msg;
            memset(&msg, 0, sizeof(msg));
            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    continue;
                }


                // Start the Dear ImGui frame
                ImGui_ImplDX9_NewFrame();
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();

                
                ShowWindows(); // 显示游戏界面
                if(Gameover_nFlage<1&& GameStart) KeyboardInput(); // 处理按键输入
               

                CheckGameOver(); // 检查游戏是否结束

                if (Gameover_nFlage == 1) {
                    GameInfo = u8"游戏结束! 失败";
                }
                else if (Gameover_nFlage == 2) {
                    GameInfo = u8"游戏结束! 恭喜获胜";
                }

                if (RandNum_nFalge == 1) {
                    RandNum(); // 生成随机数
                }

                ImGui::EndFrame();



                g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
                if (g_pd3dDevice->BeginScene() == D3D_OK)
                {
                    ImGui::Render();
                    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
                    g_pd3dDevice->EndScene();
                }
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                }


                HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

                // Handle loss of D3D9 device
                if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
                    ResetDevice();
                }
                if (!Enabled) {
                    msg.message = WM_QUIT;
                }

                

                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 120));
            }
        

    


    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    std::this_thread::sleep_for(std::chrono::milliseconds::duration(100));
    DestroyWindow(Window);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;


}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return 0; 

    switch (message)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0; 
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0; 

    case WM_CREATE:
        // Make the window top
        SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return 0;

    }
    return DefWindowProc(hWnd, message, wParam, lParam);

}