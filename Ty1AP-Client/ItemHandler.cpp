#include "ItemHandler.h"

std::queue<APClient::NetworkItem> ItemHandler::storedItems;

std::unordered_map<int, std::string> ItemHandler::boomerangMessages {
	{1, "Two Rangs are better than one. (Second Rang)"},
	{2, "You graduated from floaties. (Swim)"},
	{3, "We're gonna need a wetter rang. (Aquarang)"},
	{4, "Jump in, the water is fine. (Dive)"},
	{5, "Hot rangs in your area. (Flamerang)"},
	{6, "HONEY, WHERE'S MY SUPER SUIT? (Frostyrang)"},
	{7, "Pikachu, I choose you! (Zappyrang)"},
	{8, "Beyblade! Beyblade! Let it rip! (Doomerang)"}
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
		if (SaveDataHandler::saveData.ArchAttributeData.GotSecondRang && Level::getCurrentLevel() == LevelCode::Z1) {
			auto gateAddr = *(uintptr_t*)(Core::moduleBase + 0x269C14);
			gateAddr = *(uintptr_t*)(gateAddr + 0x78);
			*(bool*)(gateAddr + 0x111) = true;
		}
	}
	else if (item.item >= 0x8750010 && item.item < 0x8750020) {
		HandleIndividualRang(item.item - 0x8750010);
		if (SaveDataHandler::saveData.ArchAttributeData.GotSecondRang && Level::getCurrentLevel() == LevelCode::Z1) {
			auto gateAddr = *(uintptr_t*)(Core::moduleBase + 0x269C14);
			gateAddr = *(uintptr_t*)(gateAddr + 0x78);
			*(bool*)(gateAddr + 0x111) = true;
		}
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
			SaveDataHandler::saveData.ZoneData[(item.item - 0x8750050) + 1].Complete = true;
	}
	else if (item.item == 0x8750082) {
		Sound::PlayTySoundByIndex(GlobalSound::OpalsAllCollected);
		auto objAddr = Core::moduleBase + 0x2888AC;
		void (*incrementLives)() = reinterpret_cast<void(*)()>(Core::moduleBase + 0xF6A80);
		__asm {
			mov ecx, objAddr
			call incrementLives
		}
	}
	else if (item.item == 0x8750083) { // Opal Magnet
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
	else if (item.item == 0x8750084) { // Quarter Pie
		auto objAddr = Hero::isBull() ? Core::moduleBase + 0x254400 : Core::moduleBase + 0x270D10;
		Sound::PlayTySoundByIndex(GlobalSound::TyBitePie);
		void (*giveQuarterPie)() = reinterpret_cast<void(*)()>(Core::moduleBase + 0x16F890);
		__asm {
			mov ecx, objAddr
			push 1
			call giveQuarterPie
		}
	}
	else if (item.item == 0x8750085) { // Full Pie
		auto objAddr = Hero::isBull() ? Core::moduleBase + 0x254400 : Core::moduleBase + 0x270D10;
		auto healthCount = *(int*)(Core::moduleBase + 0x2737CC);
		auto maxHealth = SaveDataHandler::saveData.ArchAttributeData.GotExtraHealth ? 8 : 4;
		auto healthPush = maxHealth - healthCount;
		Sound::PlayTySoundByIndex(GlobalSound::TyBitePie);
		void (*giveFullPie)() = reinterpret_cast<void(*)()>(Core::moduleBase + 0x16F890);
		__asm {
			mov ecx, objAddr
			push healthPush
			call giveFullPie
		}
	}
	else if (item.item == 0x8750090) { // Knocked Down Trap
		Hero::setState(TyState::KnockedOver);
		Hero::setHealth(1);
	}
	else if (item.item == 0x8750091) { // Slow Trap
		std::thread([] {
			auto iterator = 0;
			while (iterator < 1000) {
				Hero::setRunSpeed(5.0f);
				Sleep(20);
				iterator++;
			}
			Hero::setRunSpeed(10.0f);
		}).detach();
	}
	else if (item.item == 0x8750092) { // Gravity Trap
		std::thread([] {
			auto iterator = 0;
			while (iterator < 1000) {
				Hero::setGravity(1.15f);
				Sleep(20);
				iterator++;
			}
			Hero::setGravity(0.75f);
		}).detach();
	}
	else if (item.item == 0x8750093) { // Acid Trap
		std::thread([] {
			auto initState = *(bool*)(Core::moduleBase + 0x2895C5);
			auto initHue = *(float*)(Core::moduleBase + 0x288F7C);
			*(bool*)(Core::moduleBase + 0x2895C5) = true;
			auto hue = initHue;
			while (hue < 10) {
				hue += 0.006f;
				*(float*)(Core::moduleBase + 0x288F7C) = hue;
				if (GameState::onLoadScreenOrMainMenu())
					break;
				Sleep(10);
			}
			while (hue > initHue) {
				hue -= 0.006f;
				*(float*)(Core::moduleBase + 0x288F7C) = hue;
				if (GameState::onLoadScreenOrMainMenu())
					break;
				Sleep(10);
			}
			*(float*)(Core::moduleBase + 0x288F7C) = initHue;
			*(bool*)(Core::moduleBase + 0x2895C5) = initState;
		}).detach();
	}
	else if (item.item == 0x8750094) { // Exit Trap
		if (Level::getCurrentLevel() != LevelCode::Z1)
			Level::changeLevel(LevelCode::Z1);
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
	if ((SaveDataHandler::saveData.ProgressiveLevel >= 3 && !bossesIncluded) ||
		(SaveDataHandler::saveData.ProgressiveLevel >= 4 && bossesIncluded)) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::B1)] = true;
	}
	if ((SaveDataHandler::saveData.ProgressiveLevel >= 4 && !bossesIncluded) ||
		(SaveDataHandler::saveData.ProgressiveLevel >= 5 && bossesIncluded)) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::B2)] = true;
	}
	if ((SaveDataHandler::saveData.ProgressiveLevel >= 5 && !bossesIncluded) ||
		(SaveDataHandler::saveData.ProgressiveLevel >= 6 && bossesIncluded)) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::B3)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 7 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::D4)] = true;
	}
	if ((SaveDataHandler::saveData.ProgressiveLevel >= 6 && !bossesIncluded) ||
		(SaveDataHandler::saveData.ProgressiveLevel >= 8 && bossesIncluded)) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C1)] = true;
	}
	if ((SaveDataHandler::saveData.ProgressiveLevel >= 7 && !bossesIncluded) ||
		(SaveDataHandler::saveData.ProgressiveLevel >= 9 && bossesIncluded)) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C2)] = true;
	}
	if ((SaveDataHandler::saveData.ProgressiveLevel >= 8 && !bossesIncluded) ||
		(SaveDataHandler::saveData.ProgressiveLevel >= 10 && bossesIncluded)) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C3)] = true;
	}
	if (SaveDataHandler::saveData.ProgressiveLevel >= 11 && bossesIncluded) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::C4)] = true;
	}
	if ((SaveDataHandler::saveData.ProgressiveLevel >= 9 && !bossesIncluded) ||
		(SaveDataHandler::saveData.ProgressiveLevel >= 12 && bossesIncluded)) {
		SaveDataHandler::saveData.PortalOpen[static_cast<int>(LevelCode::E1)] = true;
	}
}

void ItemHandler::HandleIndividualLevel(int code) {
	switch (code) {
	case 0:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[4]] = true;
		break;
	case 1:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[5]] = true;
		break;
	case 2:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[6]] = true;
		break;
	case 3:
		SaveDataHandler::saveData.PortalOpen[7] = true;
		break;
	case 4:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[8]] = true;
		break;
	case 5:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[9]] = true;
		break;
	case 6:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[10]] = true;
		break;
	case 7:
		SaveDataHandler::saveData.PortalOpen[19] = true;
		break;
	case 8:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[12]] = true;
		break;
	case 9:
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[13]] = true;
		break;
	case 10: 
		SaveDataHandler::saveData.PortalOpen[ArchipelagoHandler::inversePortalMap[14]] = true;
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