#include "LoginWindow.h"

void LoginWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void LoginWindow::Draw() {
    if (!isVisible) 
        return;

    // Draw login window
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::InputText("Server", server, IM_ARRAYSIZE(server));
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
    ImGui::InputText("Slot Name", slot, IM_ARRAYSIZE(slot));

    if (ImGui::Button("Connect")) {
        if (strlen(server) > 0 && strlen(slot) > 0) {
            ArchipelagoHandler::ConnectAP(this);
            SetMessage("Connecting to " + std::string(server) + "...");
        }
        else {
            SetMessage("Please enter server and slot name");
        }
    }

    ImGui::TextWrapped("%s", message.c_str());
    ImGui::End();
}

void LoginWindow::SetMessage(std::string newMessage) {
    message = newMessage;
}