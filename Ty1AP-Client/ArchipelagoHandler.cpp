#include "ArchipelagoHandler.h"
#include "apuuid.hpp"
#include <stdio.h>
#include <inttypes.h>
#include <algorithm>
#include <memory>
#include "hero.h"
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
#include <math.h>
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

using nlohmann::json;
bool ap_connect_sent = false;
bool is_https = false; 
bool is_wss = false; 
bool is_ws = false; 
unsigned connect_error_count = 0;
bool awaiting_password = false;
std::string slot = "Player";
int nextCheckToGet = 0;
ArchipelagoHandler* ArchipelagoHandler::instance = nullptr;


ArchipelagoHandler::ArchipelagoHandler() {
    bool ap_sync_queued = false;
    bool ap_connected = false;
    bossMap.clear();
    portalMap.clear();
    goal = Goal::BEAT_CASS;
    deathlink = false;
    startWithBoom = true;
    levelUnlockStyle = LevelUnlockStyle::VANILLA;
    hubTheggCounts = 17;
    deathtime = -1;
    cogsanity = Cogsanity::EACH_COG;
    bilbysanity = Bilbysanity::ALL_NO_THEGG;
    attributesanity = Attributesanity::SKIP_ELEMENTALS;
    framesanity = Framesanity::NONE;
    progressiveRang = false;
    progressiveLevel = false;
}

bool isEqual(double a, double b)
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon() * fmax(fabs(a), fabs(b));
}

void ArchipelagoHandler::DisconnectAP(LoginWindow* login) {
    LoggerWindow::Log("Disconnected");
    SetAPStatus("Disconnected", 1);
    polling = false;
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

    std::string uuid = ap_get_uuid(UUID_FILE,
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
    try {
        ap->set_data_package_from_file(OLD_DATAPACKAGE_CACHE);
    }
    catch (std::exception) { /* ignore */ }

    ap_sync_queued = false;
    ap->set_socket_connected_handler([login, this]() {
        login->SetMessage("Connected, authenticating...");
        SetAPStatus("Authenticating", 1);
    });

    ap->set_socket_disconnected_handler([login, this]() {
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

    ap->set_room_info_handler([login, this]() {
        login->SetMessage("Room info received");
        ap->ConnectSlot(login->slot, login->password, 0b111, {}, { 0,5,1 });
        ap_connect_sent = true;
    });

    ap->set_slot_connected_handler([this](const json& data) {
        ap_connected = true;

        if (data.find("DeathLink") != data.end() && data["DeathLink"].is_boolean())
            deathlink = data["DeathLink"].get<bool>();

        if (data.find("Goal") != data.end() && data["Goal"].is_number_integer())
            goal = static_cast<Goal>(data["Goal"].get<int>());

        if (data.find("LevelUnlockStyle") != data.end() && data["LevelUnlockStyle"].is_number_integer())
            levelUnlockStyle = static_cast<LevelUnlockStyle>(data["LevelUnlockStyle"].get<int>());
        else levelUnlockStyle = LevelUnlockStyle::VANILLA;

        if (data.find("PortalMap") != data.end() && data["PortalMap"].is_array()) {
            levelA1 = data["PortalMap"].get<std::vector<int>>()[0];
            levelA2 = data["PortalMap"].get<std::vector<int>>()[1];
            levelA3 = data["PortalMap"].get<std::vector<int>>()[2];
            levelB1 = data["PortalMap"].get<std::vector<int>>()[3];
            levelB2 = data["PortalMap"].get<std::vector<int>>()[4];
            levelB3 = data["PortalMap"].get<std::vector<int>>()[5];
            levelC1 = data["PortalMap"].get<std::vector<int>>()[6];
            levelC2 = data["PortalMap"].get<std::vector<int>>()[7];
            levelC3 = data["PortalMap"].get<std::vector<int>>()[8];
        }

        if (data.find("BossMap") != data.end() && data["BossMap"].is_array()) {
            levelA4 = data["BossMap"].get<std::vector<int>>()[0];
            levelD4 = data["BossMap"].get<std::vector<int>>()[1];
            levelC4 = data["BossMap"].get<std::vector<int>>()[2];
        }

        if (data.find("DeathLink") != data.end() && data["DeathLink"].is_boolean())
            deathlink = data["DeathLink"].get<bool>();

        if (data.find("ProgressiveLevel") != data.end() && data["ProgressiveLevel"].is_boolean())
            progressiveLevel = data["ProgressiveLevel"].get<bool>();

        if (data.find("ProgressiveElementals") != data.end() && data["ProgressiveElementals"].is_boolean())
            progressiveRang = data["ProgressiveElementals"].get<bool>();
        if (data.find("StartWithBoom") != data.end() && data["StartWithBoom"].is_boolean())
            startWithBoom = data["StartWithBoom"].get<bool>();
        startWithBoom = !(progressiveRang && !startWithBoom);

        if (data.find("HubTheggCounts") != data.end() && data["HubTheggCounts"].is_number_integer())
            hubTheggCounts = data["HubTheggCounts"].get<int>();

        if (deathlink) ap->ConnectUpdate(false, 0b111, true, { "DeathLink" });
        ap->StatusUpdate(APClient::ClientStatus::PLAYING);

        GameHandler::SetupOnConnect(ap->get_seed());

        LoggerWindow::Log("Connected as " + ap->get_player_alias(ap->get_player_number()));

        SetAPStatus("Connected", 0);
    });

    ap->set_slot_disconnected_handler([login, this]() {
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
}

void ArchipelagoHandler::Check(int64_t locationId)
{
    std::list<int64_t> check;
    check.push_back(locationId);
    ap->LocationScouts(check);
    ap->LocationChecks(check);
}

void ArchipelagoHandler::Poll() {
    if (polling)
        ap->poll();
}

void ArchipelagoHandler::SetAPStatus(std::string status, char important) {
    API::LogPluginMessage("AP status: " + status);
    //apLastStatusUpdate = important ? 0 : time(NULL);
}