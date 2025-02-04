#pragma once
#include "ArchipelagoHandler.h"
#include "CheckHandler.h"
#include "ItemHandler.h"
#include "core.h"
#include "gameobject.h"
#include "TygerFrameworkAPI.hpp"
#include <string>
#include "level.h"
#include "MinHook.h"
#include "SaveDataHandler.h"
#include <filesystem>
#include <memory>
#include "windows.h"

class GameHandler
{
public:
	static void Setup();
	static void SetupOnConnect(std::string seed);
	static void SetLoadActive(bool value);
	static void HandleItemReceived(APClient::NetworkItem item);
private:
	static void OnEnterRainbowCliffs();
	static void OnEnterLevel();
	static void OnMainMenu();
	static void OnLoadSaves();
	static void __stdcall MainMenuHook();
	static void WatchMemory();
};

