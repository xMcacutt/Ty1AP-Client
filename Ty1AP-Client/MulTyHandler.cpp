#include "pch.h"
#include "MulTyHandler.h"

std::map<int, MulTyHandler::TimedPos> MulTyHandler::posData;

void MulTyHandler::HandlePosData(int level, int index, std::vector<float> pos)
{
    if (GameState::onLoadScreenOrMainMenu())
        return;

    if (pos.size() != 4)
        return;
    
    uint64_t now = GetTimeMS();

    auto& data = posData[index];
    data.lastPos = data.newPos;
    data.lastTime = data.newTime;
    
    data.newPos = pos;
    data.newTime = now;

    data.level = level;
}

bool MulTyHandler::IsRunning;

void MulTyHandler::Run() {
    IsRunning = true;
    posData = {};

    int frameCounter = 0;
    while (IsRunning) {
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

void MulTyHandler::DisableDraw(int index) {
    auto& data = posData[index];
    data.lastPos = MulTyHandler::defaultpositions.at(Level::getCurrentLevel());
    data.lastTime = 0;

    data.newPos = MulTyHandler::defaultpositions.at(Level::getCurrentLevel());
    data.newTime = 0;

    data.level = (int)Level::getCurrentLevel();
}

void MulTyHandler::TryRemove(int index) {
    posData.erase(index);
}

void MulTyHandler::RemoveCollision() {
    for (const auto& [index, data] : MulTyHandler::posData) {
        auto modifier = Level::getCurrentLevel() == LevelCode::B2 || Level::getCurrentLevel() == LevelCode::C2 ? 2 : 1;
        auto offset = Level::getCurrentLevel() == LevelCode::B2 || Level::getCurrentLevel() == LevelCode::C2 ? 0x518 : 0x0;
        auto koalaOffset = 0x518 * modifier * index + offset;
        auto baseKoalaAddress = *(uintptr_t*)(Core::moduleBase + 0x26B070);

        *(float*)(baseKoalaAddress + 0x298 + koalaOffset) = (byte)0;
    }
}

void MulTyHandler::ScaleKoalas() {
    float outbackMultiplier = 1.0f;
    if (Level::getCurrentLevel() == LevelCode::B3) {
        outbackMultiplier = 2.0f;
    }
    float initialScale =  1.5f;
    float scaleFactor = initialScale * outbackMultiplier;
    if (scaleFactor < 0.5f) scaleFactor = 0.5f;
    if (scaleFactor > 3) scaleFactor = 3;
    const float snugsMultiplier = 1.0725f;
    const float katieMultiplier = 0.825f;

    for (const auto& [index, data] : MulTyHandler::posData) {

        float tempScaleFactor = scaleFactor;
        if (index == 0) tempScaleFactor = scaleFactor * katieMultiplier;
        if (index == 3) tempScaleFactor = scaleFactor * snugsMultiplier;

        auto modifier = Level::getCurrentLevel() == LevelCode::B2 || Level::getCurrentLevel() == LevelCode::C2 ? 2 : 1;
        auto offset = Level::getCurrentLevel() == LevelCode::B2 || Level::getCurrentLevel() == LevelCode::C2 ? 0x518 : 0x0;
        auto koalaOffset = 0x518 * modifier * index + offset;
        auto baseKoalaAddress = *(uintptr_t*)(Core::moduleBase + 0x26B070);

        *(float*)(baseKoalaAddress + 0x60 + koalaOffset) = tempScaleFactor;
    }
}

void MulTyHandler::InterpolateAndDraw() {

    if (GameState::onLoadScreenOrMainMenu()) {
        return;
    }
    uint64_t now = GetTimeMS();
    for (const auto& [index, data] : MulTyHandler::posData) {
        //API::LogPluginMessage(std::to_string(index) + " " + std::to_string(data.level));

        if (data.lastPos.size() != 4 || data.newPos.size() != 4) {
            continue;
        }

        if ((int)Level::getCurrentLevel() != data.level) {
            continue;
        }

        if (now - data.newTime >= 30000) // 30 seconds = 30,000 milliseconds
        {
            DisableDraw(index);
        }

        float t = (now - data.lastTime) / float(data.newTime - data.lastTime);
        t = std::clamp(t, 0.0f, 1.0f);

        float x = Lerp(data.lastPos[0], data.newPos[0], t);
        float y = Lerp(data.lastPos[1], data.newPos[1], t);
        float z = Lerp(data.lastPos[2], data.newPos[2], t);
        float yaw = LerpAngle(data.lastPos[3], data.newPos[3], t);
        int level = data.level;
        
        std::vector<float> interpolated{ x, y, z };
        auto playerPos = Hero::getPosition();
        std::vector<float> playerVec{ playerPos.x, playerPos.y, playerPos.z };
        float distSq = DistanceSquared(playerVec, interpolated);
        const float maxDistSq = 35000000000.0f;

        if (distSq > maxDistSq) {
            // Snap to end pos
            x = data.newPos[0];
            y = data.newPos[1];
            z = data.newPos[2];
            yaw = data.newPos[3];
        }
        auto modifier = (LevelCode)level == LevelCode::B2 || (LevelCode)level == LevelCode::C2 ? 2 : 1;
        auto offset = (LevelCode)level == LevelCode::B2 || (LevelCode)level == LevelCode::C2 ? 0x518 : 0x0;
        auto koalaOffset = 0x518 * modifier * index + offset;
        auto baseKoalaAddress = *(uintptr_t*)(Core::moduleBase + 0x26B070);
        if (baseKoalaAddress == NULL) {
            return;
            API::LogPluginMessage("Koala Null, Do you have the Patch?");
        }
        if (*(unsigned short*)(baseKoalaAddress) != (17327608 & 0xFFFF)) {
            API::LogPluginMessage("Not a Koala, Do you have the Patch?");
            return;
        }
        
        ScaleKoalas();
        RemoveCollision();

        *(float*)(baseKoalaAddress + 0x2A4 + koalaOffset) = x;
        *(float*)(baseKoalaAddress + 0x2A8 + koalaOffset) = y;
        *(float*)(baseKoalaAddress + 0x2AC + koalaOffset) = z;
        *(float*)(baseKoalaAddress + 0x2B8 + koalaOffset) = yaw;
        auto position2Address = *(uintptr_t*)(baseKoalaAddress + 8 + koalaOffset);
        *(float*)(position2Address + 0x74) = x;
        *(float*)(position2Address + 0x78) = y;
        *(float*)(position2Address + 0x7C) = z;
    }
}

float MulTyHandler::Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float MulTyHandler::LerpAngle(float a, float b, float t) {
    float delta = fmodf(b - a + 540.0f, 360.0f) - 180.0f;
    return a + delta * t;
}

uint64_t MulTyHandler::GetTimeMS() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

float MulTyHandler::DistanceSquared(const std::vector<float>& a, const std::vector<float>& b) {
    return (a[0] - b[0]) * (a[0] - b[0]) +
        (a[1] - b[1]) * (a[1] - b[1]) +
        (a[2] - b[2]) * (a[2] - b[2]);
}
