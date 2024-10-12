#include "framework.h"

Timer gameTimer;

int Bound[SIZE][SIZE]; // ��Ϸ��ͼ
int RandNum_nFalge;    // �Ƿ����һ���µ��������־
int Gameover_nFlage;   // ��Ϸ����״̬
int Score;             // ��Ϸ����
bool GameStart=false;
bool keyPressed[256] = { false }; // �洢����״̬

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

// �����ƶ�ʵ��
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

// ���������
void RandNum() {
    int emptyCount = nCountNullNum();
    if (emptyCount == 0) return; // ���û�пո�ֱ�ӷ���

    // ���ѡ��һ���ո�
    srand((unsigned int)time(NULL));
    int n = rand() % emptyCount;

    // �������飬�ҵ���n���ո�
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (Bound[i][j] == 0) { // �ҵ��ո�
                if (n == 0) { // �ҵ���n���ո�
                    Bound[i][j] = (rand() % 2 == 0) ? 2 : 4; // �������2��4
                    RandNum_nFalge = 0; // ������ɺ����ñ�־
                    return; // ������ϣ�����
                }
                n--; // ����n������Ѱ����һ���ո�
            }
        }
    }
}

// �����Ϸ����
void CheckGameOver() {
    bool hasEmptyCell = false;
    bool canMerge = false;

    // ����Ƿ��пո���
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (Bound[i][j] == 0) {
                hasEmptyCell = true; // ���пո��ӣ���Ϸû�н���
            }
            if (Bound[i][j] >= 2048) {
                Gameover_nFlage = 2; // �ﵽ2048����Ϸʤ��
                return; // ֱ�ӷ��أ���Ϊʤ�������Ѿ�����
            }
        }
    }

    // ����Ƿ��пɺϲ������ڸ��ӣ����������
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (Bound[i][j] == Bound[i][j + 1]) {
                canMerge = true; // �����п��Ժϲ��ĸ���
            }
            if (Bound[j][i] == Bound[j + 1][i]) {
                canMerge = true; // �����п��Ժϲ��ĸ���
            }
        }
    }

    // ���û�пո�����û�пɺϲ��ĸ��ӣ���Ϸ����
    if (!hasEmptyCell && !canMerge) {
        Gameover_nFlage = 1; // ��Ϸʧ��
    }
}

// ��Ϸ��ʼ��
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

// ������ʾ
void ShowNum(int i, int j) {
    if (Bound[i][j] != 0) {
        ImGui::Text("%5d", Bound[i][j]);
    }
    else {
        ImGui::Text("     ");
    }
}

// ��Ϸ������ʾ
void ShowWindows() {
    ImGuiStyle& Style = ImGui::GetStyle();
    auto Color = Style.Colors;
    Style.WindowMinSize = ImVec2(650, 520);
    ImVec2 displaySize = ImVec2((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN)); // ��Ļ�ߴ�
    ImVec2 windowPos = ImVec2((displaySize.x - Style.WindowMinSize.x) * 0.5f, (displaySize.y - Style.WindowMinSize.y) * 0.5f); // ���㴰��λ��
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
    Style.WindowBorderSize = 2;

    Style.ChildRounding = 0;
    Style.FrameRounding = 5;
    Style.ScrollbarRounding = 0;
    Style.GrabRounding = 1;
    Style.PopupRounding = 1;
    Style.WindowRounding = 1;

    Color[ImGuiCol_Text] = ImColor(245, 245, 245); // �ı���ɫ����ɫ
    Color[ImGuiCol_TextDisabled] = ImColor(130, 130, 130); // �����ı���ɫ
    Color[ImGuiCol_WindowBg] = ImColor(40, 40, 40); // ���ڱ�����ɫ
    Color[ImGuiCol_ChildBg] = ImColor(40, 40, 40); // �Ӵ��ڱ�����ɫ
    Color[ImGuiCol_PopupBg] = ImColor(50, 50, 50); // �������ڱ�����ɫ
    Color[ImGuiCol_Border] = ImColor(90, 90, 90); // �߿���ɫ
    Color[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 0); // �߿���Ӱ��ɫ
    Color[ImGuiCol_FrameBg] = ImColor(50, 50, 50); // ��ܱ�����ɫ
    Color[ImGuiCol_FrameBgHovered] = ImColor(70, 130, 180); // ��ͣ��ܱ�����ɫ
    Color[ImGuiCol_FrameBgActive] = ImColor(70, 130, 180, 100); // �����ܱ�����ɫ
    Color[ImGuiCol_TitleBg] = ImColor(30, 30, 30); // ���ⱳ����ɫ
    Color[ImGuiCol_TitleBgActive] = ImColor(135, 206, 250); // ������ⱳ����ɫ
    Color[ImGuiCol_TitleBgCollapsed] = ImColor(30, 30, 30); // �۵����ⱳ����ɫ
    Color[ImGuiCol_MenuBarBg] = ImColor(50, 50, 50); // �˵���������ɫ
    Color[ImGuiCol_ScrollbarBg] = ImColor(40, 40, 40); // ������������ɫ
    Color[ImGuiCol_ScrollbarGrab] = ImColor(80, 80, 80); // ������ץȡ��ɫ
    Color[ImGuiCol_ScrollbarGrabHovered] = ImColor(135, 206, 250); // ������ץȡ��ͣ��ɫ
    Color[ImGuiCol_ScrollbarGrabActive] = ImColor(135, 206, 250, 100); // ������ץȡ������ɫ
    Color[ImGuiCol_CheckMark] = ImColor(135, 206, 250); // ��ѡ�����ɫ
    Color[ImGuiCol_SliderGrab] = ImColor(135, 206, 250); // ����ץȡ��ɫ
    Color[ImGuiCol_SliderGrabActive] = ImColor(135, 206, 250, 100); // ����ץȡ������ɫ
    Color[ImGuiCol_Button] = ImColor(100, 100, 100); // ��ť��ɫ
    Color[ImGuiCol_ButtonHovered] = ImColor(135, 206, 250); // ��ť��ͣ��ɫ
    Color[ImGuiCol_ButtonActive] = ImColor(135, 206, 250, 100); // ��ť������ɫ
    Color[ImGuiCol_Header] = ImColor(135, 206, 250); // ������ɫ
    Color[ImGuiCol_HeaderHovered] = ImColor(77, 200, 250); // ������ͣ��ɫ
    Color[ImGuiCol_HeaderActive] = ImColor(135, 206, 250, 100); // ���⼤����ɫ
    Color[ImGuiCol_Separator] = ImColor(90, 90, 90); // �ָ�����ɫ
    Color[ImGuiCol_SeparatorHovered] = ImColor(135, 206, 250); // ��ͣ�ָ�����ɫ
    Color[ImGuiCol_SeparatorActive] = ImColor(135, 206, 250, 100); // ����ָ�����ɫ
    Color[ImGuiCol_ResizeGrip] = ImColor(50, 50, 50); // ������С�ֱ���ɫ
    Color[ImGuiCol_ResizeGripHovered] = ImColor(135, 206, 250); // ��ͣ������С�ֱ���ɫ
    Color[ImGuiCol_ResizeGripActive] = ImColor(135, 206, 250, 100); // ���������С�ֱ���ɫ
    Color[ImGuiCol_Tab] = ImColor(60, 60, 60); // ��ǩ��ɫ
    Color[ImGuiCol_TabHovered] = ImColor(135, 206, 250); // ��ǩ��ͣ��ɫ
    Color[ImGuiCol_TabActive] = ImColor(80, 80, 80); // �����ǩ��ɫ
    Color[ImGuiCol_TabUnfocused] = ImColor(40, 40, 40); // δ�۽���ǩ��ɫ
    Color[ImGuiCol_TabUnfocusedActive] = ImColor(60, 60, 60); // δ�۽������ǩ��ɫ
    Color[ImGuiCol_DockingPreview] = ImColor(135, 206, 250, 89); // DockingԤ����ɫ
    Color[ImGuiCol_DockingEmptyBg] = ImColor(30, 30, 30); // Docking�ձ�����ɫ
    Color[ImGuiCol_PlotLines] = ImColor(135, 206, 250); // ��ͼ����ɫ
    Color[ImGuiCol_PlotLinesHovered] = ImColor(255, 110, 90); // ��ͣ��ͼ����ɫ
    Color[ImGuiCol_PlotHistogram] = ImColor(135, 206, 250); // ��ͼֱ��ͼ��ɫ
    Color[ImGuiCol_PlotHistogramHovered] = ImColor(255, 110, 90); // ��ͣ��ͼֱ��ͼ��ɫ
    Color[ImGuiCol_TextSelectedBg] = ImColor(135, 206, 250, 100); // �ı�ѡ�񱳾���ɫ
    Color[ImGuiCol_DragDropTarget] = ImColor(135, 206, 250); // �Ϸ�Ŀ����ɫ
    Color[ImGuiCol_NavHighlight] = ImColor(135, 206, 250); // ����������ɫ
    Color[ImGuiCol_NavWindowingHighlight] = ImColor(255, 255, 255, 178); // ���ڵ���������ɫ
    Color[ImGuiCol_NavWindowingDimBg] = ImColor(204, 204, 204, 89); // ���ڵ���������ɫ
    Color[ImGuiCol_ModalWindowDimBg] = ImColor(204, 204, 204, 89); // ģ̬���ڱ�����ɫ


    RandomRGB();

    ImGui::Begin("IMGUI_2048", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    {
        ImGui::TextColored(ImColor(random_R, random_G, random_B, 255), "  ImGui - 2048");
        ImGui::SetCursorPos(ImVec2(570, 20));
        if (ImGui::ButtonNew(u8"�˳�"))exit(0);
        ImGui::SetCursorPos(ImVec2(380, 55));
        ImGui::Text(u8"����: ");
        ImGui::SameLine();
        // ���ݷ���������ɫ
        ImVec4 scoreColor;
        if (Score < 500) {
            scoreColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // ��ɫ
        }
        else if (Score < 1000) {
            scoreColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // ��ɫ
        }
        else if (Score < 2000) {
            scoreColor = ImVec4(1.0f, 0.5f, 0.0f, 1.0f); // ��ɫ
        }
        else {
            scoreColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // ��ɫ
        }
        ImGui::TextColored(scoreColor, u8"%d", Score);

        // ������Ϸ����
        //int cout = 0;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                ImGui::PushID(i * SIZE + j); // ȷ��ÿ����ť��Ψһ�� ID

                ImVec4 cellColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Ĭ�ϰ�ť��ɫ
                ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Ĭ���ı���ɫ

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
                    //ImGui::Button(std::to_string(Bound[i][j]).c_str(), ImVec2(70, 70)); // ��ʾ����
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
        ImGui::Text(u8"W: ��  A: ��  S: ��  D: ��");
        ImGui::SetCursorPos(ImVec2(35, 450));
        if(GameInfo!="")ImGui::TextColored(ImColor(135, 206, 250, 255), GameInfo);

        ImGui::SetCursorPos(ImVec2(380, 85));
        // ��ȡ��Ϸ�ľ���ʱ�䲢��ʾ
        
        float elapsedTime = 0;
        if (GameStart)
        {
            elapsedTime = gameTimer.GetElapsedTime();
            if (Gameover_nFlage > 0)gameTimer.Stop();
        }

        ImGui::Text(u8"ʱ��: %.2f ��", elapsedTime);
        
        ImGui::SetCursorPos(ImVec2(340, 135));

        ImGui::Text(u8"Ŀ�꣺�ϲ����飬ֱ������\nһ������Ϊ 2048 �ķ���\n\n���������������Ҽ�\n(�� W��A��S��D)�������з���\n\n�ϲ�����������ͬ���ֵķ���\n����һ��ʱ���ǻ�ϲ���\nһ���·����������\n\n�·��飺ÿ�λ������������\nһ���µ����ַ���(2 �� 4)\n");
        
            ImGui::SetCursorPos(ImVec2(400, 440));
            if (!GameStart)
            {
                if (ImGui::ButtonNew(u8" ��ʼ��Ϸ"))GameStart = true;
            }
            else
            {
                if (ImGui::ButtonNew(u8" ���¿�ʼ"))
                {
                    Gameover_nFlage = 0; // ������Ϸ״̬
                    Score = 0;
                    GameInfo = "";
                    gameTimer.Reset();
                    for (int i = 0; i < SIZE; ++i) {
                        for (int j = 0; j < SIZE; ++j) {
                            Bound[i][j] = 0;
                        }
                    }

                    StartDate(); // ��ʼ����Ϸ
                }

            }
    }
    ImGui::End();
}

// �����������
void KeyboardInput() {
    // ���� W ��
    if (GetAsyncKeyState('W') & 0x8000) {
        if (!keyPressed['W']) {
            keyPressed['W'] = true;
            MoveUP();
        }
    }
    else {
        keyPressed['W'] = false; // ����״̬
    }

    // ���� A ��
    if (GetAsyncKeyState('A') & 0x8000) {
        if (!keyPressed['A']) {
            keyPressed['A'] = true;           
            MoveLEFT();
        }
    }
    else {
        keyPressed['A'] = false; // ����״̬
    }

    // ���� S ��
    if (GetAsyncKeyState('S') & 0x8000) {
        if (!keyPressed['S']) {
            keyPressed['S'] = true;
            MoveDOWN();
        }
    }
    else {
        keyPressed['S'] = false; // ����״̬
    }

    // ���� D ��
    if (GetAsyncKeyState('D') & 0x8000) {
        if (!keyPressed['D']) {
            keyPressed['D'] = true;
            
            MoveRIGHT();
        }
    }
    else {
        keyPressed['D'] = false; // ����״̬
    }
}