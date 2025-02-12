#pragma once
#include "ArchipelagoHandler.h"
#include "GameHandler.h"
#include <algorithm>

const std::unordered_map<int, std::string> LEVEL_NAMES = {
    {0, "Rainbow Cliffs"},
    {4, "Two Up"},
    {5, "WitP"},
    {6, "Ship Rex"},
    {7, "Bull's Pen"},
    {8, "BotRT"},
    {9, "Snow Worries"},
    {10, "Outback Safari"},
    {12, "LLPoF"},
    {13, "BtBS"},
    {14, "RMtS"},
    {15, "Fluffy's Fjord"},
    {17, "Cass' Crest"},
    {19, "Crikey's Cove"},
    {20, "Cass' Pass"},
    {21, "Bonus World 1"},
    {22, "Bonus World 2"},
    {23, "Final Battle"}
};

const std::unordered_map<int, int> FRAME_COUNTS = {
    {0, 9},
    {4, 7},
    {5, 6},
    {6, 9},
    {8, 20},
    {9, 24},
    {12, 5},
    {13, 29},
    {14, 18}
};

const std::unordered_map<int, int> RUNNING_FRAME_COUNTS = {
    {0, 0},
    {4, 9},
    {5, 16},
    {6, 22},
    {8, 31},
    {9, 51},
    {12, 75},
    {13, 80},
    {14, 109}
};

class CheckHandler
{
public:
	static void __stdcall CollectTheggHook();
    static void __stdcall CollectCogHook();
    static void __stdcall CollectBilbyHook();
    static void __stdcall CollectFrameHook();
    static void __stdcall CollectFrameHook2();
    static void __stdcall CollectTalismanHook();
    static void __stdcall CollectDiveHook();
    static void __stdcall CollectSwimHook();
    static void __stdcall CollectSecondRangHook();
    static void __stdcall CollectLifePawHook();
    static void __stdcall CollectRangHook();
    static void __stdcall CollectOpalHook();
	static void SetupHooks();
	static void OnCollectThegg(int theggIndex);
    static void OnCollectCog(int cogIndex);
    static void OnCollectBilby(int bilbyIndex);
    static void OnCollectFrame(int frameIndex);
    static void OnCollectTalisman(int talismanIndex);
    static void OnCollectDive();
    static void OnCollectSwim();
    static void OnCollectSecondRang();
    static void OnCollectLifePaw();
    static void OnCollectRang(int rangId);
    static void OnCollectOpal(uintptr_t opalPtr);
};

