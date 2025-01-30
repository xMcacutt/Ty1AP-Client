#pragma once
#include "imgui.h"
#include "ArchipelagoHandler.h"
#include "Window.h"
#include <string>

class LoginWindow : public Window {
public:
    LoginWindow() : Window("Login") {};
    void ToggleVisibility() override;
    void Draw() override;
    void SetMessage(std::string);
    char server[128] = "archipelago.gg:";
    char password[128] = "";
    char slot[128] = "";
private:
    std::string message;
};

