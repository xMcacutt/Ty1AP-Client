#include "SaveDataHandler.h"

std::string GetSteamUserdataPath() {
    HKEY hKey;
    const char* regPath = "SOFTWARE\\Valve\\Steam";
    char steamPath[MAX_PATH];
    DWORD steamPathSize = sizeof(steamPath);

    if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "SteamPath", nullptr, nullptr, (LPBYTE)steamPath, &steamPathSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::string(steamPath) + "\\userdata\\";
        }
        RegCloseKey(hKey);
    }

    return "";
}

std::string GetSteamUserID() {
    HKEY hKey;
    const char* regPath = "SOFTWARE\\Valve\\Steam\\ActiveProcess";
    DWORD activeUserID;
    DWORD size = sizeof(activeUserID);

    if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "ActiveUser", nullptr, nullptr, (LPBYTE)&activeUserID, &size) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::to_string(activeUserID);
        }
        RegCloseKey(hKey);
    }
    return "";
}

bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

bool saveToFile(const std::string& filePath, const ExtendedSaveData saveData) {
    std::ofstream file(filePath, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&saveData), sizeof(ExtendedSaveData));
        file.close();
        return true;
    }
    LoggerWindow::Log("Error, unable to create file.");
    return false;
}

bool loadFromFile(const std::string& filePath, ExtendedSaveData saveData) {
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
    auto filePath = "./Saves/" + seed;
    createDirectoriesIfNeeded(filePath);
    ExtendedSaveData saveData = {};
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
        saveData.Size = sizeof(ExtendedSaveData);
        saveData.Magic = 0x701EE;
        saveData.AttributeData.GotBoomerang = true;
        saveData.AttributeData.GotSecondRang = true;
        saveToFile(filePath, saveData);
        LoggerWindow::Log("Save file created");
    }
    auto steamUserDataPath = GetSteamUserdataPath();
    auto steamUserID = GetSteamUserID();
    auto savePath = steamUserDataPath + "\\" + steamUserID + "\\" + "411960" + "\\" + "remote\\Game 3";
    std::filesystem::copy_file(filePath, savePath, std::filesystem::copy_options::overwrite_existing);
    SaveDataHandler::saveData = saveData;
    return true;
}

bool RedirectSaveData() {

}
