#pragma once
#include "Fierce3D.h"
#include <stdlib.h>
#include <string>
#include <vector>

#include <Windows.h>//#include "framework.h"
#include "resource.h"


//int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
// Forward Windows functions
HWND GetWindowHandle();
HINSTANCE GetInstance();
bool CreateWin64Window( Fierce3D::CF3DApp* app, const char* appName, const char* winName, const Fierce3D::SVideoCreateStruct& VCStruct, bool show );

// Helper Functions
LPCWSTR StringToLPCWSTR(const std::string& str);
std::vector<wchar_t> StringToWCharArray(const std::string& str);

