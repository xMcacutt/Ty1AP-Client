#pragma once
#include "apclient.hpp"
#include "GameHandler.h"
#include "nlohmann/json.hpp"
#include "TygerFrameworkAPI.hpp"
#include "gui.h"
#include "LoginWindow.h"
#include <inttypes.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>
#else
#define EM_BOOL bool
#define EM_TRUE true
#define EM_FALSE false
#if !defined WIN32 && !defined _WIN32
#include <poll.h>
#endif
#endif
#include <limits>

#if defined(WIN32) && !defined(PRId64 )
#define PRId64 "I64d"
#endif

#define VERSION_TUPLE {0,5,1}

#ifdef __EMSCRIPTEN__
#define VIRTUAL_HOME_DIR "/settings"
#define OLD_DATAPACKAGE_CACHE "/settings/datapackage.json"
#define UUID_FILE "/settings/uuid"
#define CERT_STORE "" // not required in a browser context
#else
#define OLD_DATAPACKAGE_CACHE "datapackage.json"
#define UUID_FILE "uuid" // TODO: place in %appdata%
#define CERT_STORE "cacert.pem" // SSL certificate store for non-browser contexts
#endif

#define GAME_NAME "Ty the Tasmanian Tiger"
#define AP_OFFSET 8750000

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
	static bool LoadSaveData();
	static void SendDeath();
	static void SendLevel(int levelId);
	static std::string GetItemName(int64_t itemId);
	static std::string GetPlayerAlias(int64_t playerId);
	static std::string GetLocationName(int64_t locId);
	static bool polling;
	static Goal goal;
	static bool deathlink;
	static LevelUnlockStyle levelUnlockStyle;
	static std::unordered_map<int, int> portalMap;
	static std::unordered_map<int, int> inversePortalMap;
	static int theggGating;
	static int cogGating;
	static bool someoneElseDied;
	static bool goalReqBosses;
	static bool gateTimeAttacks;
	static double deathtime;
	static Framesanity framesanity;
	static bool scalesanity;
	static bool signsanity;
	static bool lifesanity;
	static bool opalsanity;
	static bool progressiveRang;
	static bool progressiveLevel;
	static bool ap_sync_queued;
	static bool ap_connected;
	static bool advancedLogic;
	static std::string seed;
	static std::string slot;
private:
	static std::unique_ptr<APClient> ap;
	static void SetAPStatus(std::string status, char important);
};

