#pragma once
#include "pch.h"
#include "ArchipelagoHandler.h"
#include "apuuid.hpp"

using nlohmann::json;
bool ap_connect_sent = false;
bool is_https = false; 
bool is_wss = false; 
bool is_ws = false; 
unsigned connect_error_count = 0;
bool awaiting_password = false;
std::string ArchipelagoHandler::slot = "Tyrone";
int nextCheckToGet = 0;
bool ArchipelagoHandler::ap_sync_queued = false;
bool ArchipelagoHandler::ap_connected = false;
bool ArchipelagoHandler::polling = false;
std::string ArchipelagoHandler::uuid = "";
Goal ArchipelagoHandler::goal = Goal::BEAT_CASS;
bool ArchipelagoHandler::deathlink = false;
bool ArchipelagoHandler::multylink = false;
bool ArchipelagoHandler::someoneElseDied = false;
LevelUnlockStyle ArchipelagoHandler::levelUnlockStyle = LevelUnlockStyle::VANILLA;
int ArchipelagoHandler::theggGating = 17;
int ArchipelagoHandler::cogGating = 10;
double ArchipelagoHandler::deathtime = -1;
Framesanity ArchipelagoHandler::framesanity = Framesanity::NONE;
bool ArchipelagoHandler::scalesanity = false;
bool ArchipelagoHandler::signsanity = false;
bool ArchipelagoHandler::lifesanity = false;
bool ArchipelagoHandler::opalsanity = false;
bool ArchipelagoHandler::progressiveRang = false;
bool ArchipelagoHandler::progressiveLevel = false;
bool ArchipelagoHandler::gateTimeAttacks = false;
bool ArchipelagoHandler::goalReqBosses = false;
bool ArchipelagoHandler::advancedLogic = false;
std::unordered_map<int, int> ArchipelagoHandler::portalMap;
std::unordered_map<int, int> ArchipelagoHandler::inversePortalMap;
std::vector<std::string> ArchipelagoHandler::koalaMapping;
std::map<int, double> ArchipelagoHandler::koalaConnected;
int ArchipelagoHandler::koalaIndex = -1;
std::string ArchipelagoHandler::mulTyName = "";
std::unique_ptr<APClient> ArchipelagoHandler::ap;
std::string ArchipelagoHandler::seed;

bool isEqual(double a, double b)
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon() * fmax(fabs(a), fabs(b));
}

void ArchipelagoHandler::DisconnectAP() {
    LoggerWindow::Log("Disconnected");
    SetAPStatus("Disconnected", 1);
    polling = false;
    MulTyHandler::IsRunning = false;
    ap_connect_sent = false;
    ap_connected = false;
    if (GameState::onLoadScreenOrMainMenu())
        GameHandler::SetLoadActive(false);
    else
        GameState::forceMainMenu();
}

void ArchipelagoHandler::ConnectAP(LoginWindow* login)
{
    ap.reset();

    std::string uri = login->server;
    is_ws = uri.rfind("ws://", 0) == 0;
    is_wss = uri.rfind("wss://", 0) == 0;

    uuid = ap_get_uuid(UUID_FILE,
        uri.empty() ? APClient::DEFAULT_URI :
        is_ws ? uri.substr(5) :
        is_wss ? uri.substr(6) :
        uri);

    deathlink = false;

    #ifdef __EMSCRIPTEN__
        if (is_https && is_ws) {
            EM_ASM({
                throw 'WS not supported in HTTPS context, only WSS is allowed';
                });
        }
        else if (is_https && !is_wss) {
            uri = "wss://" + uri;
    }
    #endif

    ap.reset(new APClient(uuid, GAME_NAME, uri.empty() ? APClient::DEFAULT_URI : uri, CERT_STORE));
    polling = true;
    API::LogPluginMessage("Connecting to AP, server " + uri + "\n", LogLevel::Info);
    SetAPStatus("Connecting", 1);

    ap_sync_queued = false;
    ap->set_socket_connected_handler([login]() {
        login->SetMessage("Connected, authenticating...");
        SetAPStatus("Authenticating", 1);
    });
    
    ap->set_socket_disconnected_handler([login]() {
        login->SetMessage("");
        LoggerWindow::Log("Disconnected");
        SetAPStatus("Disconnected", 1);
        polling = false;
        MulTyHandler::IsRunning = false;
        ap_connect_sent = false;
        ap_connected = false;
        if (GameState::onLoadScreenOrMainMenu())
            GameHandler::SetLoadActive(false);
        else
            GameState::forceMainMenu();
    });
    ap->set_room_info_handler([login]() {
        login->SetMessage("Room info received");
        ap->ConnectSlot(login->slot, login->password, 0b111, {}, { 0,6,0 });
        ap_connect_sent = true;
    });
    ap->set_slot_connected_handler([login](const json& data) {
        ap_connected = true;

        if (data.find("ModVersion") != data.end() || data["ModVersion"] != "1.3.0")
            LoggerWindow::Log("Your client and apworld versions do not match. This might be fine but please check for updates.");

        if (data.find("Goal") != data.end() && data["Goal"].is_number_integer())
            goal = static_cast<Goal>(data["Goal"].get<int>());

        if (data.find("LevelUnlockStyle") != data.end() && data["LevelUnlockStyle"].is_number_integer())
            levelUnlockStyle = static_cast<LevelUnlockStyle>(data["LevelUnlockStyle"].get<int>());
        else levelUnlockStyle = LevelUnlockStyle::VANILLA;

        portalMap.clear();
        inversePortalMap.clear();
        if (data.find("PortalMap") != data.end() && data["PortalMap"].is_array()) {
            auto portalList = data["PortalMap"].get<std::vector<int>>();
            portalMap[4] = portalList[0];
            portalMap[5] = portalList[1];
            portalMap[6] = portalList[2];
            portalMap[8] = portalList[3];
            portalMap[9] = portalList[4];
            portalMap[10] = portalList[5];
            portalMap[12] = portalList[6];
            portalMap[13] = portalList[7];
            portalMap[14] = portalList[8];
            for (const auto& pair : portalMap) {
                inversePortalMap[pair.second] = pair.first;
            }
        }

        if (data.find("DeathLink") != data.end())
            deathlink = data["DeathLink"].get<int>() == 1;;
        if (data.find("MulTyLink") != data.end())
            multylink = data["MulTyLink"].get<int>() == 1;;

        if (data.find("ProgressiveLevel") != data.end())
            progressiveLevel = data["ProgressiveLevel"].get<int>() == 1;;

        if (data.find("ProgressiveElementals") != data.end())
            progressiveRang = data["ProgressiveElementals"].get<int>() == 1;;

        if (data.find("TheggGating") != data.end() && data["TheggGating"].is_number_integer())
            theggGating = data["TheggGating"].get<int>();

        if (data.find("CogGating") != data.end() && data["CogGating"].is_number_integer())
            cogGating = data["CogGating"].get<int>();

        if (data.find("GateTimeAttacks") != data.end())
            gateTimeAttacks = data["GateTimeAttacks"].get<int>() == 1;;
        
        if (data.find("ReqBosses") != data.end())
            goalReqBosses = data["ReqBosses"].get<int>() == 1;

        if (data.find("Framesanity") != data.end() && data["Framesanity"].is_number_integer()) 
            framesanity = (Framesanity)data["Framesanity"].get<int>();

        if (data.find("Scalesanity") != data.end())
            scalesanity = data["Scalesanity"].get<int>() == 1;

        if (data.find("Lifesanity") != data.end())
            lifesanity = data["Lifesanity"].get<int>() == 1;

        if (data.find("Signsanity") != data.end())
            signsanity = data["Signsanity"].get<int>() == 1;

        if (data.find("Opalsanity") != data.end())
            opalsanity = data["Opalsanity"].get<int>() == 1;

        if (data.find("AdvancedLogic") != data.end() && data["AdvancedLogic"].is_number_integer())
            advancedLogic = data["AdvancedLogic"].get<int>() == 1;

        std::list<std::string> tags = {};
        if (deathlink)
            tags.push_back("DeathLink");
        
        ap->ConnectUpdate(false, 0b111, true, tags);
        ap->StatusUpdate(APClient::ClientStatus::PLAYING);
        seed = ap->get_seed();
        slot = ap->get_slot();
        if (multylink) {
            tags.push_back("MulTyLink");
            mulTyName = login->mulTyName;
            ReadKoala();
        }
        if (ArchipelagoHandler::LoadSaveData()) {
            GameHandler::SetLoadActive(true);
        }
        GameHandler::SetupOnConnect();

        SetAPStatus("Connected", 0);
    });
    ap->set_slot_disconnected_handler([login]() {
        login->SetMessage("");
        LoggerWindow::Log("Disconnected");
        SetAPStatus("Disconnected", 1);
        ap_connect_sent = false;
        ap_connected = false;
        if (GameState::onLoadScreenOrMainMenu())
            GameHandler::SetLoadActive(false);
        else
            GameState::forceMainMenu();
    });
    ap->set_items_received_handler([](const std::list<APClient::NetworkItem>& items) {
        if (!ap->is_data_package_valid()) {
            if (!ap_sync_queued) ap->Sync();
            ap_sync_queued = true;
            return;
        }
        for (const auto& item : items) {
            ItemHandler::HandleItem(item);
        }
    });
    ap->set_location_checked_handler([](const std::list<int64_t> locations){
        for (auto& location : locations) {
            LocationHandler::HandleLocation(location);
        }
    });
    ap->set_print_handler([](const std::string& msg) {
        LoggerWindow::Log(msg);
    });
    ap->set_print_json_handler([](const std::list<APClient::TextNode>& msg) {
        LoggerWindow::Log(ap->render_json(msg, APClient::RenderFormat::TEXT));
     });
    ap->set_bounced_handler([](const json& cmd) {
        auto tagsIt = cmd.find("tags");
        auto dataIt = cmd.find("data");
        if (multylink) {
            if (tagsIt != cmd.end() && tagsIt->is_array()
                && std::find(tagsIt->begin(), tagsIt->end(), "MulTyLink") != tagsIt->end())
            {
                if (dataIt != cmd.end() && dataIt->is_object()) {
                    json data = *dataIt;
                    if (data["source"].get<std::string>() != mulTyName) {
                        std::string source = data["source"].get<std::string>().c_str();
                        int level = data["level"].is_number_integer() ? data["level"].get<int>() : -1;
                        if ((int)Level::getCurrentLevel() == level)
                        {
                            if (data.find("pos") != data.end() && data["pos"].is_array()) {
                                auto pos = data["pos"].get<std::vector<float>>();
                                auto it = std::find(koalaMapping.begin(), koalaMapping.end(), source);

                                if (it != koalaMapping.end()) {
                                    int index = std::distance(koalaMapping.begin(), it);
                                    koalaConnected[index] = ap->get_server_time();
                                    MulTyHandler::HandlePosData(level, index, pos);
                                }
                            }
                        }
                        else {
                            auto it = std::find(koalaMapping.begin(), koalaMapping.end(), source);

                            if (it != koalaMapping.end()) {
                                int index = std::distance(koalaMapping.begin(), it);
                                MulTyHandler::DisableDraw(index);
                            }
                            
                        }

                    }
                }
                else {
                    LoggerWindow::Log("Bad MulTy Pos");
                }
            }
        }
        if (deathlink) {
            if (tagsIt != cmd.end() && tagsIt->is_array()
                && std::find(tagsIt->begin(), tagsIt->end(), "DeathLink") != tagsIt->end())
            {
                if (dataIt != cmd.end() && dataIt->is_object()) {
                    json data = *dataIt;
                    if (data["source"].get<std::string>() != slot) {
                        std::string source = data["source"].is_string() ? data["source"].get<std::string>().c_str() : "???";
                        std::string cause = data["cause"].is_string() ? data["cause"].get<std::string>().c_str() : "???";
                        LoggerWindow::Log("[color = FFFFFFFF]" + cause);
                        someoneElseDied = true;
                        Hero::kill();
                    }
                }
                else {
                    LoggerWindow::Log("Bad Deathlink");
                }
            }
        }
    });
    ap->set_retrieved_handler([](const std::map<std::string, json>& data, const json& message) {
        auto koalakey = "ty1_koalaId_" + std::to_string(ap->get_team_number()) + "_" + slot;
        ap->SetNotify({ koalakey });
        bool hasIndex = false;
        for (const auto& [key, value_json] : data) {
            if (koalakey == key) {
                if (value_json.is_null()) {
                    koalaMapping = {};
                    continue;
                }

                try {
                    std::vector<std::string> values = value_json.get<std::vector<std::string>>();
                    for (int i = 0; i < values.size(); i++) {
                        if (values[i] == mulTyName) {
                            hasIndex = true;
                            koalaIndex = i;
                            break;
                        }
                    }
                    koalaMapping = values;
                }
                catch (const std::exception& e) {
                    API::LogPluginMessage("Error parsing value for key " + key + ": " + e.what());
                }
            }
        }
        if (koalaMapping.size() < 8) {
            if (!hasIndex) {
                koalaIndex = koalaMapping.size();
                koalaMapping.push_back(mulTyName);
                UpdateKoalaIndex();
            }
        }
        LoggerWindow::Log("Joined. Total Users Joined : " + std::to_string(koalaMapping.size()));
    });

    ap->set_set_reply_handler([](const json& command) {
        if (!command.contains("key") || !command.contains("value"))
            return;
        auto key = "ty1_koalaId_" + std::to_string(ap->get_team_number()) + "_" + slot;
        if (command["key"] == key){
            std::vector<std::string> values;
            try {
                values = command["value"].get<std::vector<std::string>>();
            }
            catch (const std::exception& e) {
                API::LogPluginMessage("Failed to parse value as vector<string>: " + std::string(e.what()));
                return;
            }

            if (koalaIndex < koalaMapping.size() && koalaIndex < values.size()) {
                if (koalaMapping[koalaIndex] == values[koalaIndex]) {
                    std::string newnames = "";
                    for (int i = 0; i < values.size(); i++) {
                        if (koalaMapping.size() > i && koalaMapping[i] != values[i]) {
                            newnames = newnames + values[i] + " ";
                        }
                        else {
                            newnames = newnames + values[i] + " ";
                        }
                    }
                    koalaMapping = values;
                    LoggerWindow::Log(newnames + "Joined. Total Koalas Joined: " + std::to_string(koalaMapping.size()));
                }
                else if (koalaMapping[koalaIndex] < values[koalaIndex] && koalaMapping.size() < 8) {
                    koalaMapping = values;
                    //API::LogPluginMessage("Uh oh I'm in charge of fixing this");
                    koalaIndex = koalaMapping.size();
                    koalaMapping.push_back(mulTyName);
                    UpdateKoalaIndex();
                }
            }
            else {
                API::LogPluginMessage("Invalid koalaIndex detected: out of bounds");
            }
        }
    });
}

void ArchipelagoHandler::Check(int64_t locationId) {
    std::list<int64_t> check;
    check.push_back(locationId);
    ap->LocationChecks(check);
}

void ArchipelagoHandler::Release() {
    ap->StatusUpdate(APClient::ClientStatus::GOAL);
}

std::string ArchipelagoHandler::GetItemName(int64_t itemId) {
    return ap->get_item_name(itemId, GAME_NAME);
}

std::string ArchipelagoHandler::GetPlayerAlias(int64_t playerId) {
    return ap->get_player_alias(playerId);
}

std::string ArchipelagoHandler::GetLocationName(int64_t locId) {
    return ap->get_location_name(locId, GAME_NAME);
}

void ArchipelagoHandler::Poll() {
    if (polling)
        ap->poll();
}

void ArchipelagoHandler::SetAPStatus(std::string status, char important) {
    API::LogPluginMessage("AP status: " + status);
    //apLastStatusUpdate = important ? 0 : time(NULL);
}

bool ArchipelagoHandler::LoadSaveData() {
    return SaveDataHandler::LoadSaveData(seed, slot);
}

const std::vector<std::string> deathCauses{
    "didn\'t catch the boomerang",
    "bit off more than they could chew",
    "turned into a tasmanian fossil",
    "forgot to wear gloves",
    "had trouble in the outhouse",
    "couldn\'t hold their breath",
    "listened to Lenny",
    "got crushed by an icicle",
    "forgot how to glide",
    "bit the ground too hard",
    "fell over",
    "got a little too Australian",
    "didn\'t wear sunblock",
    "didn\'t listen to Lenny",
    "ate too much pie",
    "hugged a crocodile",
    "got lost in the outback",
    "died",
    "forgot eels were electric",
    "got too confident with the didgeridoo",
    "tried to ride a stingray",
    "told a dingo it wasn\'t that scary",
    "got dropkicked by a kangaroo",
    "said it wasn't their fault",
    "blames Boonie",
    "miscounted the multirangs",
    "licked a toad for science",
    "ran out of \'she\'ll be right\' energy mid-jump",
    "tried to code an AP game"
};

std::string GetRandomCause() {
    if (deathCauses.empty()) return ""; // Handle empty list case
    std::random_device rd;  // Obtain a random seed
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_int_distribution<int> dist(0, deathCauses.size() - 1);
    return deathCauses[dist(gen)];
}

void ArchipelagoHandler::SendDeath() {
    std::string cause = GetRandomCause();
    LoggerWindow::Log("Death Sent:  " + slot + " " + cause);
    json data{
        {"time", ap->get_server_time()},
        {"cause", slot + " " + GetRandomCause()},
        {"source", slot},
    };
    ap->Bounce(data, {}, {}, { "DeathLink" });
}

void ArchipelagoHandler::SendPosition(int level, std::vector<float> pos) {
    if (koalaIndex == -1) {
        return;
    }
    json data{
        {"level", level},
        {"source", mulTyName},
        {"pos", pos},
    };
    ap->Bounce(data, { "Ty the Tasmanian Tiger" }, { ap->get_player_number()}, {"MulTyLink"});
}

void ArchipelagoHandler::SendLevel(int levelId) {
    APClient::DataStorageOperation operation;
    operation.operation = "replace";
    operation.value = levelId;
    auto key = "ty1_level_" + std::to_string(ap->get_team_number()) + "_" + slot;
    ap->Set(key, 0, false, { operation });
}

void ArchipelagoHandler::ReadKoala() {
    auto key = "ty1_koalaId_" + std::to_string(ap->get_team_number()) + "_" + slot;
    ap->Get({ key });
}

void ArchipelagoHandler::UpdateKoalaIndex() {
    APClient::DataStorageOperation operation;
    operation.operation = "replace";
    operation.value = koalaMapping;
    auto key = "ty1_koalaId_" + std::to_string(ap->get_team_number()) + "_" + slot;
    ap->Set(key, {}, true, { operation });
}

void ArchipelagoHandler::TryClaimKoalaIndex() {
    if (!multylink)
        return;

    if (koalaIndex >= 0) {
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (koalaConnected[i] < ap->get_server_time() - 10000) {
            koalaMapping[i] = mulTyName;
            koalaIndex = i;
            API::LogPluginMessage("I Claim Koala: " + i);
            UpdateKoalaIndex();
            break;
        }
    }
}