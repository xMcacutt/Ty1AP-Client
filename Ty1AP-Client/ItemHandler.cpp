#include "ItemHandler.h"


std::queue<APClient::NetworkItem> ItemHandler::storedItems;

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
}

void ItemHandler::HandleIndividualLevel(int code) {
	switch (code) {
	case 1:
		SaveDataHandler::saveData.PortalOpen[5] = true;
		break;
	case 2:
		SaveDataHandler::saveData.PortalOpen[6] = true;
		break;
	case 3:
		SaveDataHandler::saveData.PortalOpen[7] = true;
		break;
	case 4:
		SaveDataHandler::saveData.PortalOpen[8] = true;
		break;
	case 5:
		SaveDataHandler::saveData.PortalOpen[9] = true;
		break;
	case 6:
		SaveDataHandler::saveData.PortalOpen[10] = true;
		break;
	case 7:
		SaveDataHandler::saveData.PortalOpen[19] = true;
		break;
	case 8:
		SaveDataHandler::saveData.PortalOpen[12] = true;
		break;
	case 9:
		SaveDataHandler::saveData.PortalOpen[13] = true;
		break;
	case 10:
		SaveDataHandler::saveData.PortalOpen[14] = true;
		break;
	case 11:
		SaveDataHandler::saveData.PortalOpen[15] = true;
		break;
	case 12:
		SaveDataHandler::saveData.PortalOpen[20] = true;
		break;
	}
}

void ItemHandler::HandleProgressiveRang() {
	if (SaveDataHandler::saveData.ProgressiveRang >= 1) {
		SaveDataHandler::saveData.AttributeData.GotBoomerang = true;
		LoggerWindow::Log("You were given the Boomerang.");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 2) {
		SaveDataHandler::saveData.AttributeData.LearntToSwim = true;
		LoggerWindow::Log("You learnt how to Swim.");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 3) {
		SaveDataHandler::saveData.AttributeData.GotSecondRang = true;
		LoggerWindow::Log("You found a Second Rang.");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 4) {
		SaveDataHandler::saveData.AttributeData.GotAquarang = true;
		LoggerWindow::Log("Rangs but underwater.");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 5) {
		SaveDataHandler::saveData.AttributeData.LearntToDive = true;
		LoggerWindow::Log("You can Dive.");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 6) {
		SaveDataHandler::saveData.AttributeData.GotFlamerang = true;
		LoggerWindow::Log("Hot Boomerangs in your area.");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 7) {
		SaveDataHandler::saveData.AttributeData.GotFrostyrang = true;
		LoggerWindow::Log("Water but cold.");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 8) {
		SaveDataHandler::saveData.AttributeData.GotZappyrang = true;
		LoggerWindow::Log("BZZZT!");
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 9) {
		SaveDataHandler::saveData.AttributeData.GotDoomerang = true;
		LoggerWindow::Log("This Boomerang spinny.");
	}
}

void ItemHandler::HandleIndividualRang(int code) {
	switch (code) {
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
	case 5:
		SaveDataHandler::saveData.AttributeData.GotFlamerang = true;
		break;
	case 6:
		SaveDataHandler::saveData.AttributeData.GotFrostyrang = true;
		break;
	case 7:
		SaveDataHandler::saveData.AttributeData.GotZappyrang = true;
		break;
	case 8:
		SaveDataHandler::saveData.AttributeData.GotAquarang = true;
		break;
	case 9:
		SaveDataHandler::saveData.AttributeData.GotZoomerang = true;
		break;
	case 10:
		SaveDataHandler::saveData.AttributeData.GotMultirang = true;
		break;
	case 11:
		SaveDataHandler::saveData.AttributeData.GotInfrarang = true;
		break;
	case 12:
		SaveDataHandler::saveData.AttributeData.GotMegarang = true;
		break;
	case 13:
		SaveDataHandler::saveData.AttributeData.GotKaboomerang = true;
		break;
	case 14:
		SaveDataHandler::saveData.AttributeData.GotChronorang = true;
		break;
	case 15:
		SaveDataHandler::saveData.AttributeData.GotDoomerang = true;
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