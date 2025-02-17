#pragma once
#include "ArchipelagoHandler.h"
#include "CheckHandler.h"
#include "ItemHandler.h"
#include "TimeAttackHandler.h"
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
#include <vector>
#include "hero.h"

class GameHandler
{
public:
	static void Setup();
	static void SetupOnConnect();
	static void SetLoadActive(bool value);
	static void HandleItemReceived(APClient::NetworkItem item);
	static bool isTyShutdown;
private:
	static void OnEnterRainbowCliffs();
	static void OnEnterCrest();
	static void OnEnterShipRex();
	static void OnEnterLevel();
	static void OnMainMenu();
	static void OnLoadGame();
	static void OnDeath();
	static void OnSpawnpointSet();
	static void __stdcall MainMenuHook();
	static void __stdcall SpawnpointHook();
	static void __stdcall StopwatchHook();
	static void __stdcall LoadGameHook();
	static void __stdcall DeathHook();
	static void WatchMemory();
	static inline std::string hardDiskMessage = "Welcome to Ty Archipelago\n\n\nIf you're seeing this, everything is probably fine\nYour vanilla save files are untouched\nAutosaves occur on each check and item\n\nGood Luck\n\n\nxMcacutt\0";
};
