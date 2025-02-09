#include "ItemHandler.h"


std::queue<APClient::NetworkItem> ItemHandler::storedItems;

std::unordered_map<int, std::string> ItemHandler::boomerangMessages {
	{1, "You were given the Boomerang."},
	{2, "You found a Second Rang."},
	{3, "You learnt how to Swim."},
	{4, "Rangs but underwater."},
	{5, "You can Dive."},
	{6, "Hot Boomerangs in your area."},
	{7, "Water but cold."},
	{8, "BZZZT!"},
	{9, "This Boomerang spinny."}
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
			SaveDataHandler::saveData.FireThunderEggCount >= ArchipelagoHandler::hubTheggCounts) {
			SaveDataHandler::saveData.PortalOpen[7] = true;
		}
		if (ArchipelagoHandler::goal == Goal::ALL_THEGGS && 
			SaveDataHandler::saveData.FireThunderEggCount == 24
			&& SaveDataHandler::saveData.IceThunderEggCount == 24
			&& SaveDataHandler::saveData.AirThunderEggCount == 24)
			ArchipelagoHandler::Release();
		if (ArchipelagoHandler::goal == Goal::COMPLETION && CheckCompletion())
			ArchipelagoHandler::Release();
	}
	else if (item.item == 0x8750001) {
		SaveDataHandler::saveData.IceThunderEggCount++;
		if (ArchipelagoHandler::levelUnlockStyle != LevelUnlockStyle::CHECKS &&
			SaveDataHandler::saveData.IceThunderEggCount >= ArchipelagoHandler::hubTheggCounts) {
			SaveDataHandler::saveData.PortalOpen[9] = true;
		}
		if (ArchipelagoHandler::goal == Goal::ALL_THEGGS &&
			SaveDataHandler::saveData.FireThunderEggCount == 24
			&& SaveDataHandler::saveData.IceThunderEggCount == 24
			&& SaveDataHandler::saveData.AirThunderEggCount == 24)
			ArchipelagoHandler::Release();
		if (ArchipelagoHandler::goal == Goal::COMPLETION && CheckCompletion())
			ArchipelagoHandler::Release();
	}
	else if (item.item == 0x8750002) {
		SaveDataHandler::saveData.AirThunderEggCount++;
		if (ArchipelagoHandler::levelUnlockStyle != LevelUnlockStyle::CHECKS &&
			SaveDataHandler::saveData.AirThunderEggCount >= ArchipelagoHandler::hubTheggCounts) {
			SaveDataHandler::saveData.PortalOpen[15] = true;
		}
		if (ArchipelagoHandler::goal == Goal::ALL_THEGGS &&
			SaveDataHandler::saveData.FireThunderEggCount == 24
			&& SaveDataHandler::saveData.IceThunderEggCount == 24
			&& SaveDataHandler::saveData.AirThunderEggCount == 24)
			ArchipelagoHandler::Release();
		if (ArchipelagoHandler::goal == Goal::COMPLETION && CheckCompletion())
			ArchipelagoHandler::Release();
	}
	else if (item.item == 0x8750003) {
		SaveDataHandler::saveData.GoldenCogCount++;
		if (ArchipelagoHandler::goal == Goal::COMPLETION && CheckCompletion())
			ArchipelagoHandler::Release();
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
		if (ArchipelagoHandler::goal == Goal::COMPLETION && CheckCompletion())
			ArchipelagoHandler::Release();
	}
	else if (item.item == 0x8750082) {
		// EXTRA LIFE
		Sound::PlayTySoundByIndex(0x1BC);
		auto objAddr = Core::moduleBase + 0x2888AC;
		void (*incrementLives)() = reinterpret_cast<void(*)()>(Core::moduleBase + 0xF6A80);
		__asm {
			mov ecx, objAddr
			call incrementLives
		}
	}
	else if (item.item == 0x8750083) {
		auto objAddr = Core::moduleBase + 0x270D34;
		void (*giveOpalMagnet)() = reinterpret_cast<void(*)()>(Core::moduleBase + 0x162000);
		__asm {
			mov ecx, objAddr
			call giveOpalMagnet
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
		SaveDataHandler::saveData.PortalOpen[5] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 2) {
		SaveDataHandler::saveData.PortalOpen[6] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 3 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[7] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 3 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 4 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[8] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 4 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 5 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[9] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 5 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 6 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[10] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 7 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[19] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 6 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 8 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[12] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 7 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 9 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[13] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 8 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 10 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[14] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 11 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[15] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 9 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 12 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[20] = true;
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
	{
		auto it = std::find(ArchipelagoHandler::bossMap.begin(), ArchipelagoHandler::bossMap.end(), 7);
		int index = std::distance(ArchipelagoHandler::bossMap.begin(), it);
		if (index == 0)
			SaveDataHandler::saveData.PortalOpen[7] = true;
		else if (index == 1)
			SaveDataHandler::saveData.PortalOpen[19] = true;
		else if (index == 2)
			SaveDataHandler::saveData.PortalOpen[15] = true;
	}
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
	{
		auto it = std::find(ArchipelagoHandler::bossMap.begin(), ArchipelagoHandler::bossMap.end(), 19);
		int index = std::distance(ArchipelagoHandler::bossMap.begin(), it);
		if (index == 0)
			SaveDataHandler::saveData.PortalOpen[7] = true;
		else if (index == 1)
			SaveDataHandler::saveData.PortalOpen[19] = true;
		else if (index == 2)
			SaveDataHandler::saveData.PortalOpen[15] = true;
	}
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
	{
		auto it = std::find(ArchipelagoHandler::bossMap.begin(), ArchipelagoHandler::bossMap.end(), 15);
		int index = std::distance(ArchipelagoHandler::bossMap.begin(), it);
		if (index == 0)
			SaveDataHandler::saveData.PortalOpen[7] = true;
		else if (index == 1)
			SaveDataHandler::saveData.PortalOpen[19] = true;
		else if (index == 2)
			SaveDataHandler::saveData.PortalOpen[15] = true;
	}
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
		SaveDataHandler::saveData.ArchAttributeData.GotBoomerang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 2)
		SaveDataHandler::saveData.ArchAttributeData.GotSecondRang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 3)
		SaveDataHandler::saveData.ArchAttributeData.LearntToSwim = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 4)
		SaveDataHandler::saveData.ArchAttributeData.GotAquarang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 5)
		SaveDataHandler::saveData.ArchAttributeData.LearntToDive = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 6)
		SaveDataHandler::saveData.ArchAttributeData.GotFlamerang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 7)
		SaveDataHandler::saveData.ArchAttributeData.GotFrostyrang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 8)
		SaveDataHandler::saveData.ArchAttributeData.GotZappyrang = true;
	if (SaveDataHandler::saveData.ProgressiveRang >= 9)
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
		SaveDataHandler::saveData.ArchAttributeData.GotExtraHealth = true;
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

bool ItemHandler::CheckCompletion() {
	return std::count(SaveDataHandler::saveData.TalismansPlaced,
		SaveDataHandler::saveData.TalismansPlaced + 5, true) == 5
		&& SaveDataHandler::saveData.FireThunderEggCount == 24
		&& SaveDataHandler::saveData.IceThunderEggCount == 24
		&& SaveDataHandler::saveData.AirThunderEggCount == 24
		&& SaveDataHandler::saveData.GoldenCogCount == 90;
}