#pragma once
#include "apclient.hpp"
#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "LoggerWindow.h"
#include "LoginWindow.h"
#include "TrackerWindow.h"
#include "InfoWindow.h"
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
