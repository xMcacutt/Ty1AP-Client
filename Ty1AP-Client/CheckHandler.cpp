#include "CheckHandler.h"

typedef void(__stdcall* CollectTheggFunctionType)();
CollectTheggFunctionType collectTheggOrigin = nullptr;

typedef void(__stdcall* CollectCogFunctionType)();
CollectCogFunctionType collectCogOrigin = nullptr;

typedef void(__stdcall* CollectBilbyFunctionType)();
CollectBilbyFunctionType collectBilbyOrigin = nullptr;

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




void CheckHandler::SetupHooks()
{
	collectBilbyOriginAddr = Core::moduleBase + 0xF7AF7 + 5;
	auto addr = (char*)(Core::moduleBase + 0xF6E80);  // Target address
	MH_CreateHook((LPVOID)addr, &CollectTheggHook, reinterpret_cast<LPVOID*>(&collectTheggOrigin));
	addr = (char*)(Core::moduleBase + 0xF6CC0);  // Target address
	MH_CreateHook((LPVOID)addr, &CollectCogHook, reinterpret_cast<LPVOID*>(&collectCogOrigin));
	addr = (char*)(Core::moduleBase + 0xF7AF7);  // Target address
	MH_CreateHook((LPVOID)addr, &CollectBilbyHook, reinterpret_cast<LPVOID*>(&collectBilbyOrigin));
}

void CheckHandler::OnCollectThegg(int theggIndex) {
	if (theggIndex == 3 && ArchipelagoHandler::bilbysanity == Bilbysanity::ALL_NO_THEGG)
		return;
	int level = (int)Level::getCurrentLevel();
	level -= 4;
	level -= (level > 3) + (level > 7);
	ArchipelagoHandler::Check(0x8750100 + static_cast<int64_t>(level) * 0x8 + static_cast<int64_t>(theggIndex));
	SaveDataHandler::SaveGame();
}

void CheckHandler::OnCollectCog(int cogIndex) {
	if (ArchipelagoHandler::cogsanity == Cogsanity::NONE){
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