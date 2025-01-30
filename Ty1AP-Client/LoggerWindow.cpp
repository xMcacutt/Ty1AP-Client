#include "LoggerWindow.h"

void LoggerWindow::ToggleVisibility() {
	isVisible = !isVisible;
}

void LoggerWindow::Draw() {
	if (!isVisible)
		return;

    // Render the Logger window at the bottom-left
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Always);
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    // Iterate through the log messages and display them
    auto now = std::chrono::steady_clock::now();
    logMessages.erase(std::remove_if(logMessages.begin(), logMessages.end(),
        [now](const LogMessage& msg) {
            return std::chrono::duration_cast<std::chrono::seconds>(now - msg.timestamp).count() > 5; // 5 seconds
        }), logMessages.end());

    // Get the window draw list for custom drawing
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 text_pos = ImGui::GetCursorScreenPos();  // Initial cursor position

    for (const auto& log : logMessages) {
        // Draw a semi-transparent background rectangle for each message
        ImVec2 text_size = ImGui::CalcTextSize(log.message.c_str());  // Calculate the size of the text
        ImVec2 box_min = text_pos; // Start point of the background box
        ImVec2 box_max = ImVec2(text_pos.x + text_size.x, text_pos.y + text_size.y);  // End point of the background box

        // Draw semi-transparent background (RGBA format: red, green, blue, alpha)
        draw_list->AddRectFilled(box_min, box_max, IM_COL32(0, 0, 0, 100));  // Semi-transparent black background (alpha=100)
        // Now draw the actual text on top of the background
        ImGui::TextWrapped("%s", log.message.c_str());
        // Update text position for next message
        text_pos.y += text_size.y + 2.0f;
    }

    ImGui::End();
}

void LoggerWindow::AddLogMessage(const std::string& message) {
    LogMessage logMessage = { message, std::chrono::steady_clock::now() };
    logMessages.push_back(logMessage);
}

void LoggerWindow::Log(const std::string& message)
{
    for (auto& window : GUI::windows) {
        if (auto logger = dynamic_cast<LoggerWindow*>(window.get())) {
            logger->AddLogMessage(message);
            break;
        }
    }
}
