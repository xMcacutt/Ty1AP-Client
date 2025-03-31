#pragma once
#include "CheckHandler.h"
#include "SaveDataHandler.h"
#include <cstdint>
#include <queue>
#include "gamestate.h"
#include <unordered_map>

class LocationHandler
{
public:
	static void HandleLocation(int64_t item);
	static void HandleStoredCheckedLocations();
private:
	static inline std::queue<int64_t> storedCheckedLocations;
	static std::unordered_map<int, int> crateOpalCounts;
}; 