#include "GameHandler.h"

bool GameHandler::isTyShutdown = false;

std::unordered_map<int, std::vector<float>> spawnpointMap{
	{ 4, {-521.0f, 2372.0f, 11286.0f, 1.530f} },
	{ 5, {-792.0f, 2372.0f, 12520.0f, 1.400f} },
	{ 6, {1357.0f, 2372.0f, 12542.0f, -1.460f} },
	{ 7, {483.0f, 2372.0f, 12543.0f, 0.028f} },
	{ 8, {-3349.0f, 148.0f, 9256.0f, 1.810f} },
	{ 9, {-3349.0f, 148.0f, 9256.0f, 2.030f} },
	{ 10, {-3349.0f, 148.0f, 9256.0f, 1.810f} },
	{ 19, {-3349.0f, 148.0f, 9256.0f, 1.859f} },
	{ 12, {5038.0f, 1029.0f, 8098.0f, 1.820f} },
	{ 13, {4803.0f, 1024.0f, 8855.0f, 1.810f} },
	{ 14, {4646.0f, 1025.0f, 9410.0f, 1.790f} },
	{ 15, {4161.0f, 1054.0f, 10735.0f, 1.458f} },
};

const int portalOrder[16] = {7, 5, 4, 13, 10, 23, 20, 19, 9, 21, 22, 12, 8, 6, 14, 15};

typedef void(__stdcall* MainMenuFunctionType)(void);
MainMenuFunctionType mainMenuOrigin = nullptr;

typedef void(__stdcall* SpawnpointFunctionType)(void);
SpawnpointFunctionType spawnpointOrigin = nullptr;

typedef void(__stdcall* StopwatchFunctionType)(void);
StopwatchFunctionType stopwatchOrigin = nullptr;

typedef void(__stdcall* LoadGameFunctionType)(void);
LoadGameFunctionType loadGameOrigin = nullptr;

typedef void(__stdcall* MenuStateFunctionType)(void);
MenuStateFunctionType menuStateOrigin = nullptr;

typedef void(__stdcall* DeathFunctionType)(void);
DeathFunctionType deathOrigin = nullptr;

typedef void(__stdcall* LoadRainbowCliffsFunctionType)(void);
LoadRainbowCliffsFunctionType loadRainbowCliffsOrigin = nullptr;


uintptr_t stopwatchJmpAddr;
uintptr_t stopwatchOriginAddr;
__declspec(naked) void __stdcall GameHandler::StopwatchHook() {
	__asm {
		cmp eax, 0x2
		je skip
		cmp eax, 0x0
		je skip
		mov[esi + 0x68], eax 
	skip :
		jmp[stopwatchOriginAddr]
	}
}

uintptr_t spawnpointOriginAddr;
void (*loadEndTransitionFunc)();
__declspec(naked) void __stdcall GameHandler::SpawnpointHook() {
	__asm {
		pushfd
		call GameHandler::OnSpawnpointSet
		popfd
		call loadEndTransitionFunc
		jmp dword ptr[spawnpointOriginAddr]
	}
}

uintptr_t loadGameOriginAddr;
__declspec(naked) void __stdcall GameHandler::LoadGameHook() {
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
		call GameHandler::OnLoadGame
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
		jmp dword ptr[loadGameOriginAddr]
	}
}

uintptr_t deathOriginAddr;
__declspec(naked) void __stdcall GameHandler::DeathHook() {
	__asm {
		pushad
		pushfd
		call GameHandler::OnDeath
		popfd
		popad
		mov [edx+0xAD0],ax
		jmp dword ptr[deathOriginAddr]
	}
}

uintptr_t loadRainbowCliffsOriginAddr;
__declspec(naked) void __stdcall GameHandler::LoadRainbowCliffsHook() {
	__asm {
		pushad
		pushfd
		call OnLoadRainbowCliffs
		popfd
		popad
		cmp byte ptr[esi+0xA9],00
		jmp dword ptr[loadRainbowCliffsOriginAddr]
	}
}

void __stdcall GameHandler::MainMenuHook() {
	GameHandler::OnMainMenu();
	mainMenuOrigin();
}

void PatchRangMemory(std::vector<std::pair<uintptr_t, short>> patches) {
	DWORD oldProtect;
	for (const auto& patch : patches) {
		char* addr = (char*)(Core::moduleBase + patch.first);
		VirtualProtect(addr, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
		*(short*)addr = patch.second;
	}
}

void GameHandler::Setup()
{
	// STOPWATCH
	char* addr = (char*)(Core::moduleBase + 0xF8388);
	DWORD oldProtect;
	VirtualProtect(addr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 5);

	// SEPARATE SECOND RANG ON JULIUS KABOOM CHECK
	addr = (char*)(Core::moduleBase + 0x16D378);
	oldProtect;
	VirtualProtect(addr, 0xC, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 0xC);

	// ONE RANG SWAP
	addr = (char*)(Core::moduleBase + 0x162B9C);
	VirtualProtect(addr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 6);
	addr = (char*)(Core::moduleBase + 0x162B65);
	VirtualProtect(addr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 6);
	addr = (char*)(Core::moduleBase + 0x162B6E);
	VirtualProtect(addr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 6);
	addr = (char*)(Core::moduleBase + 0x162B5C);
	VirtualProtect(addr, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 6);

	// REMOVE SAVE GAME
	addr = (char*)(Core::moduleBase + 0xE4334);
	VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0xEB, 1);

	// KABOOM FIX
	addr = (char*)(Core::moduleBase + 0x3F8EE);
	VirtualProtect(addr, 2, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0x90, 2);

	*(uintptr_t*)(Core::moduleBase + 0x528210) = *(uintptr_t*)&hardDiskMessage;

	// RANG COG REQ CHECKS
	uintptr_t goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	addr = (char*)(Core::moduleBase + 0xD8A4E);
	for (int i = 0; i < 6; i++) {
		VirtualProtect(addr, 0xC, PAGE_EXECUTE_READWRITE, &oldProtect);
		addr[0] = 0xA1;
		*(uintptr_t*)(addr + 1) = goldenCogAddr;
		memset(addr + 5, 0x90, 5);
		addr += 0xC;
		*(char*)(addr) = (char)(ArchipelagoHandler::cogGating * i + 1);
		addr += 0x37 - 0xC;
	}

	MH_Uninitialize();
	MH_Initialize();
	addr = (char*)(Core::moduleBase + 0xE018C);
	MH_CreateHook((LPVOID)addr, &MainMenuHook, reinterpret_cast<LPVOID*>(&mainMenuOrigin));

	spawnpointOriginAddr = Core::moduleBase + 0x16929D;
	loadEndTransitionFunc = reinterpret_cast<void(*)()>(Core::moduleBase + 0x192C20);
	addr = (char*)(Core::moduleBase + 0x169298);
	MH_CreateHook((LPVOID)addr, &SpawnpointHook, reinterpret_cast<LPVOID*>(&spawnpointOrigin));

	stopwatchJmpAddr = Core::moduleBase + 0x15A0BD;
	stopwatchOriginAddr = Core::moduleBase + 0x15A081;
	addr = (char*)(Core::moduleBase + 0x15A07C);
	MH_CreateHook((LPVOID)addr, &StopwatchHook, reinterpret_cast<LPVOID*>(&stopwatchOrigin));

	// MENU STATE LOAD SLOT BYPASS
	loadGameOriginAddr = Core::moduleBase + 0x170834;
	addr = (char*)(Core::moduleBase + 0x17082A);
	MH_CreateHook((LPVOID)addr, &LoadGameHook, reinterpret_cast<LPVOID*>(&loadGameOrigin));

	deathOriginAddr = Core::moduleBase + 0xF7A1A;
	addr = (char*)(Core::moduleBase + 0xF7A13);
	MH_CreateHook((LPVOID)addr, &DeathHook, reinterpret_cast<LPVOID*>(&deathOrigin));

	loadRainbowCliffsOriginAddr = Core::moduleBase + 0x14D345;
	addr = (char*)(Core::moduleBase + 0x14D33E);
	MH_CreateHook((LPVOID)addr, &LoadRainbowCliffsHook, reinterpret_cast<LPVOID*>(&loadRainbowCliffsOrigin));

	CheckHandler::SetupHooks();
	TimeAttackHandler::SetupHooks();
	MH_EnableHook(MH_ALL_HOOKS);

	GameState::setNoIdle(true);

	// MOVE RANG CHECKS TO MY CUSTOM DATA
	std::vector<std::pair<uintptr_t, short>> patches = {
		{0x3ED9E, 0xBA8}, {0x3CA46, 0xBA8}, {0x3C9AC, 0xBA8}, {0x14D2E1, 0xBB1}, {0x3F446, 0xBA8}, {0x3EEE7, 0xBA8},
		{0x16C40F, 0xBA6}, {0x165B96, 0xBA6}, {0x162B97, 0xBA6}, {0x3F8E2, 0xBA6}, {0x3EF64, 0xBA6},
		{0x2E61B, 0xBA6}, {0x2E5B9, 0xBA6}, {0x2E3FB, 0xBA6}, {0x25F75, 0xBA6}, {0x35723, 0xBA4},
		{0x35754, 0xBA4}, {0x162EFD, 0xBA4}, {0x35781, 0xBA4}, {0x357AE, 0xBA4}, {0x34D0D, 0xBA4},
		{0x34F32, 0xBA4}, {0x26B1E, 0xBA5}, {0xF8490, 0xBA7}, /*{0xD50B1, 0xBA7},*/
		{0x3E32C, 0xBA8}, {0x3E02D, 0xBA8}, {0xF356A, 0xBA8}, {0xF330E, 0xBA8}, {0xF352E, 0xBA8}, {0xF356A, 0xBA8}, 
		{0x3DC0D, 0xBA8} /*Rang Wheel 1*/, {0x3DC40, 0xBA8} /*Rang Wheel 2*/,
		{0x3DFA1, 0xBA8}, {0x3DFD0, 0xBA8}, {0x3DFF6, 0xBA8}

	};
	PatchRangMemory(patches);
	 
	// DETATCH AQUAS FROM REQ SWIM
	addr = (char*)(Core::moduleBase + 0x162F02);
	VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(addr, 0xEB, 1);

	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	*(char*)(menuAddr + 0x164) = 0x0;
	*(char*)(menuAddr + 0x165) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x164) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x165) = 0x0;
	*(int*)(Core::moduleBase + 0x286640) = 0x2;
	std::thread watcher(WatchMemory);
	watcher.join();
}

void GameHandler::WatchMemory() {
	auto currentLoadValue = *(int*)(Core::moduleBase + 0x27EBF0);
	auto oldLoadValue = currentLoadValue;
	while (!isTyShutdown) {
		currentLoadValue = *(int*)(Core::moduleBase + 0x27EBF0);
		if (currentLoadValue != oldLoadValue) {
			if (currentLoadValue != 0 && oldLoadValue == 0)
				OnEnterLevel();
			oldLoadValue = currentLoadValue;
		}

		if (Level::getCurrentLevel() == LevelCode::Z1) {
			auto& portalMap = ArchipelagoHandler::portalMap;
			auto portalCount = *(int*)(Core::moduleBase + 0x267404);
			auto portalAddr = *(int*)(Core::moduleBase + 0x267408);
			for (auto portalIndex = 0; portalIndex < portalCount; portalIndex++) {
				auto portalDestination = portalOrder[portalIndex];
				auto portalLocationLevel = portalDestination;
				if (portalMap.find(portalDestination) != portalMap.end()) {
					portalDestination = portalMap[portalDestination];
					portalLocationLevel = ArchipelagoHandler::inversePortalMap[portalDestination];
				}
				*(int*)(portalAddr + 0xAC) = portalDestination;
				int portalOpenState = SaveDataHandler::saveData.PortalOpen[portalLocationLevel];
				int& portalState = *(int*)(portalAddr + 0x9C);
				if (portalOpenState && portalState == 3) {
					portalState = 1;
				}
				else if (!portalOpenState && portalState == 2) {
					portalState = 3;
				} 
				portalAddr = *(int*)(portalAddr + 0x34);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void GameHandler::SetupOnConnect() {
	uintptr_t goldenCogAddr = (uintptr_t)&SaveDataHandler::saveData.GoldenCogCount;
	auto addr = (char*)(Core::moduleBase + 0xD8A4E);
	DWORD oldProtect;
	for (int i = 0; i < 6; i++) {
		VirtualProtect(addr, 0xC, PAGE_EXECUTE_READWRITE, &oldProtect);
		addr[0] = 0xA1;
		*(uintptr_t*)(addr + 1) = goldenCogAddr;
		memset(addr + 5, 0x90, 5);
		addr += 0xC;
		*(char*)(addr) = (char)(ArchipelagoHandler::cogGating * (i + 1));
		addr += 0x37 - 0xC;
	}
}

void GameHandler::OnEnterRainbowCliffs() {
	auto theggCollectorCount = *(int*)(Core::moduleBase + 0x26D944);
	auto addr = *(uintptr_t*)(Core::moduleBase + 0x26D948);
	for (auto tcIndex = 0; tcIndex < theggCollectorCount; tcIndex++) {
		memset((char*)(addr + 0x94), 0x9, 1);
		memset((char*)(addr + 0x48), 0x0, 1);
		addr = *(int*)(addr + 0x34);
	}
	auto juliusCount = *(int*)(Core::moduleBase + 0x2608C4);
	addr = *(uintptr_t*)(Core::moduleBase + 0x2608C8);
	for (auto juliusIndex = 0; juliusIndex < juliusCount; juliusIndex++) {
		memset((char*)(addr + 0xA8), 0x9, 1);
		memset((char*)(addr + 0xC0), 0x0, 1);
		addr = *(int*)(addr + 0x34);
	}
	auto triggerCount = *(int*)(Core::moduleBase + 0x26DBD4);
	auto triggerAddr = *(uintptr_t*)(Core::moduleBase + 0x26DBD8);
	for (auto triggerIndex = 0; triggerIndex < triggerCount; triggerIndex++) {
		auto targetMessage = *(int*)(triggerAddr + 0xA0);
		if (targetMessage == 47) {
			memset((char*)(triggerAddr + 0x88), 0x0, 7);
			memset((char*)(triggerAddr + 0x85), 0x0, 1);
		}
		triggerAddr = *(int*)(triggerAddr + 0x34);
	}
	auto counterCount = *(int*)(Core::moduleBase + 0x26BCB8 + 0x44);
	auto counterAddr = *(uintptr_t*)(Core::moduleBase + 0x26BCB8 + 0x48);
	for (auto counterIndex = 0; counterIndex < counterCount; counterIndex++) {
		memset((char*)(counterAddr + 0xA0), 0x0, 1);
		memset((char*)(counterAddr + 0x48), 0x0, 1);
		counterAddr = *(int*)(counterAddr + 0x34);
	}

	auto gateAddr = *(uintptr_t*)(Core::moduleBase + 0x269C14);
	gateAddr = *(uintptr_t*)(gateAddr + 0x78);
	*(bool*)(gateAddr + 0x111) = SaveDataHandler::saveData.ArchAttributeData.GotSecondRang;
}

void GameHandler::HandleItemReceived(APClient::NetworkItem item) {
	if (GameState::onLoadScreenOrMainMenu())
		ItemHandler::storedItems.push(item);
	else
		ItemHandler::HandleItem(item);
}

void GameHandler::OnEnterCrest() {
	int scriptCount = *(int*)(Core::moduleBase + 0x26F480 + 0x44);
	int scriptAddr = *(uintptr_t*)(Core::moduleBase + 0x26F480 + 0x48);
	for (int scriptIndex = 0; scriptIndex < scriptCount; scriptIndex++) {
		int scriptId = *(int*)(scriptAddr + 0x14);
		API::LogPluginMessage(std::to_string(scriptId));
		if (scriptId == 32) {
			*(int*)(scriptAddr + 0x58) = 0;
			break;
		}
		scriptAddr = *(uintptr_t*)(scriptAddr + 0x34);
	}
	bool canGoE4 = ((SaveDataHandler::saveData.TalismansPlaced[0]
		&& SaveDataHandler::saveData.TalismansPlaced[1]
		&& SaveDataHandler::saveData.TalismansPlaced[2]
		&& SaveDataHandler::saveData.TalismansPlaced[3]) && ArchipelagoHandler::goalReqBosses)
		|| (!ArchipelagoHandler::goalReqBosses && SaveDataHandler::saveData.TalismansPlaced[3]);
	auto triggerCount = *(int*)(Core::moduleBase + 0x26DC20 + 0x44);
	auto triggerAddr = *(uintptr_t*)(Core::moduleBase + 0x26DC20 + 0x48);
	for (auto triggerIndex = 0; triggerIndex < triggerCount; triggerIndex++) {
		auto triggerId = *(int*)(triggerAddr + 0x14);
		if (triggerId == 3) {
			memset((char*)(triggerAddr + 0x88), canGoE4, 7);
			memset((char*)(triggerAddr + 0x85), canGoE4, 1);
		}
		triggerAddr = *(int*)(triggerAddr + 0x34);
	}
}

void GameHandler::OnEnterShipRex() {
	if (!SaveDataHandler::saveData.AttributeData.GotAquarang &&
		SaveDataHandler::saveData.ArchAttributeData.GotAquarang) {
		auto triggerCount = *(int*)(Core::moduleBase + 0x26DBD4);
		auto triggerAddr = *(uintptr_t*)(Core::moduleBase + 0x26DBD8);
		for (auto triggerIndex = 0; triggerIndex < triggerCount; triggerIndex++) {
			auto triggerId = *(int*)(triggerAddr + 0x14);
			if (triggerId == 2063) {
				memset((char*)(triggerAddr + 0x88), 0x0, 7);
				memset((char*)(triggerAddr + 0x85), 0x0, 1);
			}
			else if (triggerId == 113) {
				memset((char*)(triggerAddr + 0x88), 0x1, 7);
				memset((char*)(triggerAddr + 0x85), 0x1, 1);
			}
			triggerAddr = *(int*)(triggerAddr + 0x34);
		}
		auto scriptCount = *(int*)(Core::moduleBase + 0x26F480 + 0x44);
		auto scriptAddr = *(uintptr_t*)(Core::moduleBase + 0x26F480 + 0x48);
		for (auto scriptIndex = 0; scriptIndex < scriptCount; scriptIndex++) {
			auto scriptId = *(int*)(scriptAddr + 0x14);
			if (scriptId == 109) {
				memset((char*)(scriptAddr + 0x4E), 0x1, 1);
			}
			scriptAddr = *(int*)(scriptAddr + 0x34);
		}
		triggerCount = *(int*)(Core::moduleBase + 0x26DC20 + 0x44);
		triggerAddr = *(uintptr_t*)(Core::moduleBase + 0x26DC20 + 0x48);
		for (auto triggerIndex = 0; triggerIndex < triggerCount; triggerIndex++) {
			auto triggerId = *(int*)(triggerAddr + 0x14);
			if (triggerId == 1) {
				memset((char*)(triggerAddr + 0x88), 1, 7);
				memset((char*)(triggerAddr + 0x85), 1, 1);
			}
			triggerAddr = *(int*)(triggerAddr + 0x34);
		}
		auto aquaAddr = *(uintptr_t*)(Core::moduleBase + 0x267578 + 0x48);
		memset((char*)(aquaAddr + 0x60), 0x1, 1);
		aquaAddr = *(int*)(aquaAddr + 0x34);
		memset((char*)(aquaAddr + 0x60), 0x1, 1);
	}
}

void GameHandler::OnLoadRainbowCliffs() {
	auto levelId = Level::getCurrentLevel();
	if (levelId == LevelCode::Z1)
		OnEnterRainbowCliffs();
}

void GameHandler::OnEnterLevel() {	
	*(uintptr_t*)(Core::moduleBase + 0x2888D8) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);
	*(uintptr_t*)(Core::moduleBase + 0x288730) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);

	SaveDataHandler::SaveGame();

	ItemHandler::HandleStoredItems();
	LocationHandler::HandleStoredCheckedLocations();

	auto levelId = Level::getCurrentLevel();
	ArchipelagoHandler::SendLevel(static_cast<int>(levelId));

	if (levelId == LevelCode::A3)
		OnEnterShipRex();
	if (levelId == LevelCode::D2)
		OnEnterCrest();

	if (*(int*)(Core::moduleBase + 0x27041C) != 0
		&& SaveDataHandler::saveData.StopwatchesActive[(int)levelId] == 1)
		*(int*)(*(int*)(Core::moduleBase + 0x270420) + 0x68) = 0x2;
}

void GameHandler::OnMainMenu() {
	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	if (!ArchipelagoHandler::ap_connected) {
		*(char*)(menuAddr + 0x164) = 0x0;
		*(char*)(menuAddr + 0x165) = 0x0;
	}
	*(char*)(menuAddr + 0x168 + 0x164) = 0x0;
	*(char*)(menuAddr + 0x168 + 0x165) = 0x0;
}

void GameHandler::OnLoadGame() {
	ArchipelagoHandler::LoadSaveData();
	*(int*)(Core::moduleBase + 0x27383C) = 0;
	*(int*)(Core::moduleBase + 0x52F2BC) = Core::moduleBase + 0x273844;
	*(int*)(Core::moduleBase + 0x52F2A4) = 0x3;
	*(uintptr_t*)(Core::moduleBase + 0x2888D8) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);
	*(uintptr_t*)(Core::moduleBase + 0x288730) = reinterpret_cast<uintptr_t>(&SaveDataHandler::saveData);
	*(int*)(Core::moduleBase + 0x28DCA0) = 0x9;
}

void GameHandler::OnSpawnpointSet() {
	if (SaveDataHandler::saveData.CurrentLevel != LevelCode::Z1)
		return;
	auto prevLevel = static_cast<int>(SaveDataHandler::saveData.PreviousLevel);
	if (prevLevel == 4 || prevLevel == 5 || prevLevel == 6 ||
		prevLevel == 8 || prevLevel == 9 || prevLevel == 10 ||
		prevLevel == 12 || prevLevel == 13 || prevLevel == 14) {
		auto it = ArchipelagoHandler::inversePortalMap.find(prevLevel);
		if (it != ArchipelagoHandler::inversePortalMap.end()) {
			prevLevel = ArchipelagoHandler::inversePortalMap[prevLevel];
			if (spawnpointMap.find(prevLevel) != spawnpointMap.end()) {
				auto& posData = spawnpointMap.at(prevLevel);
				Vector3f posVector;
				posVector.x = posData[0];
				posVector.y = posData[1];
				posVector.z = posData[2];
				Hero::setPosition(posVector);
				*(float*)(Core::moduleBase + 0x271C20) = posData[3];
			}
		}
	}
}

void GameHandler::SetLoadActive(bool value) {
	if (!GameState::onLoadScreenOrMainMenu())
		return;
	auto menuAddr = *(int*)(Core::moduleBase + 0x286644);
	*(bool*)(menuAddr + 0x164) = value;
}

void GameHandler::OnDeath() {
	if (ArchipelagoHandler::someoneElseDied) {
		ArchipelagoHandler::someoneElseDied = false;
		return;
	}
	if (ArchipelagoHandler::deathlink)
		ArchipelagoHandler::SendDeath();
}