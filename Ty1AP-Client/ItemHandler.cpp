#include "ItemHandler.h"


std::queue<APClient::NetworkItem> storedItems;

void ItemHandler::HandleItem(APClient::NetworkItem item)
{
	if (item.item == 0x8750000) {
		SaveDataHandler::saveData.FireThunderEggCount++;
		return;
	}
	if (item.item == 0x8750001) {
		SaveDataHandler::saveData.IceThunderEggCount++;
		return;
	}
	if (item.item == 0x8750002) {
		SaveDataHandler::saveData.AirThunderEggCount++;
		return;
	}
	if (item.item == 0x8750003) {
		SaveDataHandler::saveData.GoldenCogCount++;
		return;
	}
	if (item.item == 0x8750071) {
		SaveDataHandler::saveData.ProgressiveLevel++;
		HandleProgressiveLevel();
		return;
	}
	if (item.item >= 0x8750030 && item.item < 0x8750040) {
		HandleIndividualLevel(item.item - 0x8750030);
		return;
	}
	if (item.item == 0x8750070) {
		SaveDataHandler::saveData.ProgressiveRang++;
		HandleProgressiveRang();
		return;
	}
	if (item.item >= 0x8750010 && item.item < 0x8750020) {
		HandleIndividualRang(item.item - 0x8750010);
		return;
	}
	if (item.item >= 0x8750040 && item.item < 0x8750050) {
		SaveDataHandler::saveData.StopwatchesActive[item.item - 0x8750040] = true;
		// TODO IF LEVEL IS CURRENT LEVEL SPAWN THE STOPWATCH
		return;
	}
	if (item.item >= 0x8750020 && item.item < 0x8750030) {
		SaveDataHandler::saveData.BilbyCount[item.item - 0x8750020]++;
		if (SaveDataHandler::saveData.BilbyCount[item.item - 0x8750020] == 5) {
			if (item.item - 0x8750020 < 8)
				SaveDataHandler::saveData.FireThunderEggCount++;
			else if (item.item - 0x8750020 < 12)
				SaveDataHandler::saveData.IceThunderEggCount++;
			else
				SaveDataHandler::saveData.AirThunderEggCount++;
		}
		return;
	}
	if (item.item >= 0x8750050 && item.item < 0x8750060) {
		SaveDataHandler::saveData.Talismans[item.item - 0x8750050] = true;
		SaveDataHandler::saveData.TalismansPlaced[item.item - 0x8750050] = true;
		return;
	}
	if (item.item == 0x8750082) {
		// TODO TRIGGER EXTRA LIFE
		return;
	}
	if (item.item == 0x8750083) {
		// TODO TRIGGER OPAL MAGNET
		return;
	}
	//if (item.item == 0x8750084) {
		// TODO TRIGGER CHARGE BITE POWERUP
	//}
}

void ItemHandler::HandleStoredItems()
{
	while (!storedItems.empty() && !GameState::onLoadScreenOrMainMenu()) {
		HandleItem(storedItems.front());
		storedItems.pop();
	}
}

void HandleProgressiveLevel() {
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
		SaveDataHandler::saveData.PortalOpen[21] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 9 && !bossesIncluded ||
		SaveDataHandler::saveData.ProgressiveLevel >= 12 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[21] = true;
	}
}

void HandleIndividualLevel(int code) {
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
		SaveDataHandler::saveData.PortalOpen[21] = true;
		break;
	}
}

void HandleProgressiveRang() {
	if (SaveDataHandler::saveData.ProgressiveRang >= 1) {
		SaveDataHandler::saveData.AttributeData.GotBoomerang = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 2) {
		SaveDataHandler::saveData.AttributeData.LearntToSwim = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 3) {
		SaveDataHandler::saveData.AttributeData.GotSecondRang = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 4) {
		SaveDataHandler::saveData.AttributeData.GotAquarang = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 5) {
		SaveDataHandler::saveData.AttributeData.LearntToDive = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 6) {
		SaveDataHandler::saveData.AttributeData.GotFlamerang = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 7) {
		SaveDataHandler::saveData.AttributeData.GotFrostyrang = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 8) {
		SaveDataHandler::saveData.AttributeData.GotZappyrang = true;
	}
	if (SaveDataHandler::saveData.ProgressiveRang >= 9) {
		SaveDataHandler::saveData.AttributeData.GotDoomerang = true;
	}
}

void HandleIndividualRang(int code) {
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