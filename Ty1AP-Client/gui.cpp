#include "gui.h"

std::vector<std::unique_ptr<Window>> GUI::windows;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN) {
        // Check if the key corresponds to any window's toggle hotkey
        if (wParam == VK_F2) {
            GUI::isShown = !GUI::isShown;
        }
        if (wParam == VK_F3) {
            for (auto& window : windows) {
                if (auto login = dynamic_cast<LoginWindow*>(window.get())) {
                    login->ToggleVisibility();
                    break;
                }
            }
        }
    }
    if (API::DrawingGUI())
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
    return false;
}

// Initialize the GUI system and windows
void GUI::Initialize() {
    ImGui::CreateContext();

    API::SetImGuiFont(ImGui::GetIO().Fonts);
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    HWND tyWindowHandle = API::GetTyWindowHandle();
    if (tyWindowHandle == 0) {
        API::LogPluginMessage("Error: Invalid window handle.");
        GUI::init = false;
        return;
    }

    ImGui_ImplWin32_InitForOpenGL(API::GetTyWindowHandle());
    ImGui_ImplOpenGL3_Init();

    windows.push_back(std::make_unique<LoginWindow>());
    windows.push_back(std::make_unique<LoggerWindow>());
    API::LogPluginMessage("Initialized ImGui successfully.");
    GUI::init = true;
}

void GUI::DrawUI() {
    if (!GUI::init)
        Initialize();
    if (!GUI::isShown)
        return;

    HWND hwnd = (HWND)API::GetTyWindowHandle();
    RECT rect;
    if (!GetClientRect(hwnd, &rect)) {
        API::LogPluginMessage("Failed to get window size.");
        return;
    }
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;
    const int baseWidth = 1920;
    const int baseHeight = 1080;
    float widthScale = static_cast<float>(windowWidth) / baseWidth;
    float heightScale = static_cast<float>(windowHeight) / baseHeight;
    float uiScale = (((widthScale) < (heightScale)) ? (widthScale) : (heightScale));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    for (auto& window : windows) { window.get()->Draw(windowWidth, windowHeight, uiScale); }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI::ImGuiWantCaptureMouse() {
    return GUI::init && ImGui::GetIO().WantCaptureMouse;
}