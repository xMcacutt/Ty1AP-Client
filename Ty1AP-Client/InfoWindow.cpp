#include "pch.h"
#include "InfoWindow.h"

void InfoWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void InfoWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
    if (!isVisible)
        return;

    // Draw info window
    ImGui::SetNextWindowSizeConstraints(ImVec2(450, 200), ImVec2(450, 500));
    ImGui::Begin(name.c_str(), &isVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(50, 50));
    CreateTree(Z1Info);
    CreateTree(A1Info);
    CreateTree(A2Info);
    CreateTree(A3Info);
    CreateTree(B1Info);
    CreateTree(B2Info);
    CreateTree(B3Info);
    CreateTree(D4Info);
    CreateTree(C1Info);
    CreateTree(C2Info);
    CreateTree(C3Info);
    CreateTree(E1Info);
    CreateTree(D2Info);
    ImGui::PopStyleVar();

    ImGui::End();
}

void InfoWindow::CreateTree(LevelCollectibleInfo levelInfo) {
    if (ImGui::CollapsingHeader(levelInfo.levelName.c_str())) {
        ImGui::PushID(levelInfo.levelName.c_str());
        if (!levelInfo.rainbowScales.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Rainbow Scales")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.rainbowScales) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.goldenCogs.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Golden Cogs")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.goldenCogs) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.bilbies.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Bilbies")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.bilbies) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.pictureFrames.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Picture Frames")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.pictureFrames) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.signposts.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Signposts")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.signposts) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.extraLives.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Extra Lives")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.extraLives) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }
        ImGui::PopID(); 
    }
}