#include "SaveDataHandler.h"

bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

bool saveToFile(const std::string& filePath) {
    std::ofstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&SaveDataHandler::saveData), sizeof(ExtendedSaveData));
        file.close();
        return true;
    }
    LoggerWindow::Log("Error, unable to create file.");
    return false;
}

bool loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&SaveDataHandler::saveData), sizeof(ExtendedSaveData));
        file.close();
        return true;
    }
    return false;
}

void createDirectoriesIfNeeded(const std::string& filepath) {
    std::filesystem::path path(filepath);
    std::filesystem::create_directories(path.parent_path());
}

bool SaveDataHandler::LoadSaveData(std::string seed)
{
    auto filePath = "./Saves/" + seed;
    createDirectoriesIfNeeded(filePath);
    if (fileExists(filePath)) {
        if (loadFromFile(filePath))
            LoggerWindow::Log("Save loaded successfully!");
        else {
            LoggerWindow::Log("Error: Unable to read save.");
            return false;
        }
    }
    else {
        LoggerWindow::Log("Creating a new save file.");
        saveData.Size = sizeof(ExtendedSaveData);
        saveData.Magic = 0x701EE;
        saveData.AttributeData.GotBoomerang = true;
        saveData.ProgressiveRang = 1;
        if (!ArchipelagoHandler::startWithBoom) {
            saveData.ProgressiveRang--;
        }
        saveData.LevelData[0].TimesEntered = 1;
        saveData.PortalOpen[4] = true;
        if (ArchipelagoHandler::levelUnlockStyle == LevelUnlockStyle::VANILLA) {
            saveData.PortalOpen[5] = true;
            saveData.PortalOpen[6] = true;
            saveData.PortalOpen[8] = true;
            saveData.PortalOpen[9] = true;
            saveData.PortalOpen[10] = true;
            saveData.PortalOpen[12] = true;
            saveData.PortalOpen[13] = true;
            saveData.PortalOpen[14] = true;
            saveData.PortalOpen[21] = true;
        }
        saveToFile(filePath);
        LoggerWindow::Log("Save file created");
    }
    return true;
}

void SaveDataHandler::SaveGame()
{
    auto objAddr = Core::moduleBase + 0x2888AC;
    auto ediAddr = 0x7562A210;
    void (*calcOpals)(intptr_t) = reinterpret_cast<void(*)(intptr_t)>(Core::moduleBase + 0xF73D0);
    __asm {
        push ediAddr
        mov ecx, objAddr
        call calcOpals
    }
    auto filePath = "./Saves/" + ArchipelagoHandler::seed;
    saveToFile(filePath);
}
