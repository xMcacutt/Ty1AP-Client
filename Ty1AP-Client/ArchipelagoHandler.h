#pragma once
#include "apclient.hpp"
#include "GameHandler.h"
#include "uuid.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <map>
#include "TygerFrameworkAPI.hpp"
#include "gui.h"
#include "LoginWindow.h"
#include "gamestate.h"

enum class LevelUnlockStyle {
	VANILLA,
	CHECKS,
	VANILLA_BOSS
};

enum class Goal{
	BEAT_CASS,
};

enum class Framesanity {
	ALL,
	PER_LEVEL,
	NONE
};

class LoginWindow;

class ArchipelagoHandler
{
public:
	static void Poll();
	static void ConnectAP(LoginWindow* login);
	static void DisconnectAP();
	static void Check(int64_t locationId);
	static void Release();
	static std::string GetItemName(int64_t itemId);
	static std::string GetPlayerAlias(int64_t playerId);
	static std::string GetLocationName(int64_t locId);
	static bool polling;
	static Goal goal;
	static bool deathlink;
	static bool startWithBoom;
	static LevelUnlockStyle levelUnlockStyle;
	static std::vector<int> portalMap;
	static std::vector<int> bossMap;
	static int theggGating;
	static int cogGating;
	static bool goalReqBosses;
	static bool gateTimeAttacks;
	static double deathtime;
	static Framesanity framesanity;
	static bool scalesanity;
	static bool progressiveRang;
	static bool progressiveLevel;
	static bool ap_sync_queued;
	static bool ap_connected;
	static bool advancedLogic;
	static std::string seed;
private:
	static std::unique_ptr<APClient> ap;
	static void SetAPStatus(std::string status, char important);
};

