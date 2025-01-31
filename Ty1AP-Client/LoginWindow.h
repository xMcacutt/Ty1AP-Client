#pragma once
#include "imgui.h"
#include "ArchipelagoHandler.h"
#include <string>
#include "Window.h"

class LoginWindow : public Window {
public:
    LoginWindow() : Window("Login") {};
    void ToggleVisibility() override;
    void Draw(int outerWidth, int outerHeight, float uiScale) override;
    void SetMessage(std::string);
    char server[128] = "archipelago.gg:";
    char password[128] = "";
    char slot[128] = "";
private:
    std::string message;
};

