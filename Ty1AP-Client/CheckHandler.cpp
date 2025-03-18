#include "CheckHandler.h"

typedef void(__stdcall* CollectTheggFunctionType)();
CollectTheggFunctionType collectTheggOrigin = nullptr;

typedef void(__stdcall* CollectCogFunctionType)();
CollectCogFunctionType collectCogOrigin = nullptr;

typedef void(__stdcall* CollectBilbyFunctionType)();
CollectBilbyFunctionType collectBilbyOrigin = nullptr;

typedef void(__stdcall* CollectFrameFunctionType)();
CollectFrameFunctionType collectFrameOrigin = nullptr;

typedef void(__stdcall* CollectFrame2FunctionType)();
CollectFrame2FunctionType collectFrameOrigin2 = nullptr;

typedef void(__stdcall* CollectTalismanFunctionType)();
CollectTalismanFunctionType collectTalismanOrigin = nullptr;

typedef void(__stdcall* CollectDiveFunctionType)();
CollectDiveFunctionType collectDiveOrigin = nullptr;

typedef void(__stdcall* CollectSwimFunctionType)();
CollectSwimFunctionType collectSwimOrigin = nullptr;

typedef void(__stdcall* CollectSecondRangFunctionType)();
CollectSecondRangFunctionType collectSecondRangOrigin = nullptr;

typedef void(__stdcall* CollectRangFunctionType)();
CollectRangFunctionType collectRangOrigin = nullptr;

typedef void(__stdcall* CollectLifePawFunctionType)();
CollectLifePawFunctionType collectLifePawOrigin = nullptr;

typedef void(__stdcall* CollectOpalFunctionType)();
CollectOpalFunctionType collectOpalOrigin = nullptr;

typedef void(__stdcall* HitSignFunctionType)();
HitSignFunctionType hitSignOrigin = nullptr;

typedef void(__stdcall* PickupLifeFunctionType)();
PickupLifeFunctionType pickupLifeOrigin = nullptr;

__declspec(naked) void __stdcall CheckHandler::CollectTheggHook() {
	__asm {
		push ebp
		mov ebp, esp
		mov eax, [ecx + 0x2C]
		imul edx, [eax + 0xAA4], 0x70
		add edx, eax
		mov eax, [ebp + 0x8]
		push edx
		push ecx
		push eax
		call CheckHandler::OnCollectThegg
		pop eax
		pop ecx
		pop edx
		mov byte ptr[edx + eax + 0x38], 0x1
		mov byte ptr[ecx + 0x1], 0x1
		mov esp, ebp
		pop ebp
		ret 4
	}
}

__declspec(naked) void __stdcall CheckHandler::CollectCogHook() {
	__asm {
		push ebp
		mov ebp, esp
		push esi
		mov esi, ecx
		mov eax, [esi + 0x2C]
		imul edx, [eax + 0xAA4], 0x70
		add edx, eax
		mov eax, [ebp + 0x8]
		mov byte ptr[edx + eax + 0x40], 0x1
		push edx
		push esi
		push eax
		call CheckHandler::OnCollectCog
		pop eax
		pop esi
		pop edx
		mov byte ptr[esi + 0x1], 0x1
		mov ecx, esi
		mov byte ptr[esi + 0x1], 0x1
		pop esi
		pop ebp
		ret 4
	}
}

uintptr_t collectBilbyOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectBilbyHook() {
	__asm {
		push eax
		push ecx
		push edx
		push esi
		push edi
		push ebx
		call CheckHandler::OnCollectBilby
		pop ebx
		pop edi
		pop esi
		pop edx
		pop ecx
		pop eax
		or byte ptr[edx + ebx + 0x4A], 0x1
		jmp dword ptr[collectBilbyOriginAddr]
	}
}

uintptr_t collectFrameOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectFrameHook()
{
	__asm {
		push ebp
		mov ebp, esp
		push ebx
		push ecx
		push edx
		push esi
		push edi
		push eax
		call CheckHandler::OnCollectFrame
		pop eax
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		push -0x1
		jmp dword ptr[collectFrameOriginAddr]
	}
}

uintptr_t collectFrameOrigin2Addr;
__declspec(naked) void __stdcall CheckHandler::CollectFrameHook2()
{
	__asm {
		push ebx
		push ecx
		push edx
		push esi
		push edi
		push eax
		call SaveDataHandler::SaveGame
		pop eax
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		sar eax,0x3
		rol bl,0x1
		jmp dword ptr[collectFrameOrigin2Addr]
	}
}

uintptr_t collectTalismanOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectTalismanHook() {
	__asm {
		mov byte ptr[eax+ecx+0xAC9],01
		push eax
		call CheckHandler::OnCollectTalisman
		pop eax
		jmp dword ptr[collectTalismanOriginAddr]
	}
}

uintptr_t collectDiveOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectDiveHook() {
	__asm {
		mov[edx + 0xAB5], al
		push eax
		push ebx
		push ecx
		push edx
		push edi
		push esi
		call CheckHandler::OnCollectDive
		pop esi
		pop edi
		pop edx
		pop ecx
		pop ebx
		pop eax
		jmp dword ptr[collectDiveOriginAddr]
	}
}

uintptr_t collectSwimOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectSwimHook() {
	__asm {
		mov[edx + 0xAB4], al
		push eax
		push ebx
		push ecx
		push edx
		push edi
		push esi
		call CheckHandler::OnCollectSwim
		pop esi
		pop edi
		pop edx
		pop ecx
		pop ebx
		pop eax
		jmp dword ptr[collectSwimOriginAddr]
	}
}

uintptr_t collectSecondRangOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectSecondRangHook() {
	__asm {
		mov[edx + 0xAB6], al
		push eax
		push ebx
		push ecx
		push edx
		push edi
		push esi
		call CheckHandler::OnCollectSecondRang
		pop esi
		pop edi
		pop edx
		pop ecx
		pop ebx
		pop eax
		jmp dword ptr[collectSecondRangOriginAddr]
	}
}

uintptr_t collectRangOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectRangHook() {
	__asm {
		mov [esi+eax+0xAB8],dl
		push ebx
		push ecx
		push edx
		push edi
		push esi
		push eax
		call CheckHandler::OnCollectRang
		pop eax
		pop esi
		pop edi
		pop edx
		pop ecx
		pop ebx
		jmp dword ptr[collectRangOriginAddr]
	}
}

uintptr_t collectLifePawOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectLifePawHook() {
	__asm {
		mov[edx + 0xAB7], dl
		push ebx
		push ecx
		push edx
		push edi
		push esi
		push eax
		call CheckHandler::OnCollectLifePaw
		pop eax
		pop esi
		pop edi
		pop edx
		pop ecx
		pop ebx
		jmp dword ptr[collectLifePawOriginAddr]
	}
}

uintptr_t collectOpalOriginAddr;
__declspec(naked) void __stdcall CheckHandler::CollectOpalHook() {
	__asm {
		mov[esi + 0x78], 0x4
		push ebx
		push ecx
		push edx
		push edi
		push eax
		push esi
		call CheckHandler::OnCollectOpal
		pop esi
		pop eax
		pop edi
		pop edx
		pop ecx
		pop ebx
		jmp dword ptr[collectOpalOriginAddr]
	}
}

uintptr_t hitSignOriginAddr;
__declspec(naked) void __stdcall CheckHandler::HitSignHook() {
	__asm {
		mov[ecx + 0xA0], 0x2
		push ebx
		push edx
		push edi
		push eax
		push esi
		push ecx
		call CheckHandler::OnHitSign
		pop ecx
		pop esi
		pop eax
		pop edi
		pop edx
		pop ebx
		jmp dword ptr[hitSignOriginAddr]
	}
}

uintptr_t pickupLifeOriginAddr;
__declspec(naked) void __stdcall CheckHandler::PickupLifeHook() {
	__asm {
		push ebx
		push edx
		push edi
		push eax
		push ecx
		push esi
		call CheckHandler::OnPickupLife
		pop esi
		pop ecx
		pop eax
		pop edi
		pop edx
		pop ebx
		cmp dword ptr[esi + 0x110], 0x4
		jmp dword ptr[pickupLifeOriginAddr]
	}
}

void CheckHandler::SetupHooks()
{
	collectBilbyOriginAddr = Core::moduleBase + 0xF7AF7 + 5;
	collectFrameOriginAddr = Core::moduleBase + 0xF7F00 + 5;
	collectFrameOrigin2Addr = Core::moduleBase + 0xF7F72 + 5;
	collectTalismanOriginAddr = Core::moduleBase + 0xF7CF7;
	collectDiveOriginAddr = Core::moduleBase + 0xF807F;
	collectSwimOriginAddr = Core::moduleBase + 0xF80A3;
	collectSecondRangOriginAddr = Core::moduleBase + 0xF7D0F;
	collectRangOriginAddr = Core::moduleBase + 0xF8064;
	collectLifePawOriginAddr = Core::moduleBase + 0xF7ED7;
	collectOpalOriginAddr = Core::moduleBase + 0x12DD72;
	hitSignOriginAddr = Core::moduleBase + 0x151EE4;
	pickupLifeOriginAddr = Core::moduleBase + 0x11FB21;


	auto addr = (char*)(Core::moduleBase + 0xF6E80);
	MH_CreateHook((LPVOID)addr, &CollectTheggHook, reinterpret_cast<LPVOID*>(&collectTheggOrigin));

	addr = (char*)(Core::moduleBase + 0xF6CC0);
	MH_CreateHook((LPVOID)addr, &CollectCogHook, reinterpret_cast<LPVOID*>(&collectCogOrigin));

	addr = (char*)(Core::moduleBase + 0xF7AF7);
	MH_CreateHook((LPVOID)addr, &CollectBilbyHook, reinterpret_cast<LPVOID*>(&collectBilbyOrigin));

	addr = (char*)(Core::moduleBase + 0xF7F00);
	MH_CreateHook((LPVOID)addr, &CollectFrameHook, reinterpret_cast<LPVOID*>(&collectFrameOrigin));

	addr = (char*)(Core::moduleBase + 0xF7F72);
	MH_CreateHook((LPVOID)addr, &CollectFrameHook2, reinterpret_cast<LPVOID*>(&collectFrameOrigin2));

	addr = (char*)(Core::moduleBase + 0xF7CEF);
	MH_CreateHook((LPVOID)addr, &CollectTalismanHook, reinterpret_cast<LPVOID*>(&collectTalismanOrigin));

	addr = (char*)(Core::moduleBase + 0xF8079);
	MH_CreateHook((LPVOID)addr, &CollectDiveHook, reinterpret_cast<LPVOID*>(&collectDiveOrigin));

	addr = (char*)(Core::moduleBase + 0xF8099);
	MH_CreateHook((LPVOID)addr, &CollectSwimHook, reinterpret_cast<LPVOID*>(&collectSwimOrigin));

	addr = (char*)(Core::moduleBase + 0xF7D09);
	MH_CreateHook((LPVOID)addr, &CollectSecondRangHook, reinterpret_cast<LPVOID*>(&collectSecondRangOrigin));

	addr = (char*)(Core::moduleBase + 0xF7ED1);
	MH_CreateHook((LPVOID)addr, &CollectLifePawHook, reinterpret_cast<LPVOID*>(&collectLifePawOrigin));

	addr = (char*)(Core::moduleBase + 0xF805D);
	MH_CreateHook((LPVOID)addr, &CollectRangHook, reinterpret_cast<LPVOID*>(&collectRangOrigin));

	addr = (char*)(Core::moduleBase + 0x12DD6B);
	MH_CreateHook((LPVOID)addr, &CollectOpalHook, reinterpret_cast<LPVOID*>(&collectOpalOrigin));

	addr = (char*)(Core::moduleBase + 0x151EDA);
	MH_CreateHook((LPVOID)addr, &HitSignHook, reinterpret_cast<LPVOID*>(&hitSignOrigin));

	addr = (char*)(Core::moduleBase + 0x11FB1A);
	MH_CreateHook((LPVOID)addr, &PickupLifeHook, reinterpret_cast<LPVOID*>(&pickupLifeOrigin));
}

void CheckHandler::OnCollectThegg(int theggIndex) {
	int level = (int)Level::getCurrentLevel();
	auto adjustedLevel = level - 4;
	adjustedLevel -= (adjustedLevel > 3) + (adjustedLevel > 7);
	ArchipelagoHandler::Check(0x8750100 + static_cast<int64_t>(adjustedLevel) * 0x8 + static_cast<int64_t>(theggIndex));
	if (theggIndex == 3 && !ArchipelagoHandler::gateTimeAttacks) {
		SaveDataHandler::saveData.StopwatchesActive[level] = true;
		if (*(int*)(Core::moduleBase + 0x27041C) != 0)
			*(int*)(*(int*)(Core::moduleBase + 0x270420) + 0x68) = 0x2;
	}
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectCog(int cogIndex) {
	int level = (int)Level::getCurrentLevel();
	auto adjustedLevel = level - 4;
	adjustedLevel -= (adjustedLevel > 3) + (adjustedLevel > 7);
	ArchipelagoHandler::Check(0x8750148 + static_cast<int64_t>(adjustedLevel) * 0xA + static_cast<int64_t>(cogIndex));
	auto levelCogCount = std::count(
		SaveDataHandler::saveData.LevelData[level].GoldenCogs,
		SaveDataHandler::saveData.LevelData[level].GoldenCogs + 10, true
	);
	API::LogPluginMessage(std::to_string(level) + " " + std::to_string(levelCogCount));
	if (levelCogCount == 10) 
		ArchipelagoHandler::Check(0x87501A2 + static_cast<int64_t>(adjustedLevel));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectBilby(int bilbyIndex) {
	int level = (int)Level::getCurrentLevel();
	auto adjustedLevel = level - 4;
	adjustedLevel -= (adjustedLevel > 3) + (adjustedLevel > 7);
	ArchipelagoHandler::Check(0x87501AC + static_cast<int64_t>(adjustedLevel) * 0x5 + static_cast<int64_t>(bilbyIndex));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectFrame(int frameIndex)
{
	if (ArchipelagoHandler::framesanity == Framesanity::NONE) {
		SaveDataHandler::SaveGame();
		return;
	}
	int level = (int)Level::getCurrentLevel();
	SaveDataHandler::saveData.FrameCount[level]++;
	auto adjustedLevel = level - (3 + (level > 7) + (level > 9) + (level > 11));
	if (adjustedLevel == -3)
		adjustedLevel = 0;
	auto levelFrameCount = SaveDataHandler::saveData.FrameCount[level];
	if (ArchipelagoHandler::framesanity == Framesanity::PER_LEVEL) {
		if (levelFrameCount == FRAME_COUNTS.find(level)->second)
			ArchipelagoHandler::Check(0x8750258 + static_cast<int64_t>(adjustedLevel));
	}
	else
		ArchipelagoHandler::Check(0x87501D9 + static_cast<int64_t>(frameIndex));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectTalisman(int talismanIndex) {
	ArchipelagoHandler::Check(0x8750261 + static_cast<int64_t>(talismanIndex));
	bool canGoE4 = ((SaveDataHandler::saveData.TalismansPlaced[0]
		&& SaveDataHandler::saveData.TalismansPlaced[1]
		&& SaveDataHandler::saveData.TalismansPlaced[2]
		&& SaveDataHandler::saveData.TalismansPlaced[3]) && ArchipelagoHandler::goalReqBosses)
		|| (!ArchipelagoHandler::goalReqBosses && SaveDataHandler::saveData.TalismansPlaced[3]);
	if (talismanIndex == 3) {
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
	if (talismanIndex == 4 && ArchipelagoHandler::goal == Goal::BEAT_CASS)
		ArchipelagoHandler::Release();
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectDive() {
	if (Level::getCurrentLevel() != LevelCode::B1)
		return;
	ArchipelagoHandler::Check(0x08750311);
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectSwim() {
	if (Level::getCurrentLevel() != LevelCode::A3)
		return;
	ArchipelagoHandler::Check(0x08750310);
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectSecondRang() {
	ArchipelagoHandler::Check(0x08750312);
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectLifePaw() {
	if (!ArchipelagoHandler::scalesanity)
		return;
	ArchipelagoHandler::Check(0x08750313);
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectRang(int rangId) {
	ArchipelagoHandler::Check(0x08750314 + static_cast<int64_t>(rangId));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectOpal(uintptr_t opalPtr) {
	if (Level::getCurrentLevel() != LevelCode::Z1 || !ArchipelagoHandler::scalesanity) 
		return;
	uintptr_t baseOpalAddr = *(uintptr_t*)(Core::moduleBase + 0x269818 + 0x48);
	int opalIndex = (opalPtr - baseOpalAddr) / 0x114;
	auto byteIndex = opalIndex / 8;
	auto bitIndex = opalIndex % 8;
	auto b = SaveDataHandler::saveData.LevelData[(int)LevelCode::Z1].Opals[byteIndex];
	b |= 1 << bitIndex;
	SaveDataHandler::saveData.LevelData[(int)LevelCode::Z1].Opals[byteIndex] = b;
	ArchipelagoHandler::Check(0x8750320 + static_cast<int64_t>(opalIndex));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnHitSign(uintptr_t signPtr) {
	if (!ArchipelagoHandler::signsanity)
		return;
	LevelCode level = Level::getCurrentLevel();
	uintptr_t baseSignAddr = *(uintptr_t*)(Core::moduleBase + 0x26FA90 + 0x48);
	int levelSignIndex = (signPtr - baseSignAddr) / 0xB4;
	if (RUNNING_SIGN_COUNTS.find((int)level) == RUNNING_SIGN_COUNTS.end())
		return;
	int signIndex = RUNNING_SIGN_COUNTS.at((int)level) + levelSignIndex;
	ArchipelagoHandler::Check(0x8750400 + static_cast<int64_t>(signIndex));
}

void CheckHandler::OnPickupLife(uintptr_t lifePtr) {
	if (!ArchipelagoHandler::lifesanity)
		return;
	API::LogPluginMessage(std::to_string(*(uintptr_t*)(lifePtr + 0x4)));
	if (*(uintptr_t*)(lifePtr + 0x4) != Core::moduleBase + 0x2679D4)
		return;
	LevelCode level = Level::getCurrentLevel();
	if (RUNNING_LIFE_COUNTS.find((int)level) == RUNNING_SIGN_COUNTS.end())
		return;
	uintptr_t baseLifeAddr = *(uintptr_t*)(Core::moduleBase + 0x267A00 + 0x48);
	int levelLifeIndex = (lifePtr - baseLifeAddr) / 0x12C;
	if (levelLifeIndex < 0 || levelLifeIndex > 4)
		return;
	int lifeIndex = RUNNING_LIFE_COUNTS.at((int)level) + levelLifeIndex;
	ArchipelagoHandler::Check(0x8750420 + static_cast<int64_t>(lifeIndex));
}
