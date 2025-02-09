#include "LoggerWindow.h"

void LoggerWindow::ToggleVisibility() {
	isVisible = !isVisible;
}

void LoggerWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
	if (!isVisible)
		return;

    // Render the Logger window at the bottom-left
    ImGui::SetNextWindowPos(ImVec2(10, outerHeight - 500 - 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_Always);
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
        float x_pos = window_pos.x;

        ImVec2 text_size = ImGui::CalcTextSize(log.message.c_str());
        y_pos -= text_size.y + 2.0f;  // Move up for next message

        // Draw semi-transparent background for text
        ImVec2 box_min = ImVec2(window_pos.x, y_pos);
        ImVec2 box_max = ImVec2(window_pos.x + text_size.x, y_pos + text_size.y);
        draw_list->AddRectFilled(box_min, box_max, IM_COL32(0, 0, 0, 100));

        // Render formatted text with colors
        RenderFormattedText(draw_list, log.message.c_str(), ImVec2(x_pos, y_pos));
    }

    ImGui::End();
}

void LoggerWindow::RenderFormattedText(ImDrawList* draw_list, const char* text, ImVec2 pos) {
    ImU32 current_color = IM_COL32(255, 255, 255, 255); // Default to white
    const char* segment_start = text;
    const char* s = text;
    float x_pos = pos.x;

    while (*s) {
        if (*s == '[' && *(s + 1) == 'c' && strncmp(s, "[color=", 7) == 0) {
            // Draw previous segment before color change
            if (segment_start < s) {
                draw_list->AddText(ImVec2(x_pos, pos.y), current_color, segment_start, s);
                x_pos += ImGui::CalcTextSize(segment_start, s).x;
            }

            // Parse color code
            s += 7; // Skip "[color="
            unsigned int r, g, b, a = 255;
            if (sscanf(s, "%2x%2x%2x%2x", &r, &g, &b, &a) >= 3) {
                current_color = IM_COL32(r, g, b, a);
            }

            // Move past the color tag
            while (*s && *s != ']') s++;
            if (*s == ']') s++;

            segment_start = s;
        }
        else {
            s++;
        }
    }

    // Draw remaining text
    if (segment_start < s) {
        draw_list->AddText(ImVec2(x_pos, pos.y), current_color, segment_start, s);
    }
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
