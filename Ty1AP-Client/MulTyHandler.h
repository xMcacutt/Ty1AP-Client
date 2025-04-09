#pragma once
#include <string>
#include "ArchipelagoHandler.h"

class MulTyHandler
{
public:
	static void HandlePosData(int level, std::string source, std::vector<float> pos);
	static void TryRemove(std::string source);
	static void Run();
	static bool IsRunning;
private:
	static std::map<int, std::vector<float>> posData;
	static std::map<std::string, int> koalaData;
};

