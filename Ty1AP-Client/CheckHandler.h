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

class CheckHandler
{
public:
	static void __stdcall CollectTheggHook();
    static void __stdcall CollectCogHook();
    static void __stdcall CollectBilbyHook();
    static void __stdcall CollectFrameHook();
	static void SetupHooks();
	static void OnCollectThegg(int theggIndex);
    static void OnCollectCog(int cogIndex);
    static void OnCollectBilby(int bilbyIndex);
    static void OnCollectFrame(int frameIndex);
};

