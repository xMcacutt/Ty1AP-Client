#pragma once
#include <string>
#include "ArchipelagoHandler.h"

class MulTyHandler
{
public:
	static void HandlePosData(int level, int index, std::vector<float> pos);
	static void TryRemove(int index);
	static void Run();
	static bool IsRunning;
private:
	static std::map<int, std::vector<float>> posData;
};

