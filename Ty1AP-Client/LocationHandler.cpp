#include "pch.h"
#include "LocationHandler.h"

std::unordered_map<int, int> LocationHandler::crateOpalCounts = {
	{ 0, 0 },
	{ 4, 170 },
	{ 5, 102 },
	{ 6, 119 },
	{ 8, 120 },
	{ 9, 60 },
	{ 10, 300 },
	{ 12, 30 },
	{ 13, 170 },
	{ 14, 215 },
};

void LocationHandler::HandleLocation(int64_t location)
{
	if (GameState::onMainMenu()) {
		storedCheckedLocations.push(location);
		return;
	}

	auto currentLevel = Level::getCurrentLevel();

	if (location >= 0x8750100 && location < 0x8750148) {
		// THUNDER EGG
		int theggId = location - 0x8750100;
		auto levelIndex = theggId / 8;
		auto levelId = levelIndex + 4 + (levelIndex > 2) + (levelIndex > 5);
		auto theggIndex = theggId % 8;
		bool myCollection = SaveDataHandler::saveData.LevelData[levelId].ThunderEggs[theggIndex];
		if (myCollection)
			return;
		SaveDataHandler::saveData.LevelData[levelId].ThunderEggs[theggIndex] = true;
		SaveDataHandler::SaveGame();
		if ((LevelCode)levelId != currentLevel)
			return;
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x270238 + 0x48);
		for (int i = 0; i < 8; i++) {
			auto currentTheggIndex = *(int*)(addr + 0x6C);
			if (theggIndex == currentTheggIndex) {
				*(bool*)(addr + 0x66) = true;
				return;
			}
			addr = *(uintptr_t*)(addr + 0x34);
		}
	}
	if (location >= 0x8750148 && location < 0x87501A2) {
		// GOLDEN COG
		int cogId = location - 0x8750148;
		auto levelIndex = cogId / 10;
		auto levelId = levelIndex + 4 + (levelIndex > 2) + (levelIndex > 5);
		auto cogIndex = cogId % 10;
		bool myCollection = SaveDataHandler::saveData.LevelData[levelId].GoldenCogs[cogIndex];
		if (myCollection)
			return;
		SaveDataHandler::saveData.LevelData[levelId].GoldenCogs[cogIndex] = true;
		SaveDataHandler::SaveGame();
		if ((LevelCode)levelId != currentLevel)
			return;
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x2702C8 + 0x48);
		for (int i = 0; i < 10; i++) {
			auto currentCogIndex = *(int*)(addr + 0x6C);
			if (cogIndex == currentCogIndex) {
				*(int*)(addr + 0xC4) = 5;
				return;
			}
			addr = *(uintptr_t*)(addr + 0x34);
		}
	}
	if (location >= 0x87501AC && location < 0x87501D9) {
		// BILBY
		int bilbyId = location - 0x87501AC;
		auto levelIndex = bilbyId / 5;
		auto levelId = levelIndex + 4 + (levelIndex > 2) + (levelIndex > 5);
		auto bilbyIndex = bilbyId % 5;
		auto bilbyWrite = std::count(SaveDataHandler::saveData.LevelData[levelId].Bilbies,
			SaveDataHandler::saveData.LevelData[levelId].Bilbies + 0x5, 1) == 4 ? 3 : 1;
		bool myCollection = SaveDataHandler::saveData.LevelData[levelId].Bilbies[bilbyIndex] > 0;
		if (myCollection)
			return;
		SaveDataHandler::saveData.LevelData[levelId].Bilbies[bilbyIndex] = bilbyWrite;
		SaveDataHandler::SaveGame();
		if ((LevelCode)levelId != currentLevel)
			return;
		if (bilbyWrite == 3)
			return;
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x27D608);
		addr = *(uintptr_t*)(addr);
		for (int i = 0; i < 5; i++) {
			auto currentBilbyIndex = *(int*)(addr);
			if (currentBilbyIndex == bilbyIndex) {
				*(int*)(addr + 0x34) = 0;
				*(int*)(addr + 0x58) = 0;
				return;
			}
			addr += 0x134;
		}
	}
	if (location >= 0x87501D9 && location < 0x8750258) {
		// PF
		int frameId = location - 0x87501D9;
		auto byteIndex = frameId / 8;
		auto bitIndex = frameId % 8;
		auto b = SaveDataHandler::saveData.PictureFrames[byteIndex];
		b |= 1 << bitIndex;
		SaveDataHandler::saveData.PictureFrames[byteIndex] = b;
		SaveDataHandler::SaveGame();
		auto frameCount = *(int*)(Core::moduleBase + 0x254D68 + 0x44);
		auto addr = *(uintptr_t*)(Core::moduleBase + 0x254D68 + 0x48);
		for (int i = 0; i < frameCount; i++) {
			auto currentFrameId = *(int*)(addr + 0x1D0);
			if (frameId == currentFrameId) {
				*(int*)(addr + 0x48) = 0;
				*(int*)(addr + 0x114) = 2;
				return;
			}
			addr = *(uintptr_t*)(addr + 0x34);
		}
		return;
	}
	if (location >= 0x8750350 && location < 0x875E47C) {
		// OPALS / RAINBOW SCALES
		int level = (location - 0x8750000) >> 0xC;
		int opalId = location - (0x8750350 + (level << 0xC));
		auto byteIndex = opalId / 8;
		auto bitIndex = opalId % 8;
		auto b = SaveDataHandler::saveData.LevelData[level].Opals[byteIndex];
		b |= 1 << bitIndex;
		SaveDataHandler::saveData.LevelData[level].Opals[byteIndex] = b;
		SaveDataHandler::SaveGame();

		if ((LevelCode)level != currentLevel)
			return;

		auto looseOpalCount = *(int*)(Core::moduleBase + 0x269818 + 0x44);
		auto looseOpalAddr = *(uintptr_t*)(Core::moduleBase + 0x269818 + 0x48);
		auto crateOpalAddr = *(uintptr_t*)(Core::moduleBase + 0x28AB70);
		if (opalId < looseOpalCount) {
			for (int i = 0; i < looseOpalCount; i++) {
				if (opalId == i) {
					*(int*)(looseOpalAddr + 0x78) = 5;
					return;
				}
				looseOpalAddr = *(uintptr_t*)(looseOpalAddr + 0x34);
			}
		}
		else {
			for (int i = looseOpalCount; i < 300; i++) {
				if (opalId == i) {
					*(int*)(crateOpalAddr + 0x78) = 5;
					return;
				}
				crateOpalAddr = *(uintptr_t*)(crateOpalAddr + 0x34);
			}
		}
	}
	if (location >= 0x8750261 && location < 0x8750266) {
		// TALISMAN
		int talismanId = location - 0x8750261;
		SaveDataHandler::saveData.Talismans[talismanId] = true;
		SaveDataHandler::SaveGame();
	}
	if (location >= 0x8750310 && location < 0x8750320) {
		// ATTRIBUTE
		int attributeId = location - 0x8750310;
		switch (attributeId) {
		case 0:
			SaveDataHandler::saveData.AttributeData.LearntToSwim = true;
			break;
		case 1:
			SaveDataHandler::saveData.AttributeData.LearntToDive = true;
			break;
		case 2:
			SaveDataHandler::saveData.AttributeData.GotSecondRang = true;
			break;
		case 3:
			SaveDataHandler::saveData.AttributeData.GotExtraHealth = true;
			break;
		case 4:
			SaveDataHandler::saveData.AttributeData.GotBoomerang = true;
			break;
		case 5:
			SaveDataHandler::saveData.AttributeData.GotFrostyrang = true;
			break;
		case 6:
			SaveDataHandler::saveData.AttributeData.GotFlamerang = true;
			break;
		case 7:
			SaveDataHandler::saveData.AttributeData.GotKaboomerang = true;
			break;
		case 8:
			SaveDataHandler::saveData.AttributeData.GotDoomerang = true;
			break;
		case 9:
			SaveDataHandler::saveData.AttributeData.GotMegarang = true;
			break;
		case 10:
			SaveDataHandler::saveData.AttributeData.GotZoomerang = true;
			break;
		case 11:
			SaveDataHandler::saveData.AttributeData.GotInfrarang = true;
			break;
		case 12:
			SaveDataHandler::saveData.AttributeData.GotZappyrang = true;
			break;
		case 13:
			SaveDataHandler::saveData.AttributeData.GotAquarang = true;
			break;
		case 14:
			SaveDataHandler::saveData.AttributeData.GotMultirang = true;
			break;
		case 15:
			SaveDataHandler::saveData.AttributeData.GotChronorang = true;
			break;
		}
		SaveDataHandler::SaveGame();
	}
	
}

void LocationHandler::HandleStoredCheckedLocations()
{
	while (!storedCheckedLocations.empty() && !GameState::onMainMenu()) {
		HandleLocation(storedCheckedLocations.front());
		storedCheckedLocations.pop();
	}
}
