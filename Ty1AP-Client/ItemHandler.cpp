#include "ItemHandler.h"

std::queue<APClient::NetworkItem> ItemHandler::storedItems;

std::unordered_map<int, std::string> ItemHandler::boomerangMessages {
	{1, "Two is better than one."},
	{2, "You learnt how to Swim."},
	{3, "Rangs but underwater."},
	{4, "You can Dive."},
	{5, "Hot Boomerangs in your area."},
	{6, "Water but cold."},
	{7, "BZZZT!"},
	{8, "This Boomerang spinny."}
};

void ItemHandler::HandleItem(APClient::NetworkItem item)
{
	if (GameState::onLoadScreenOrMainMenu()) {
		storedItems.push(item);
		return;
	}

	if (item.index < SaveDataHandler::saveData.LastItemIndex)
		return;
	
	SaveDataHandler::saveData.LastItemIndex++;

	std::string itemname = ArchipelagoHandler::GetItemName(item.item);
	std::string sender = ArchipelagoHandler::GetPlayerAlias(item.player);
	std::string location = ArchipelagoHandler::GetLocationName(item.location);
	LoggerWindow::Log(itemname + " [color=AAAAAAFF]found at [color=FFFFFFFF]" + location + " [color=AAAAAAFF]in [color=FFFFFFFF]" + sender + "'s world");

	if (item.item == 0x8750000) {
		SaveDataHandler::saveData.FireThunderEggCount++;
		if (ArchipelagoHandler::levelUnlockStyle != LevelUnlockStyle::CHECKS &&
			SaveDataHandler::saveData.FireThunderEggCount >= ArchipelagoHandler::theggGating) {
			SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::A4)] = true;
		}
	}
	else if (item.item == 0x8750001) {
		SaveDataHandler::saveData.IceThunderEggCount++;
		if (ArchipelagoHandler::levelUnlockStyle != LevelUnlockStyle::CHECKS &&
			SaveDataHandler::saveData.IceThunderEggCount >= ArchipelagoHandler::theggGating) {
			SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::D4)] = true;
		}
	}
	else if (item.item == 0x8750002) {
		SaveDataHandler::saveData.AirThunderEggCount++;
		if (ArchipelagoHandler::levelUnlockStyle != LevelUnlockStyle::CHECKS &&
			SaveDataHandler::saveData.AirThunderEggCount >= ArchipelagoHandler::theggGating) {
			SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C4)] = true;
		}
	}
	else if (item.item == 0x8750003) {
		SaveDataHandler::saveData.GoldenCogCount++;
	}
	else if (item.item == 0x8750071) {
		SaveDataHandler::saveData.ProgressiveLevel++;
		HandleProgressiveLevel();
	}
	else if (item.item >= 0x8750030 && item.item < 0x8750040) {
		HandleIndividualLevel(item.item - 0x8750030);
	}
	else if (item.item == 0x8750070) {
		SaveDataHandler::saveData.ProgressiveRang++;
		if (boomerangMessages.find(SaveDataHandler::saveData.ProgressiveRang) != boomerangMessages.end())
			LoggerWindow::Log(boomerangMessages.at(SaveDataHandler::saveData.ProgressiveRang));
		HandleProgressiveRang();
	}
	else if (item.item >= 0x8750010 && item.item < 0x8750020) {
		HandleIndividualRang(item.item - 0x8750010);
	}
	else if (item.item >= 0x8750040 && item.item < 0x8750050) {
		SaveDataHandler::saveData.StopwatchesActive[item.item - 0x8750040] = true;
		if ((int)Level::getCurrentLevel() == item.item - 0x8750040)
			if (*(int*)(Core::moduleBase + 0x27041C) != 0)
				*(int*)(*(int*)(Core::moduleBase + 0x270420) + 0x68) = 0x2;
	}
	else if (item.item >= 0x8750020 && item.item < 0x8750030) {
		SaveDataHandler::saveData.BilbyCount[item.item - 0x8750020]++;
		if (SaveDataHandler::saveData.BilbyCount[item.item - 0x8750020] == 5) {
			auto level = item.item - 0x8750020;
			auto adjustedLevel = level - 4;
			adjustedLevel -= (adjustedLevel > 3) + (adjustedLevel > 7);
			ArchipelagoHandler::Check(0x8750270 + adjustedLevel);
		}
	}
	else if (item.item >= 0x8750050 && item.item < 0x8750060) {
		SaveDataHandler::saveData.Talismans[item.item - 0x8750050] = true;
		if (item.item - 0x8750050 < 3)
			SaveDataHandler::saveData.ZoneData[item.item - 0x8750050].Complete = true;
	}
	else if (item.item == 0x8750082) {
		Sound::PlayTySoundByIndex(0x1BC);
		auto objAddr = Core::moduleBase + 0x2888AC;
		void (*incrementLives)() = reinterpret_cast<void(*)()>(Core::moduleBase + 0xF6A80);
		__asm {
			mov ecx, objAddr
			call incrementLives
		}
	}
	else if (item.item == 0x8750083) {
		if (Level::getCurrentLevel() != LevelCode::Z1)
		{
			auto objAddr = Core::moduleBase + 0x270D34;
			void (*giveOpalMagnet)() = reinterpret_cast<void(*)()>(Core::moduleBase + 0x162000);
			__asm {
				mov ecx, objAddr
				call giveOpalMagnet
			}
		}
	}

	SaveDataHandler::SaveGame();
}

void ItemHandler::HandleStoredItems()
{
	while (!storedItems.empty() && !GameState::onLoadScreenOrMainMenu()) {
		HandleItem(storedItems.front());
		storedItems.pop();
	}
}

void ItemHandler::HandleProgressiveLevel() {
	bool bossesIncluded = ArchipelagoHandler::levelUnlockStyle == LevelUnlockStyle::CHECKS;
	if (SaveDataHandler::saveData.ProgressiveLevel >= 1) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::A2)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 2) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::A3)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 3 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::A4)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 3 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 4 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::B1)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 4 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 5 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::B2)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 5 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 6 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::B3)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 7 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::D4)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 6 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 8 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C1)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 7 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 9 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C2)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 8 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 10 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C3)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 11 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C4)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 9 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 12 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::E1)] = true;
	}

	if (Level::getCurrentLevel() != LevelCode::Z1)
		return;
	auto portalCount = *(int*)(Core::moduleBase + 0x267404);
	auto portalAddr = *(int*)(Core::moduleBase + 0x267408);
	for (auto portalIndex = 0; portalIndex < portalCount; portalIndex++) {
		auto portalDestination = *(int*)(portalAddr + 0xAC);
		if (SaveDataHandler::saveData.PortalOpen[portalDestination])
			*(int*)(portalAddr + 0x9C) = 1;
		else
			*(int*)(portalAddr + 0x9C) = 3;
		portalAddr = *(int*)(portalAddr + 0x34);
	}
}

void ItemHandler::HandleIndividualLevel(int code) {
	switch (code) {
	case 0:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 4);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 1:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 5);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 2:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 6);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 3:
		SaveDataHandler::saveData.PortalOpen[7] = true;
		break;
	case 4:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 8);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 5:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 9);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 6:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 10);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 7:
		SaveDataHandler::saveData.PortalOpen[19] = true;
		break;
	case 8:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 12);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 9:
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 13);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 10: 
	{
		auto it = std::find(ArchipelagoHandler::portalMap.begin(), ArchipelagoHandler::portalMap.end(), 14);
		int index = std::distance(ArchipelagoHandler::portalMap.begin(), it);
		auto levelIndex = index + 4 + (index > 2) + (index > 5);
		SaveDataHandler::saveData.PortalOpen[levelIndex] = true;
	}
		break;
	case 11:
		SaveDataHandler::saveData.PortalOpen[15] = true;
		break;
	case 12:
		SaveDataHandler::saveData.PortalOpen[20] = true;
		break;
	}

	if (Level::getCurrentLevel() != LevelCode::Z1)
		return;
	auto portalCount = *(int*)(Core::moduleBase + 0x267404);
	auto portalAddr = *(int*)(Core::moduleBase + 0x267408);
	for (auto portalIndex = 0; portalIndex < portalCount; portalIndex++) {
		auto portalDestination = *(int*)(portalAddr + 0xAC);
		if (SaveDataHandler::saveData.PortalOpen[portalDestination])
			*(int*)(portalAddr + 0x9C) = 1;
		else
			*(int*)(portalAddr + 0x9C) = 3;
		portalAddr = *(int*)(portalAddr + 0x34);
	}
}

void ItemHandler::HandleProgressiveRang() {
	if (SaveDataHandler::saveData.ProgressiveRang >= 1)
		SaveDataHandler::saveData.ArchAttributeData.GotSecondRang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 2)
		SaveDataHandler::saveData.ArchAttributeData.LearntToSwim = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 3)
		SaveDataHandler::saveData.ArchAttributeData.GotAquarang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 4)
		SaveDataHandler::saveData.ArchAttributeData.LearntToDive = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 5)
		SaveDataHandler::saveData.ArchAttributeData.GotFlamerang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 6)
		SaveDataHandler::saveData.ArchAttributeData.GotFrostyrang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 7)
		SaveDataHandler::saveData.ArchAttributeData.GotZappyrang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 8)
		SaveDataHandler::saveData.ArchAttributeData.GotDoomerang = true;
}

void ItemHandler::HandleIndividualRang(int code) {
	switch (code) {
	case 0:
		SaveDataHandler::saveData.ArchAttributeData.LearntToSwim = true;
		break;
	case 1:
		SaveDataHandler::saveData.ArchAttributeData.LearntToDive = true;
		break;
	case 2:
		SaveDataHandler::saveData.ArchAttributeData.GotSecondRang = true;
		break;
	case 3:
		SaveDataHandler::saveData.ArchAttributeData.GotExtraHealth = true;
		break;
	case 4:
		SaveDataHandler::saveData.ArchAttributeData.GotBoomerang = true;
		break;
	case 5:
		SaveDataHandler::saveData.ArchAttributeData.GotFlamerang = true;
		break;
	case 6:
		SaveDataHandler::saveData.ArchAttributeData.GotFrostyrang = true;
		break;
	case 7:
		SaveDataHandler::saveData.ArchAttributeData.GotZappyrang = true;
		break;
	case 8:
		SaveDataHandler::saveData.ArchAttributeData.GotAquarang = true;
		break;
	case 9:
		SaveDataHandler::saveData.ArchAttributeData.GotZoomerang = true;
		break;
	case 10:
		SaveDataHandler::saveData.ArchAttributeData.GotMultirang = true;
		break;
	case 11:
		SaveDataHandler::saveData.ArchAttributeData.GotInfrarang = true;
		break;
	case 12:
		SaveDataHandler::saveData.ArchAttributeData.GotMegarang = true;
		break;
	case 13:
		SaveDataHandler::saveData.ArchAttributeData.GotKaboomerang = true;
		break;
	case 14:
		SaveDataHandler::saveData.ArchAttributeData.GotChronorang = true;
		break;
	case 15:
		SaveDataHandler::saveData.ArchAttributeData.GotDoomerang = true;
		break;
	}
}