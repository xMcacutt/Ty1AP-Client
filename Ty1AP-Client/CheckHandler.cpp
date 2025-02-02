#include "CheckHandler.h"

typedef void(__stdcall* CollectTheggFunctionType)(void* ecx, int param);
CollectTheggFunctionType levelInitOrigin = nullptr;

void __stdcall CheckHandler::CollectTheggHook(void* ecx, int param) {
    // Capture eax before modification
	int theggIndex;
	__asm { 
		push eax
		mov eax, [esp + 8]
		mov theggIndex, eax
		pop eax
	}
	CheckHandler::OnCollectThegg(theggIndex);
	levelInitOrigin(ecx, param);
}

void CheckHandler::SetupHooks()
{
	auto addr = (char*)(Core::moduleBase + 0xF6E80);  // Target address
	MH_CreateHook((LPVOID)addr, &CollectTheggHook, reinterpret_cast<LPVOID*>(&levelInitOrigin));
}


void CheckHandler::OnCollectThegg(int theggIndex) {
	auto& ap = ArchipelagoHandler::get();
	if (theggIndex == 3 && ap.bilbysanity == Bilbysanity::ALL_NO_THEGG)
		return;
	int level = (int)Level::getCurrentLevel();
	level -= 4;
	level -= (level > 3) + (level > 7);
	ap.Check(0x8750100 + static_cast<int64_t>(level) * 0x8 + static_cast<int64_t>(theggIndex));
}