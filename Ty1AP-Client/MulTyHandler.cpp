#include "pch.h"
#include "MulTyHandler.h"

std::map<int, std::vector<float>> MulTyHandler::posData;

void MulTyHandler::HandlePosData(int level, int index, std::vector<float> pos)
{
    if (GameState::onLoadScreenOrMainMenu())
        return;

    if (pos.size() != 4)
        return;

    posData[index] = pos;

    auto modifier = (LevelCode)level == LevelCode::B2 || (LevelCode)level == LevelCode::C2 ? 2 : 1;
    auto offset = (LevelCode)level == LevelCode::B2 || (LevelCode)level == LevelCode::C2 ? 0x518 : 0x0;
    auto koalaOffset = 0x518 * modifier * index + offset;
    auto baseKoalaAddress = *(uintptr_t*)(Core::moduleBase + 0x26B070);
    if (*(unsigned short*)(baseKoalaAddress) != (17327608 & 0xFFFF))
        return;
    *(float*)(baseKoalaAddress + 0x2A4 + koalaOffset) = pos[0];
    *(float*)(baseKoalaAddress + 0x2A8 + koalaOffset) = pos[1];
    *(float*)(baseKoalaAddress + 0x2AC + koalaOffset) = pos[2];
    *(float*)(baseKoalaAddress + 0x2B8 + koalaOffset) = pos[3];
}

void MulTyHandler::TryRemove(int index) {
    posData.erase(index);
}

bool MulTyHandler::IsRunning;
void MulTyHandler::Run() {
    IsRunning = true;
    posData = {};
    while (IsRunning) {
        //if (GameState::onLoadScreenOrMainMenu())
        //    continue;
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