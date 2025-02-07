#pragma once
#include "apclient.hpp"
#include "gamestate.h"
#include "ArchipelagoHandler.h"
#include "SaveDataHandler.h"
#include <queue>
#include "sound.h"

class ItemHandler
{
public:
	static void HandleItem(APClient::NetworkItem item);
	static void HandleStoredItems();
	static void HandleProgressiveLevel();
	static void HandleIndividualLevel(int code);
	static void HandleProgressiveRang();
	static void HandleIndividualRang(int code);
	static std::queue<APClient::NetworkItem> storedItems;
private: 
	static bool CheckCompletion();
};

