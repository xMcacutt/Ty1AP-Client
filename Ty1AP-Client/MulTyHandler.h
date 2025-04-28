#pragma once
#include <string>
#include "ArchipelagoHandler.h"

class MulTyHandler
{
public:
	static void InterpolateAndDraw();
	static void HandlePosData(int level, int index, std::vector<float> pos);
	static void DisableDraw(int index);
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
	
	static inline const std::map<LevelCode,std::vector<float>> defaultpositions{
		{ LevelCode::Z1, { 250.0f, 1700.0f, 6400.0f, 0.0f } },
		{ LevelCode::A1, { -2989.0f, -500.0f, 8238.0f, 0.0f } },
		{ LevelCode::A2, { -8940.0f, -2153.0f, 7162.0f, 0.0f } },
		{ LevelCode::A3, { -13646.0f, -338.0f, 22715.0f, 0.0f } },
		{ LevelCode::A4, { -572.0f, -1600.0f, -59.0f, 0.0f } },
		{ LevelCode::B1, { -3242.0f, -1309.0f, 6197.0f, 0.0f } },
		{ LevelCode::B2, { -518.0f, -4827.0f, 212.0f, 0.0f } },
		{ LevelCode::B3, { -14213.0f, 4000.0f, 16627.0f, 0.0f } },
		{ LevelCode::C1, { -4246.0f, -773.0f, 1343.0f, 0.0f } },
		{ LevelCode::C2, { -5499.0f, -2708.0f, -6951.0f, 0.0f } },
		{ LevelCode::C3, { -1615.0f, -1488.0f, 811.0f, 0.0f } },
		{ LevelCode::C4, { 90.0f, -789.0f, 93.0f, 0.0f } },
		{ LevelCode::D2, { -166.0f, -100.0f, -7041.0f, 0.0f } },
		{ LevelCode::D3, { -2989.0f, -500.0f, 8238.0f, 0.0f } },
		{ LevelCode::D4, { -192.0f, -3000.0f, 3264.0f, 0.0f } },
		{ LevelCode::E1, { -8845.0f, 1000.0f, 17487.0f, 0.0f } },
		{ LevelCode::E2, { -82.0f, -1524.0f, 449.0f, 0.0f } },
		{ LevelCode::E3, { -82.0f, -1524.0f, 449.0f, 0.0f } },
		{ LevelCode::E4, { 10.0f, -200.0f, -250.0f, 0.0f } }
	};

	static float Lerp(float a, float b, float t);
	static float LerpAngle(float a, float b, float t);
	static uint64_t GetTimeMS();
	static float DistanceSquared(const std::vector<float>& a, const std::vector<float>& b);
	static void RemoveCollision();
	static void ScaleKoalas();

};

