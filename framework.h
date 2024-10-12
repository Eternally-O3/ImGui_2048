// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <algorithm> 
#include <fstream>
#include <shlobj.h>
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <ctime>
#include <tchar.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
#include <corecrt_math_defines.h>
#include <assert.h>
#include <random>
#include <mutex>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <wintrust.h>
#include <softpub.h>
#include <sstream>
#include <string>
#include <codecvt>
#include <locale>
#include <cstdlib>


#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include <d3d9.h>
#include <d3dx9tex.h>
#pragma comment(lib,"d3d9.lib")

#include"imgui\imgui.h"
#include"imgui\imgui_internal.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"

#define SIZE 4
#include"Timer.h"
#include"Game.h"