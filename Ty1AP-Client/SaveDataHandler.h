#pragma once
#include "LoggerWindow.h"
#include "savedata.h"
#include <iostream>
#include <fstream>
#include <string>

struct ExtendedSaveData {
	int Size;
	int Magic;
	LevelCode SavedLevel;
	char PercentageCompletion;
	char ThunderEggCount;
	bool IsHardcore;
	bool IsDevMode;
	LevelData LevelData[24];
	ZoneData ZoneData[6];
	short Unk2;
	LevelCode CurrentLevel;
	LevelCode PreviousLevel;
	Zone CurrentZone;
	Rang CurrentRang;
	AttributeData AttributeData;
	bool Talismans[5];
	bool TalismansPlaced[5];
	short Unk3;
	short Lives;
	char PictureFrames[0x41];
	char FmvsWatched;
	int PlayTimeSeconds;
	int EnemiesBitten;
	int EnemiesZapped;
	int EnemiesFlamed;
	int CricketBatsBurnt;
	bool CheatsUsed;
	bool CheatsUsed2;
	bool Unk4;
	bool Unk5;
	int SkinId;
	bool StopwatchesActive[24];
	int FireThunderEggCount;
	int IceThunderEggCount;
	int AirThunderEggCount;
	int GoldenCogCount;
	char BilbyCount[24];
	char FrameCount[24];
	bool PortalOpen[24];
	char ProgressiveRang;
	char ProgressiveLevel;
};

class SaveDataHandler
{
public:
	static inline ExtendedSaveData saveData;
	static bool LoadSaveData(std::string seed);
	static void SaveGame();
};