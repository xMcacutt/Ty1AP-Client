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
	ALL_BOSSES,
	ALL_THEGGS,
	COMPLETION
};

enum class Cogsanity {
	EACH_COG,
	PER_LEVEL,
	NONE
};

enum class Bilbysanity {
	ALL_NO_THEGG,
	ALL_WITH_THEGG,
	NONE
};

enum class Attributesanity {
	SKIP_ELEMENTALS,
	ALL,
	NONE
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
	static ArchipelagoHandler& get() {
		if (!instance) {
			instance = new ArchipelagoHandler();
		}
		return *instance;
	}
	static ArchipelagoHandler* instance;
	bool polling;
	void Poll();
	void ConnectAP(LoginWindow* login);
	void DisconnectAP(LoginWindow* login);
	void Check(int64_t locationId);
	Goal goal;
	bool deathlink;
	bool startWithBoom;
	LevelUnlockStyle levelUnlockStyle;
	std::vector<int> portalMap;
	std::vector<int> bossMap;
	int hubTheggCounts;
	double deathtime;
	Cogsanity cogsanity;
	Bilbysanity bilbysanity;
	Attributesanity attributesanity;
	Framesanity framesanity;
	bool progressiveRang;
	bool progressiveLevel;
	bool ap_sync_queued;
	bool ap_connected;
	int levelA1;
	int levelA2;
	int levelA3;
	int levelA4;
	int levelB1;
	int levelB2;
	int levelB3;
	int levelD4;
	int levelC1;
	int levelC2;
	int levelC3;
	int levelC4;
private:
	std::unique_ptr<APClient> ap;
	void SetAPStatus(std::string status, char important);
	ArchipelagoHandler();
	ArchipelagoHandler(const ArchipelagoHandler&) = delete;
	ArchipelagoHandler& operator=(const ArchipelagoHandler&) = delete;
};

