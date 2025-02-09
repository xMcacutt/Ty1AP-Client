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
		push edx
		push esi
		push eax
		call CheckHandler::OnCollectCog
		pop eax
		pop esi
		pop edx
		mov byte ptr[edx + eax + 0x40], 0x1
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
		mov edx, [edi + 0x2C]
		mov ecx, edi
		push ebx
		push ecx
		push edx
		push edi
		push esi
		push eax
		call CheckHandler::OnCollectTalisman
		pop eax
		pop esi
		pop edi
		pop edx
		pop ecx
		pop ebx
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
		mov[esi + eax + 0xAB7], dl
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

void CheckHandler::SetupHooks()
{
	collectBilbyOriginAddr = Core::moduleBase + 0xF7AF7 + 5;
	collectFrameOriginAddr = Core::moduleBase + 0xF7F00 + 5;
	collectFrameOrigin2Addr = Core::moduleBase + 0xF7F72 + 5;
	collectTalismanOriginAddr = Core::moduleBase + 0xF7CCE;
	collectDiveOriginAddr = Core::moduleBase + 0xF807F;
	collectSwimOriginAddr = Core::moduleBase + 0xF80A3;
	collectSecondRangOriginAddr = Core::moduleBase + 0xF7D0F;
	collectRangOriginAddr = Core::moduleBase + 0xF8064;
	collectLifePawOriginAddr = Core::moduleBase + 0xF7ED7;

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

	addr = (char*)(Core::moduleBase + 0xF7CC6);
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

}

void CheckHandler::OnCollectThegg(int theggIndex) {
	if (theggIndex == 1 && ArchipelagoHandler::bilbysanity == Bilbysanity::ALL_NO_THEGG)
		return;
	int level = (int)Level::getCurrentLevel();
	level -= 4;
	level -= (level > 3) + (level > 7);
	ArchipelagoHandler::Check(0x8750100 + static_cast<int64_t>(level) * 0x8 + static_cast<int64_t>(theggIndex));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectCog(int cogIndex) {
	if (ArchipelagoHandler::cogsanity == Cogsanity::NONE) {
		SaveDataHandler::SaveGame();
		return;
	}
	int level = (int)Level::getCurrentLevel();
	auto adjustedLevel = level - 4;
	adjustedLevel -= (adjustedLevel > 3) + (adjustedLevel > 7);
	if (ArchipelagoHandler::cogsanity == Cogsanity::PER_LEVEL) {
		auto levelCogCount = std::count(
			SaveDataHandler::saveData.LevelData[level].GoldenCogs, 
			SaveDataHandler::saveData.LevelData[level].GoldenCogs + 10, true
		);
		if (levelCogCount == 10) 
			ArchipelagoHandler::Check(0x87501A2 + static_cast<int64_t>(adjustedLevel));
	}
	else
		ArchipelagoHandler::Check(0x8750148 + static_cast<int64_t>(adjustedLevel) * 0xA + static_cast<int64_t>(cogIndex));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectBilby(int bilbyIndex) {
	if (ArchipelagoHandler::bilbysanity == Bilbysanity::NONE) {
		SaveDataHandler::SaveGame();
		return;
	}
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
	adjustedLevel -= (adjustedLevel > 3) + (adjustedLevel > 7);
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
	if (talismanIndex == 4 && ArchipelagoHandler::goal == Goal::BEAT_CASS)
		ArchipelagoHandler::Release();
	else if (ArchipelagoHandler::goal == Goal::ALL_BOSSES &&
		std::count(SaveDataHandler::saveData.TalismansPlaced, 
			SaveDataHandler::saveData.TalismansPlaced + 5, true) == 5) 
		ArchipelagoHandler::Release();
	else
		ArchipelagoHandler::Check(0x8750261 + static_cast<int64_t>(talismanIndex));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectDive() {
	if (ArchipelagoHandler::attributesanity == Attributesanity::NONE)
		return;
	if (Level::getCurrentLevel() != LevelCode::B1)
		return;
	ArchipelagoHandler::Check(0x08750311);
}

void CheckHandler::OnCollectSwim() {
	if (ArchipelagoHandler::attributesanity == Attributesanity::NONE)
		return;
	if (Level::getCurrentLevel() != LevelCode::A3)
		return;
	ArchipelagoHandler::Check(0x08750310);
}

void CheckHandler::OnCollectSecondRang() {
	if (ArchipelagoHandler::attributesanity == Attributesanity::NONE)
		return;
	ArchipelagoHandler::Check(0x08750312);
}

void CheckHandler::OnCollectLifePaw() {
	if (ArchipelagoHandler::attributesanity == Attributesanity::NONE)
		return;
	ArchipelagoHandler::Check(0x08750313);
}

void CheckHandler::OnCollectRang(int rangId) {
	if (ArchipelagoHandler::attributesanity == Attributesanity::NONE)
		return;
	if (ArchipelagoHandler::attributesanity == Attributesanity::SKIP_ELEMENTALS)
		if (rangId == 1 || rangId == 2 || rangId == 8) 
			return;
	ArchipelagoHandler::Check(0x08750314 + static_cast<int64_t>(rangId));
}
