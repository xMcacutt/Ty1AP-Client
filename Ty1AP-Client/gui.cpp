#include "gui.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


std::vector<std::unique_ptr<Window>> GUI::windows;
std::map<std::string, unsigned int> GUI::icons;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
    io.FontGlobalScale = 1.3f;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    HWND tyWindowHandle = API::GetTyWindowHandle();
    if (tyWindowHandle == 0) {
        API::LogPluginMessage("Error: Invalid window handle.");
        GUI::init = false;
        return;
    }

    ImGui_ImplWin32_InitForOpenGL(API::GetTyWindowHandle());
    ImGui_ImplOpenGL3_Init();

    if (!GUI::LoadIcons()) {
        API::LogPluginMessage("Failed to load icons.");
        return;
    }

    windows.push_back(std::make_unique<LoginWindow>());
    windows.push_back(std::make_unique<LoggerWindow>());
    windows.push_back(std::make_unique<TrackerWindow>());
    windows.push_back(std::make_unique<InfoWindow>());
    API::LogPluginMessage("Initialized ImGui successfully.");
    GUI::init = true;
}

void GUI::DrawUI() {
    if (!GUI::init)
        Initialize();

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
    ImGui::SetNextWindowPos(ImVec2(windowWidth - (114 * uiScale), 0));
    auto popupId = "APMenu";
    ImGui::Begin("AP Button", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
    if (ImGui::ImageButton("##AP_BUTTON", (ImTextureID)(intptr_t)GUI::icons["ap"], ImVec2(64 * uiScale, 64 * uiScale), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
        ImGui::OpenPopup(popupId);
    }
    ImGui::PopStyleColor(3);

    if (ImGui::BeginPopup(popupId)) {
        if (ImGui::MenuItem("Connection")) {
            for (auto& window : windows) {
                if (auto login = dynamic_cast<LoginWindow*>(window.get())) {
                    login->isVisible = true;
                    break;
                }
            }
        }
        if (ImGui::MenuItem("Collectible Info")) {
            for (auto& window : windows) {
                if (auto info = dynamic_cast<InfoWindow*>(window.get())) {
                    info->isVisible = true;
                    break;
                }
            }
        }
    }
    ImGui::EndPopup();
    ImGui::End();

    for (auto& window : windows) { window.get()->Draw(windowWidth, windowHeight, uiScale); }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI::ImGuiWantCaptureMouse() {
    return GUI::init && ImGui::GetIO().WantCaptureMouse;
}

GLuint GUI::LoadTextureFromResource(int resource_id) {
    auto hModule = GetModuleHandle(std::wstring(API::PluginName.begin(), API::PluginName.end()).c_str());

    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resource_id), L"PNG");
    if (!hResource) {
        API::LogPluginMessage("Failed to find resource\n");
        return 0;
    }

    HGLOBAL hMemory = LoadResource(hModule, hResource);
    if (!hMemory) {
        API::LogPluginMessage("Failed to load resource\n");
        return 0;
    }

    DWORD size = SizeofResource(hModule, hResource);
    if (size == 0) {
        API::LogPluginMessage("Resource size is zero\n");
        return 0;
    }

    void* pData = LockResource(hMemory);
    if (!pData) {
        API::LogPluginMessage("Failed to lock resource\n");
        return 0;
    }

    int width, height, channels;
    unsigned char* data = stbi_load_from_memory((unsigned char*)pData, size, &width, &height, &channels, 4);
    if (!data) {
        API::LogPluginMessage(stbi_failure_reason());  // Log the reason for failure
        return 0;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture_id;
}

bool GUI::LoadIcons() {
    icons["thegg"] = GUI::LoadTextureFromResource(THEGG_ICON);
    icons["cog"] = GUI::LoadTextureFromResource(COG_ICON);
    icons["talisman"] = GUI::LoadTextureFromResource(TALISMAN_ICON);
    icons["rang"] = GUI::LoadTextureFromResource(RANG_ICON);
    icons["bilby"] = GUI::LoadTextureFromResource(BILBY_ICON);
    icons["level"] = GUI::LoadTextureFromResource(LEVEL_ICON);
    icons["stopwatch"] = GUI::LoadTextureFromResource(STOPWATCH_ICON);
    icons["boom"] = GUI::LoadTextureFromResource(BOOM_ICON);
    icons["flame"] = GUI::LoadTextureFromResource(FLAME_ICON);
    icons["frosty"] = GUI::LoadTextureFromResource(FROSTY_ICON);
    icons["zappy"] = GUI::LoadTextureFromResource(ZAPPY_ICON);
    icons["doom"] = GUI::LoadTextureFromResource(DOOM_ICON);
    icons["multi"] = GUI::LoadTextureFromResource(MULTI_ICON);
    icons["zoom"] = GUI::LoadTextureFromResource(ZOOM_ICON);
    icons["mega"] = GUI::LoadTextureFromResource(MEGA_ICON);
    icons["infra"] = GUI::LoadTextureFromResource(INFRA_ICON);
    icons["kaboom"] = GUI::LoadTextureFromResource(KABOOM_ICON);
    icons["chrono"] = GUI::LoadTextureFromResource(CHRONO_ICON);
    icons["aqua"] = GUI::LoadTextureFromResource(AQUA_ICON);
    icons["ap"] = GUI::LoadTextureFromResource(AP_ICON);
    return true;
}