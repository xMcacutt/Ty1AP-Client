#pragma once
#include "apclient.hpp"
#include "gamestate.h"
#include "SaveDataHandler.h"
#include <queue>

class ItemHandler
{
public:
	static void HandleItem(APClient::NetworkItem item);
	static void HandleStoredItems();
	static std::queue<APClient::NetworkItem> storedItems;
};

