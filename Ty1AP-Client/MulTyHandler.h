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
	struct TimedPos {          
		int level = -1;                
		std::vector<float> lastPos;       
		std::vector<float> newPos;        
		uint64_t lastTime = 0;            
		uint64_t newTime = 0;             
	};
	static std::map<int, TimedPos> posData;
	static void InterpolateAndDraw();
	static float Lerp(float a, float b, float t);
	static float LerpAngle(float a, float b, float t);
	static uint64_t GetTimeMS();
	static float DistanceSquared(const std::vector<float>& a, const std::vector<float>& b);
};

