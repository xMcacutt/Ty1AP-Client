#pragma once
#include "apclient.hpp"
#include "pch.h"
#include <map>
#include <string>
#include <vector>
#include <functional>
#include "TygerFrameworkAPI.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "LoggerWindow.h"
#include "LoginWindow.h"
#include "TrackerWindow.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "resource.h"
#include "include/TyNumberFont.hpp"


class GUI
{
public:
	static inline bool init;
	static inline bool isShown;
	static std::vector<std::unique_ptr<Window>> windows;
	static void Initialize();
	static void DrawUI();
	static bool ImGuiWantCaptureMouse();
	static bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static std::map<std::string, GLuint> icons;
private:
	static bool LoadIcons();
	static GLuint LoadTextureFromResource(int resource_id);
};
