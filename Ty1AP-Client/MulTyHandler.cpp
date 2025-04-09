#include "pch.h"
#include "MulTyHandler.h"

std::map<int, std::vector<float>> MulTyHandler::posData;
std::map<std::string, int> MulTyHandler::koalaData;

void MulTyHandler::HandlePosData(int level, std::string source, std::vector<float> pos)
{
    if (GameState::onLoadScreenOrMainMenu())
        return;

    if (pos.size() != 4)
        return;

    if (koalaData.find(source) == koalaData.end()) {
        std::set<int> usedIndices;
        for (const auto& entry : koalaData) {
            usedIndices.insert(entry.second);
        }
        for (int i = 0; i <= 7; ++i) {
            if (usedIndices.find(i) == usedIndices.end()) {
                koalaData[source] = i;
                break;
            }
        }
    }

    posData[koalaData[source]] = pos;

    auto modifier = (LevelCode)level == LevelCode::B2 || (LevelCode)level == LevelCode::C2 ? 2 : 1;
    auto offset = (LevelCode)level == LevelCode::B2 || (LevelCode)level == LevelCode::C2 ? 0x518 : 0x0;
    auto koalaOffset = 0x518 * modifier * koalaData[source] + offset;
    auto baseKoalaAddress = *(uintptr_t*)(Core::moduleBase + 0x26B070);
    if (*(unsigned short*)(baseKoalaAddress) != (17327608 & 0xFFFF))
        return;
    *(float*)(baseKoalaAddress + 0x2A4 + koalaOffset) = pos[0];
    *(float*)(baseKoalaAddress + 0x2A8 + koalaOffset) = pos[1];
    *(float*)(baseKoalaAddress + 0x2AC + koalaOffset) = pos[2];
    *(float*)(baseKoalaAddress + 0x2B8 + koalaOffset) = pos[3];
}

void MulTyHandler::TryRemove(std::string source) {
    auto it = koalaData.find(source);
    if (it != koalaData.end()) {
        int index = it->second;
        posData.erase(index);
        koalaData.erase(it);
    }
}

bool MulTyHandler::IsRunning;
void MulTyHandler::Run() {
    IsRunning = true;
    posData = {};
    koalaData = {};
    while (IsRunning) {
        if (GameState::onLoadScreenOrMainMenu())
            continue;
        auto vectorPos = Hero::getPosition();
        auto pos = std::vector<float>{};
        pos.push_back(vectorPos.x);
        pos.push_back(vectorPos.y);
        pos.push_back(vectorPos.z);
        pos.push_back(*(float*)(Core::moduleBase + 0x271C20)); // YAW
        ArchipelagoHandler::SendPosition((int)Level::getCurrentLevel(), pos);
        Sleep(50);
    }
}