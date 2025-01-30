#include "SaveDataHandler.h"

bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

bool saveToFile(const std::string& filePath, const ExtendedSaveData& saveData) {
    std::ofstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&saveData), sizeof(ExtendedSaveData));
        file.close();
        return true;
    }
    LoggerWindow::Log("Error, unable to create file.");
    return false;
}

bool loadFromFile(const std::string& filePath, ExtendedSaveData& saveData) {
    std::ifstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&saveData), sizeof(ExtendedSaveData));
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
    auto filePath = "./Saves/" + seed + ".ex";
    createDirectoriesIfNeeded(filePath);
    ExtendedSaveData saveData;
    if (fileExists(filePath)) {
        if (loadFromFile(filePath, saveData))
            LoggerWindow::Log("Save loaded successfully!");
        else {
            LoggerWindow::Log("Error: Unable to read save.");
            return false;
        }
    }
    else {
        LoggerWindow::Log("Creating a new save file.");
        saveToFile(filePath, saveData);
        LoggerWindow::Log("Save file created");
    }
    SaveDataHandler::saveData = saveData;
    return true;
}
