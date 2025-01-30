#pragma once
#include "ArchipelagoHandler.h"
#include "core.h"
#include "gameobject.h"
#include "TygerFrameworkAPI.hpp"
#include <string>
#include "level.h"
#include "MinHook.h"
#include "SaveDataHandler.h"
#include "windows.h"

class GameHandler
{
public:
	static void Setup();
	static void SetupOnConnect(std::string seed);
	static void SetLoadActive(bool value);
private:
	static void OnEnterRainbowCliffs();
	static void OnEnterLevel();
	static void OnMainMenu();
	static void __stdcall LevelInitHook();
	static void __stdcall MainMenuHook();
};

