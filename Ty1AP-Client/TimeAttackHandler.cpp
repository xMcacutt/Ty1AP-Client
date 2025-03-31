#include "pch.h"
#include "TimeAttackHandler.h"

std::map<LevelCode, int> TimeAttackHandler::stdTimes = {
	{LevelCode::A1, 3000},
	{LevelCode::A2, 4000},
	{LevelCode::A3, 10700},
	{LevelCode::B1, 11200},
	{LevelCode::B2, 12400},
	{LevelCode::B3, 14500},
	{LevelCode::C1, 2900},
	{LevelCode::C2, 8500},
	{LevelCode::C3, 9500},
};

std::map<LevelCode, int> TimeAttackHandler::advTimes = {
	{LevelCode::A1, 2700},
	{LevelCode::A2, 3850},
	{LevelCode::A3, 10200},
	{LevelCode::B1, 7100}, 
	{LevelCode::B2, 11300},
	{LevelCode::B3, 9900},
	{LevelCode::C1, 2700},
	{LevelCode::C2, 8200},
	{LevelCode::C3, 7950},
};

typedef void(__stdcall* StartTimeAttackFunctionType)();
StartTimeAttackFunctionType startTimeAttackOrigin = nullptr;

typedef void(__stdcall* EndTimeAttackFunctionType)();
EndTimeAttackFunctionType endTimeAttackOrigin = nullptr;

uintptr_t startTimeAttackOriginAddr;
uintptr_t hideStopwatchAddr;
__declspec(naked) void __stdcall TimeAttackHandler::StartTimeAttackHook() {
	__asm {
		call dword ptr[hideStopwatchAddr]
		pushfd
		pushad
		call TimeAttackHandler::OnStartTimeAttack
		popad
		popfd
		jmp dword ptr[startTimeAttackOriginAddr]
	}
}

uintptr_t endTimeAttackOriginAddr;
__declspec(naked) void __stdcall TimeAttackHandler::EndTimeAttackHook() {
	__asm {
		pushfd
		pushad
		call TimeAttackHandler::OnEndTimeAttack
		popad
		popfd
		mov byte ptr[ecx+0x1],0x1
		pop esi
		jmp dword ptr[endTimeAttackOriginAddr]
	}
}


void TimeAttackHandler::SetupHooks() {
	startTimeAttackOriginAddr = Core::moduleBase + 0x1599C2;
	hideStopwatchAddr = Core::moduleBase + 0x15A070;
	auto addr = (char*)(Core::moduleBase + 0x1599BD);
	MH_CreateHook((LPVOID)addr, &StartTimeAttackHook, reinterpret_cast<LPVOID*>(&startTimeAttackOrigin));

	endTimeAttackOriginAddr = Core::moduleBase + 0xF823A;
	addr = (char*)(Core::moduleBase + 0xF8235);
	MH_CreateHook((LPVOID)addr, &EndTimeAttackHook, reinterpret_cast<LPVOID*>(&endTimeAttackOrigin));
}

void TimeAttackHandler::OnStartTimeAttack() {
	if (!ArchipelagoHandler::gateTimeAttacks)
		return;
	auto level = Level::getCurrentLevel();
	if (SaveDataHandler::saveData.LevelData[static_cast<int>(level)].ThunderEggs[2]) {
		int time = ArchipelagoHandler::advancedLogic ? advTimes[level] : stdTimes[level];
		*(int*)(Core::moduleBase + 0x28CB58) = time;
	}
}

void TimeAttackHandler::OnEndTimeAttack() {
	if (!ArchipelagoHandler::gateTimeAttacks)
		return;
	auto level = Level::getCurrentLevel();
	auto timeAchieved = SaveDataHandler::saveData.LevelData[static_cast<int>(level)].TimeAttackLastSeconds;
	auto timeToBeat = ArchipelagoHandler::advancedLogic ? advTimes[level] : stdTimes[level];
	if (timeAchieved < timeToBeat)
		ArchipelagoHandler::Check(0x8750340 + static_cast<int64_t>(level));
}