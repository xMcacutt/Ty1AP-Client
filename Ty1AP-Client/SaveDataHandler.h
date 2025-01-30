#pragma once
#include "LoggerWindow.h"
#include "savedata.h"
#include <iostream>
#include <fstream>
#include <string>

struct ExtendedSaveData : public SaveDataStruct {
	bool StopwatchesActive[24];
	int FireThunderEggCount;
	int IceThunderEggCount;
	int AirThunderEggCount;
	int GoldenCogCount;
	char BilbyCount[24];
};

class SaveDataHandler
{
public:
	static inline ExtendedSaveData saveData;
	static bool LoadSaveData(std::string seed);
};

