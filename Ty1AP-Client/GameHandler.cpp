#include "GameHandler.h"

typedef void(__stdcall* LevelInitFunctionType)(void);
LevelInitFunctionType levelInitOrigin = nullptr;

typedef void(__stdcall* MainMenuFunctionType)(void);
MainMenuFunctionType mainMenuOrigin = nullptr;

typedef void(__stdcall* LoadSavesFunctionType)(void);
MainMenuFunctionType loadSavesOrigin = nullptr;


void __stdcall GameHandler::LevelInitHook() {
	GameHandler::OnEnterLevel();
	levelInitOrigin();
}

void __stdcall GameHandler::LoadSavesHook() {
	GameHandler::OnLoadSaves();
	loadSavesOrigin();
}

void __stdcall GameHandler::MainMenuHook() {
	GameHandler::OnMainMenu();
	mainMenuOrigin();
}

void GameHandler::Setup()
{
	// STOPWATCH
	char* addr = (char*)(Core::moduleBase + 0xF8388);
	DWORD oldProtect;
	VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 5);
	addr = (char*)(Core::moduleBase + 0x15A07E);
	VirtualProtect(addr, 3, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 3);

	// ONE RANG SWAP
	addr = (char*)(Core::moduleBase + 0x162B9C);
	VirtualProtect(addr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 6);

	MH_Uninitialize();
	MH_Initialize();
	addr = (char*)(Core::moduleBase + 0x10A1AB);  // Target address
	MH_CreateHook((LPVOID)addr, &LevelInitHook, reinterpret_cast<LPVOID*>(&levelInitOrigin));
	addr = (char*)(Core::moduleBase + 0xE018C);  // Target address
	MH_CreateHook((LPVOID)addr, &MainMenuHook, reinterpret_cast<LPVOID*>(&mainMenuOrigin));
	addr = (char*)(Core::moduleBase + 0x1724C7);
	MH_CreateHook((LPVOID)addr, &LoadSavesHook, reinterpret_cast<LPVOID*>(&loadSavesOrigin));
	//CheckHandler::SetupHooks();
	MH_EnableHook(MH_ALL_HOOKS);

	GameState::setNoIdle(true);

	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	*(char*)(menuAddr + 0x164) = 0x0;
	*(char*)(menuAddr + 0x165) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x164) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x165) = 0x0;
}

void GameHandler::SetupOnConnect(std::string seed) {
	if (SaveDataHandler::LoadSaveData(seed)) {

		SetLoadActive(true);
	}
}

void GameHandler::OnEnterRainbowCliffs() {
	// TODO FIX RAINBOW SCALE BS
	auto theggCollectorCount = *(int*)(Core::moduleBase + 0x26D944);
	auto addr = *(int*)(Core::moduleBase + 0x26D948);
	for (auto tcIndex = 0; tcIndex < theggCollectorCount; tcIndex++) {
		memset((char*)(addr + 0x94), 0x7, 1);
		memset((char*)(addr + 0x48), 0x1, 1);
		addr = *(int*)(addr + 0x34);
	}
	auto juliusCount = *(int*)(Core::moduleBase + 0x2608C4);
	addr = *(uintptr_t*)(Core::moduleBase + 0x2608C8);
	for (auto juliusIndex = 0; juliusIndex < juliusCount; juliusIndex++) {
		memset((char*)(addr + 0xA8), 0x7, 1);
		memset((char*)(addr + 0xC0), 0x1, 1);
		addr = *(int*)(addr + 0x34);
	}
	auto triggerCount = *(int*)(Core::moduleBase + 0x26DBD4);
	auto triggerAddr = *(uintptr_t*)(Core::moduleBase + 0x26DBD8);
	for (auto triggerIndex = 0; triggerIndex < triggerCount; triggerIndex++) {
		auto targetMessage = *(int*)(triggerAddr + 0xA0);
		if (targetMessage == 47) {
			memset((char*)(triggerAddr + 0x89), 0x1, 6);
			memset((char*)(triggerAddr + 0x85), 0x1, 1);
		}
		triggerAddr = *(int*)(triggerAddr + 0x34);
	}

	/*
	if (ArchipelagoHandler::get().levelUnlockStyle != LevelUnlockStyle::VANILLA) {
		auto portalCount = *(int*)(Core::moduleBase + 0x267404);
		auto portalAddr = *(int*)(Core::moduleBase + 0x267408);
		for (auto portalIndex = 0; portalIndex < portalCount; portalIndex++) {
			API::LogPluginMessage(portalIndex + " " + portalAddr);
			auto portalDestination = *(int*)(portalAddr + 0xAC);
			if (SaveDataHandler::saveData.PortalOpen[portalAddr])
				*(int*)(portalAddr + 0x9C) = 1;
			else
				*(int*)(portalAddr + 0x9C) = 3;
			portalAddr = *(int*)(portalAddr + 0x34);
		}
	}*/

	auto& ap = ArchipelagoHandler::get(); 
	auto portalCount = *(int*)(Core::moduleBase + 0x267404);
	auto portalAddr = *(int*)(Core::moduleBase + 0x267408);
	API::LogPluginMessage(std::to_string(ap.levelA1));
	for (auto portalIndex = 0; portalIndex < portalCount; portalIndex++) {
		auto portalDestination = *(int*)(portalAddr + 0xAC);
		if (portalDestination == 4)
			*(int*)(portalAddr + 0xAC) = ap.levelA1;
		if (portalDestination == 5)
			*(int*)(portalAddr + 0xAC) = ap.levelA2;
		if (portalDestination == 6)
			*(int*)(portalAddr + 0xAC) = ap.levelA3;
		if (portalDestination == 7)
			*(int*)(portalAddr + 0xAC) = ap.levelA4;
		if (portalDestination == 8)
			*(int*)(portalAddr + 0xAC) = ap.levelB1;
		if (portalDestination == 9)
			*(int*)(portalAddr + 0xAC) = ap.levelB2;
		if (portalDestination == 10)
			*(int*)(portalAddr + 0xAC) = ap.levelB3;
		if (portalDestination == 19)
			*(int*)(portalAddr + 0xAC) = ap.levelD4;
		if (portalDestination == 12)
			*(int*)(portalAddr + 0xAC) = ap.levelC1;
		if (portalDestination == 13)
			*(int*)(portalAddr + 0xAC) = ap.levelC2;
		if (portalDestination == 14)
			*(int*)(portalAddr + 0xAC) = ap.levelC3;
		if (portalDestination == 15)
			*(int*)(portalAddr + 0xAC) = ap.levelC4;
		portalAddr = *(int*)(portalAddr + 0x34);
	}
}

void GameHandler::HandleItemReceived(APClient::NetworkItem item) {
	if (GameState::onLoadScreenOrMainMenu())
		ItemHandler::storedItems.push(item);
	else
		ItemHandler::HandleItem(item);
}

void GameHandler::OnEnterLevel() {
	*(int*)(Core::moduleBase + 0x288730) = reinterpret_cast<int>(&SaveDataHandler::saveData);
	
	if (SaveDataHandler::saveData.ProgressiveLevel == 0) 
		SaveDataHandler::saveData.AttributeData.GotBoomerang = false;

	ItemHandler::HandleStoredItems();

	auto levelId = Level::getCurrentLevel();
	if (levelId == LevelCode::Z1)
		OnEnterRainbowCliffs();

	if (*(int*)(Core::moduleBase + 0x27041C) != 0
		&& SaveDataHandler::saveData.StopwatchesActive[(int)levelId] == 1)
		*(int*)(*(int*)(Core::moduleBase + 0x270420) + 0x68) = 0x2;
}

void GameHandler::OnMainMenu() {
	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	if (!ArchipelagoHandler::get().ap_connected) {
		*(char*)(menuAddr + 0x164) = 0x0;
		*(char*)(menuAddr + 0x165) = 0x0;
	}
	*(char*)(menuAddr + 0x168 + 0x164) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x165) = 0x0;
}

void GameHandler::OnLoadSaves() {
	//*(int*)(Core::moduleBase + 0x28E6C4) = 2;
	//*(int*)(Core::moduleBase + 0x52F2B8) = 2;
	//*(int*)(Core::moduleBase + 0x273838) = 2;
	//*(int*)(Core::moduleBase + 0x27383C) = 0;
	//*(int*)(Core::moduleBase + 0x273840) = 2;
	//API::LogPluginMessage(std::to_string(reinterpret_cast<int>(&SaveDataHandler::saveData)));
	//*(int*)(Core::moduleBase + 0x52F2A4) = 0x3;
	//*(int*)(Core::moduleBase + 0x273F74) = 0x9;
}

void GameHandler::SetLoadActive(bool value)
{
	if (!GameState::onLoadScreenOrMainMenu())
		return;
	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	*(bool*)(menuAddr + 0x164) = value;
}


