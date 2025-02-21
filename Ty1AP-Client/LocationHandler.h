#pragma once
#include "CheckHandler.h"
#include "SaveDataHandler.h"
#include <cstdint>
#include <queue>
#include "gamestate.h"

class LocationHandler
{
public:
	static void HandleLocation(int64_t item);
	static void HandleStoredCheckedLocations();
private:
	static inline std::queue<int64_t> storedCheckedLocations;
};

