#include "LoggerWindow.h"

void LoggerWindow::ToggleVisibility() {
	isVisible = !isVisible;
}

void LoggerWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
	if (!isVisible)
		return;

    // Render the Logger window at the bottom-left
    ImGui::SetNextWindowPos(ImVec2(10, outerHeight - 500 - 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_Always);
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);

    // Iterate through the log messages and display them
    auto now = std::chrono::steady_clock::now();
    logMessages.erase(std::remove_if(logMessages.begin(), logMessages.end(),
        [now](const LogMessage& msg) {
            return std::chrono::duration_cast<std::chrono::seconds>(now - msg.timestamp).count() > 5; // 5 seconds
        }), logMessages.end());

    // Get the window draw list for custom drawing
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Get the available window size
    ImVec2 window_pos = ImGui::GetCursorScreenPos(); // Top-left corner of content
    ImVec2 window_size = ImGui::GetContentRegionAvail(); // Available space inside window

    // Start drawing from the bottom
    float y_pos = window_pos.y + window_size.y;

    // Iterate messages in reverse order (newest at bottom)
    for (auto it = logMessages.rbegin(); it != logMessages.rend(); ++it) {
        const LogMessage& log = *it;

        ImVec2 text_size = ImGui::CalcTextSize(log.message.c_str());
        y_pos -= text_size.y + 2.0f;  // Move up for next message

        ImVec2 box_min = ImVec2(window_pos.x, y_pos);
        ImVec2 box_max = ImVec2(window_pos.x + text_size.x, y_pos + text_size.y);

        // Draw semi-transparent background for text
        draw_list->AddRectFilled(box_min, box_max, IM_COL32(0, 0, 0, 100));

        // Draw the text
        draw_list->AddText(ImVec2(window_pos.x, y_pos), IM_COL32(255, 255, 255, 255), log.message.c_str());
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
