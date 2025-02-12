#pragma once
#include "SaveDataHandler.h"
#include <map>
#include "core.h"
#include "level.h"
#include "MinHook.h"

class TimeAttackHandler
{
public:
	static std::map<LevelCode, int> stdTimes;
	static std::map<LevelCode, int> advTimes;
	static void SetupHooks();
private:
	static void __stdcall StartTimeAttackHook();
	static void __stdcall EndTimeAttackHook();
	static void OnStartTimeAttack();
	static void OnEndTimeAttack();
};

