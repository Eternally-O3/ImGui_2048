#include "framework.h"

Timer gameTimer;

int Bound[SIZE][SIZE]; // 游戏地图
int RandNum_nFalge;    // 是否添加一个新的随机数标志
int Gameover_nFlage;   // 游戏结束状态
int Score;             // 游戏分数
bool GameStart=false;
bool keyPressed[256] = { false }; // 存储键盘状态

ULONGLONG NowRGB = 0;
int random_R = 255;
int random_G = 255;
int random_B = 255;

extern const char* GameInfo;

void RandomRGB()
{
    
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    
    std::uniform_int_distribution<> dis(0, 255);

    if (GetTickCount64() - NowRGB >= 3000)
    {
        random_R = dis(gen);
        random_G = dis(gen);
        random_B = dis(gen);
        NowRGB = GetTickCount64();
    }
}

// 数字移动实现
void MoveUP() {
    for (int i = 0; i < SIZE; i++) {
        for (int k = 0, j = 1; j < SIZE; j++) {
            if (Bound[i][j] > 0) {
                if (Bound[i][k] == Bound[i][j]) {
                    Score += Bound[i][k++] *= 2;
                    Bound[i][j] = 0;
                    RandNum_nFalge = 1;
                }
                else if (Bound[i][k] == 0) {
                    Bound[i][k] = Bound[i][j];
                    Bound[i][j] = 0;
                    RandNum_nFalge = 1;
                }
                else {
                    Bound[i][++k] = Bound[i][j];
                    if (k != j) {
                        Bound[i][j] = 0;
                        RandNum_nFalge = 1;
                    }
                }
            }
        }
    }
}

void MoveDOWN() {
    for (int i = 0; i < SIZE; i++) {
        for (int k = SIZE - 1, j = SIZE - 2; j >= 0; j--) {
            if (Bound[i][j] > 0) {
                if (Bound[i][k] == Bound[i][j]) {
                    Score += Bound[i][k--] *= 2;
                    Bound[i][j] = 0;
                    RandNum_nFalge = 1;
                }
                else if (Bound[i][k] == 0) {
                    Bound[i][k] = Bound[i][j];
                    Bound[i][j] = 0;
                    RandNum_nFalge = 1;
                }
                else {
                    Bound[i][--k] = Bound[i][j];
                    if (k != j) {
                        Bound[i][j] = 0;
                        RandNum_nFalge = 1;
                    }
                }
            }
        }
    }
}

void MoveLEFT() {
    for (int i = 0; i < SIZE; i++) {
        for (int k = 0, j = 1; j < SIZE; j++) {
            if (Bound[j][i] > 0) {
                if (Bound[j][i] == Bound[k][i]) {
                    Score += Bound[k++][i] *= 2;
                    Bound[j][i] = 0;
                    RandNum_nFalge = 1;
                }
                else if (Bound[k][i] == 0) {
                    Bound[k][i] = Bound[j][i];
                    Bound[j][i] = 0;
                    RandNum_nFalge = 1;
                }
                else {
                    Bound[++k][i] = Bound[j][i];
                    if (k != j) {
                        Bound[j][i] = 0;
                        RandNum_nFalge = 1;
                    }
                }
            }
        }
    }
}

void MoveRIGHT() {
    for (int i = 0; i < SIZE; i++) {
        for (int k = SIZE - 1, j = SIZE - 2; j >= 0; j--) {
            if (Bound[j][i] > 0) {
                if (Bound[j][i] == Bound[k][i]) {
                    Score += Bound[k--][i] *= 2;
                    Bound[j][i] = 0;
                    RandNum_nFalge = 1;
                }
                else if (Bound[k][i] == 0) {
                    Bound[k][i] = Bound[j][i];
                    Bound[j][i] = 0;
                    RandNum_nFalge = 1;
                }
                else {
                    Bound[--k][i] = Bound[j][i];
                    if (k != j) {
                        Bound[j][i] = 0;
                        RandNum_nFalge = 1;
                    }
                }
            }
        }
    }
}


int nCountNullNum()
{
    int n = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (0 == Bound[i][j])
                n++;
        }
    }

    return n;
}

// 随机数生成
void RandNum() {
    int emptyCount = nCountNullNum();
    if (emptyCount == 0) return; // 如果没有空格，直接返回

    // 随机选择一个空格
    srand((unsigned int)time(NULL));
    int n = rand() % emptyCount;

    // 遍历数组，找到第n个空格
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Bound[i][j] == 0) { // 找到空格
                if (n == 0) { // 找到第n个空格
                    Bound[i][j] = (rand() % 2 == 0) ? 2 : 4; // 随机生成2或4
                    RandNum_nFalge = 0; // 生成完成后重置标志
                    return; // 生成完毕，返回
                }
                n--; // 减少n，继续寻找下一个空格
            }
        }
    }
}

// 检查游戏结束
void CheckGameOver() {
    bool hasEmptyCell = false;
    bool canMerge = false;

    // 检查是否有空格子
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (Bound[i][j] == 0) {
                hasEmptyCell = true; // 还有空格子，游戏没有结束
            }
            if (Bound[i][j] >= 2048) {
                Gameover_nFlage = 2; // 达到2048，游戏胜利
                return; // 直接返回，因为胜利条件已经满足
            }
        }
    }

    // 检查是否有可合并的相邻格子（横向和纵向）
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (Bound[i][j] == Bound[i][j + 1]) {
                canMerge = true; // 横向有可以合并的格子
            }
            if (Bound[j][i] == Bound[j + 1][i]) {
                canMerge = true; // 纵向有可以合并的格子
            }
        }
    }

    // 如果没有空格子且没有可合并的格子，游戏结束
    if (!hasEmptyCell && !canMerge) {
        Gameover_nFlage = 1; // 游戏失败
    }
}

// 游戏初始化
void StartDate() {
    /*
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            Bound[i][j] = 0;
        }
    }
    */
    RandNum();
    RandNum();
}

// 数字显示
void ShowNum(int i, int j) {
    if (Bound[i][j] != 0) {
        ImGui::Text("%5d", Bound[i][j]);
    }
    else {
        ImGui::Text("     ");
    }
}

// 游戏界面显示
void ShowWindows() {
    ImGuiStyle& Style = ImGui::GetStyle();
    auto Color = Style.Colors;
    Style.WindowMinSize = ImVec2(650, 520);
    ImVec2 displaySize = ImVec2((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN)); // 屏幕尺寸
    ImVec2 windowPos = ImVec2((displaySize.x - Style.WindowMinSize.x) * 0.5f, (displaySize.y - Style.WindowMinSize.y) * 0.5f); // 计算窗口位置
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
    Style.WindowBorderSize = 2;

    Style.ChildRounding = 0;
    Style.FrameRounding = 5;
    Style.ScrollbarRounding = 0;
    Style.GrabRounding = 1;
    Style.PopupRounding = 1;
    Style.WindowRounding = 1;

    Color[ImGuiCol_Text] = ImColor(245, 245, 245); // 文本颜色，白色
    Color[ImGuiCol_TextDisabled] = ImColor(130, 130, 130); // 禁用文本颜色
    Color[ImGuiCol_WindowBg] = ImColor(40, 40, 40); // 窗口背景颜色
    Color[ImGuiCol_ChildBg] = ImColor(40, 40, 40); // 子窗口背景颜色
    Color[ImGuiCol_PopupBg] = ImColor(50, 50, 50); // 弹出窗口背景颜色
    Color[ImGuiCol_Border] = ImColor(90, 90, 90); // 边框颜色
    Color[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 0); // 边框阴影颜色
    Color[ImGuiCol_FrameBg] = ImColor(50, 50, 50); // 框架背景颜色
    Color[ImGuiCol_FrameBgHovered] = ImColor(70, 130, 180); // 悬停框架背景颜色
    Color[ImGuiCol_FrameBgActive] = ImColor(70, 130, 180, 100); // 激活框架背景颜色
    Color[ImGuiCol_TitleBg] = ImColor(30, 30, 30); // 标题背景颜色
    Color[ImGuiCol_TitleBgActive] = ImColor(135, 206, 250); // 激活标题背景颜色
    Color[ImGuiCol_TitleBgCollapsed] = ImColor(30, 30, 30); // 折叠标题背景颜色
    Color[ImGuiCol_MenuBarBg] = ImColor(50, 50, 50); // 菜单栏背景颜色
    Color[ImGuiCol_ScrollbarBg] = ImColor(40, 40, 40); // 滚动条背景颜色
    Color[ImGuiCol_ScrollbarGrab] = ImColor(80, 80, 80); // 滚动条抓取颜色
    Color[ImGuiCol_ScrollbarGrabHovered] = ImColor(135, 206, 250); // 滚动条抓取悬停颜色
    Color[ImGuiCol_ScrollbarGrabActive] = ImColor(135, 206, 250, 100); // 滚动条抓取激活颜色
    Color[ImGuiCol_CheckMark] = ImColor(135, 206, 250); // 复选标记颜色
    Color[ImGuiCol_SliderGrab] = ImColor(135, 206, 250); // 滑块抓取颜色
    Color[ImGuiCol_SliderGrabActive] = ImColor(135, 206, 250, 100); // 滑块抓取激活颜色
    Color[ImGuiCol_Button] = ImColor(100, 100, 100); // 按钮颜色
    Color[ImGuiCol_ButtonHovered] = ImColor(135, 206, 250); // 按钮悬停颜色
    Color[ImGuiCol_ButtonActive] = ImColor(135, 206, 250, 100); // 按钮激活颜色
    Color[ImGuiCol_Header] = ImColor(135, 206, 250); // 标题颜色
    Color[ImGuiCol_HeaderHovered] = ImColor(77, 200, 250); // 标题悬停颜色
    Color[ImGuiCol_HeaderActive] = ImColor(135, 206, 250, 100); // 标题激活颜色
    Color[ImGuiCol_Separator] = ImColor(90, 90, 90); // 分隔符颜色
    Color[ImGuiCol_SeparatorHovered] = ImColor(135, 206, 250); // 悬停分隔符颜色
    Color[ImGuiCol_SeparatorActive] = ImColor(135, 206, 250, 100); // 激活分隔符颜色
    Color[ImGuiCol_ResizeGrip] = ImColor(50, 50, 50); // 调整大小手柄颜色
    Color[ImGuiCol_ResizeGripHovered] = ImColor(135, 206, 250); // 悬停调整大小手柄颜色
    Color[ImGuiCol_ResizeGripActive] = ImColor(135, 206, 250, 100); // 激活调整大小手柄颜色
    Color[ImGuiCol_Tab] = ImColor(60, 60, 60); // 标签颜色
    Color[ImGuiCol_TabHovered] = ImColor(135, 206, 250); // 标签悬停颜色
    Color[ImGuiCol_TabActive] = ImColor(80, 80, 80); // 激活标签颜色
    Color[ImGuiCol_TabUnfocused] = ImColor(40, 40, 40); // 未聚焦标签颜色
    Color[ImGuiCol_TabUnfocusedActive] = ImColor(60, 60, 60); // 未聚焦激活标签颜色
    Color[ImGuiCol_DockingPreview] = ImColor(135, 206, 250, 89); // Docking预览颜色
    Color[ImGuiCol_DockingEmptyBg] = ImColor(30, 30, 30); // Docking空背景颜色
    Color[ImGuiCol_PlotLines] = ImColor(135, 206, 250); // 绘图线颜色
    Color[ImGuiCol_PlotLinesHovered] = ImColor(255, 110, 90); // 悬停绘图线颜色
    Color[ImGuiCol_PlotHistogram] = ImColor(135, 206, 250); // 绘图直方图颜色
    Color[ImGuiCol_PlotHistogramHovered] = ImColor(255, 110, 90); // 悬停绘图直方图颜色
    Color[ImGuiCol_TextSelectedBg] = ImColor(135, 206, 250, 100); // 文本选择背景颜色
    Color[ImGuiCol_DragDropTarget] = ImColor(135, 206, 250); // 拖放目标颜色
    Color[ImGuiCol_NavHighlight] = ImColor(135, 206, 250); // 导航高亮颜色
    Color[ImGuiCol_NavWindowingHighlight] = ImColor(255, 255, 255, 178); // 窗口导航高亮颜色
    Color[ImGuiCol_NavWindowingDimBg] = ImColor(204, 204, 204, 89); // 窗口导航背景颜色
    Color[ImGuiCol_ModalWindowDimBg] = ImColor(204, 204, 204, 89); // 模态窗口背景颜色


    RandomRGB();

    ImGui::Begin("IMGUI_2048", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    {
        ImGui::TextColored(ImColor(random_R, random_G, random_B, 255), "  ImGui - 2048");
        ImGui::SetCursorPos(ImVec2(570, 20));
        if (ImGui::ButtonNew(u8"退出"))exit(0);
        ImGui::SetCursorPos(ImVec2(380, 55));
        ImGui::Text(u8"分数: ");
        ImGui::SameLine();
        // 根据分数设置颜色
        ImVec4 scoreColor;
        if (Score < 500) {
            scoreColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
        }
        else if (Score < 1000) {
            scoreColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
        }
        else if (Score < 2000) {
            scoreColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f); // 橙色
        }
        else {
            scoreColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 红色
        }
        ImGui::TextColored(scoreColor, u8"%d", Score);

        // 绘制游戏网格
        //int cout = 0;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                ImGui::PushID(i * SIZE + j); // 确保每个按钮有唯一的 ID

                ImVec4 cellColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // 默认按钮颜色
                ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 默认文本颜色

                ImGui::SetCursorPos(ImVec2(25+(i*75), 75+(j*75)));
                if (Bound[i][j] != 0) {
                    //int Pos_x = 25;
                   // int Pos_y = 75;
                    switch (Bound[i][j]) {
                    case 2:   cellColor = ImVec4(0.9f, 0.9f, 0.7f, 1.0f); textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); break;
                    case 4:   cellColor = ImVec4(0.9f, 0.8f, 0.5f, 1.0f); textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); break;
                    case 8:   cellColor = ImVec4(0.8f, 0.6f, 0.4f, 1.0f); textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;
                    case 16:  cellColor = ImVec4(0.8f, 0.4f, 0.2f, 1.0f); textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;
                    case 32:  cellColor = ImVec4(0.8f, 0.3f, 0.3f, 1.0f); textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;
                    case 64:  cellColor = ImVec4(0.8f, 0.2f, 0.2f, 1.0f); textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;
                    case 128: cellColor = ImVec4(0.9f, 0.7f, 0.3f, 1.0f); textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); break;
                    case 256: cellColor = ImVec4(0.9f, 0.6f, 0.4f, 1.0f); textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); break;
                    case 512: cellColor = ImVec4(0.9f, 0.5f, 0.5f, 1.0f); textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); break;
                    case 1024:cellColor = ImVec4(0.9f, 0.4f, 0.5f, 1.0f); textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); break;
                    case 2048:textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); cellColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f); break;
                    }
                    //ImGui::Button(std::to_string(Bound[i][j]).c_str(), ImVec2(70, 70)); // 显示数字
                    ImGui::ColorButton("#colorbutton", cellColor, ImGuiColorEditFlags_NoTooltip, ImVec2(70, 70));
                    //cout++;
                    if (Bound[i][j]== 2|| Bound[i][j] == 4|| Bound[i][j] == 8) ImGui::SetCursorPos(ImVec2(55 + (i * 75), 100 + (j * 75)));
                    if(Bound[i][j] == 16 || Bound[i][j] == 32 || Bound[i][j] == 64)ImGui::SetCursorPos(ImVec2(50 + (i * 75), 100 + (j * 75)));
                    if (Bound[i][j] == 128 || Bound[i][j] == 256 || Bound[i][j] == 512)ImGui::SetCursorPos(ImVec2(45 + (i * 75), 100 + (j * 75)));
                    if (Bound[i][j] == 1024 || Bound[i][j] == 2048 )ImGui::SetCursorPos(ImVec2(40 + (i * 75), 100 + (j * 75)));
                    
                    ImGui::TextColored(textColor, std::to_string(Bound[i][j]).c_str());
                    
                   
                }
                else {
                    ImGui::ColorButton("#colorbutton_", cellColor, ImGuiColorEditFlags_NoTooltip, ImVec2(70, 70));
                    //ImGui::Button(" ", ImVec2(70, 70)); 
                }
                ImGui::PopID();
                if (j < SIZE - 1) {
                    ImGui::SameLine(); 
                }
            }
            ImGui::NewLine(); 
        }
        ImGui::SetCursorPos(ImVec2(35,420));
        ImGui::Text(u8"W: 上  A: 左  S: 下  D: 右");
        ImGui::SetCursorPos(ImVec2(35, 450));
        if(GameInfo!="")ImGui::TextColored(ImColor(135, 206, 250, 255), GameInfo);

        ImGui::SetCursorPos(ImVec2(380, 85));
        // 获取游戏的经过时间并显示
        
        float elapsedTime = 0;
        if (GameStart)
        {
            elapsedTime = gameTimer.GetElapsedTime();
            if (Gameover_nFlage > 0)gameTimer.Stop();
        }

        ImGui::Text(u8"时间: %.2f 秒", elapsedTime);
        
        ImGui::SetCursorPos(ImVec2(340, 135));

        ImGui::Text(u8"目标：合并方块，直到出现\n一个数字为 2048 的方块\n\n操作：用上下左右键\n(或 W、A、S、D)滑动所有方块\n\n合并：当两个相同数字的方块\n碰到一起时它们会合并成\n一个新方块数字相加\n\n新方块：每次滑动后随机生成\n一个新的数字方块(2 或 4)\n");
        
            ImGui::SetCursorPos(ImVec2(400, 440));
            if (!GameStart)
            {
                if (ImGui::ButtonNew(u8" 开始游戏"))GameStart = true;
            }
            else
            {
                if (ImGui::ButtonNew(u8" 重新开始"))
                {
                    Gameover_nFlage = 0; // 重置游戏状态
                    Score = 0;
                    GameInfo = "";
                    gameTimer.Reset();
                    for (int i = 0; i < SIZE; ++i) {
                        for (int j = 0; j < SIZE; ++j) {
                            Bound[i][j] = 0;
                        }
                    }

                    StartDate(); // 初始化游戏
                }

            }
    }
    ImGui::End();
}

// 处理键盘输入
void KeyboardInput() {
    // 处理 W 键
    if (GetAsyncKeyState('W') & 0x8000) {
        if (!keyPressed['W']) {
            keyPressed['W'] = true;
            MoveUP();
        }
    }
    else {
        keyPressed['W'] = false; // 重置状态
    }

    // 处理 A 键
    if (GetAsyncKeyState('A') & 0x8000) {
        if (!keyPressed['A']) {
            keyPressed['A'] = true;           
            MoveLEFT();
        }
    }
    else {
        keyPressed['A'] = false; // 重置状态
    }

    // 处理 S 键
    if (GetAsyncKeyState('S') & 0x8000) {
        if (!keyPressed['S']) {
            keyPressed['S'] = true;
            MoveDOWN();
        }
    }
    else {
        keyPressed['S'] = false; // 重置状态
    }

    // 处理 D 键
    if (GetAsyncKeyState('D') & 0x8000) {
        if (!keyPressed['D']) {
            keyPressed['D'] = true;
            
            MoveRIGHT();
        }
    }
    else {
        keyPressed['D'] = false; // 重置状态
    }
}