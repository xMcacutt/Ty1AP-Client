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

std::unique_ptr<APClient> ap;
bool ap_sync_queued = false;
bool ap_connect_sent = false;
bool ap_connected = false;
bool is_https = false; 
bool is_wss = false; 
bool is_ws = false; 
unsigned connect_error_count = 0;
bool awaiting_password = false;
std::string slot = "Player";
int nextCheckToGet = 0;

Goal goal = Goal::BEAT_CASS;
bool deathlink = false;
bool startWithBoom = true;
LevelUnlockStyle levelUnlockStyle = LevelUnlockStyle::VANILLA;
std::vector<int> portalMap = std::vector<int>();
std::vector<int> bossMap = std::vector<int>();
int hubTheggCounts = 17;
double deathtime = -1;

bool isEqual(double a, double b)
{
    return fabs(a - b) < std::numeric_limits<double>::epsilon() * fmax(fabs(a), fabs(b));
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
        // If running in a browser context, enforce wss:// for secure connections
        if (is_https && is_ws) {
            EM_ASM({
                throw 'WS not supported in HTTPS context, only WSS is allowed';
                });
        }
        else if (is_https && !is_wss) {
            uri = "wss://" + uri; // force wss:// if it's an HTTPS context
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

    // set state and callbacks
    ap_sync_queued = false;
    ap->set_socket_connected_handler([login]() {
        login->SetMessage("Connected, authenticating...");
        SetAPStatus("Authenticating", 1);
    });

    ap->set_socket_disconnected_handler([login]() {
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

    ap->set_room_info_handler([login]() {
        login->SetMessage("Room info received");
        ap->ConnectSlot(login->slot, login->password, 0b111, {  }, { 0,5,1 });
        ap_connect_sent = true;
    });

    ap->set_slot_connected_handler([](const json& data) {
        ap_connected = true;

        if (data.find("DeathLink") != data.end() && data["DeathLink"].is_boolean())
            deathlink = data["DeathLink"].get<bool>();

        if (data.find("Goal") != data.end() && data["Goal"].is_number_integer())
            goal = static_cast<Goal>(data["Goal"].get<int>());

        if (data.find("LevelUnlockStyle") != data.end() && data["LevelUnlockStyle"].is_number_integer())
            levelUnlockStyle = static_cast<LevelUnlockStyle>(data["LevelUnlockStyle"].get<int>());
        else levelUnlockStyle = LevelUnlockStyle::VANILLA;

        if (data.find("PortalMap") != data.end() && data["PortalMap"].is_array())
            for (int code : data["PortalMap"])
                portalMap.push_back(static_cast<int>(code));

        if (data.find("BossMap") != data.end() && data["BossMap"].is_array())
            for (int code : data["BossMap"])
                bossMap.push_back(static_cast<int>(code));

        bool progElementals = false;
        if (data.find("ProgressiveElementals") != data.end() && data["ProgressiveElementals"].is_boolean())
            progElementals = data["ProgressiveElementals"].get<bool>();
        if (data.find("StartWithBoom") != data.end() && data["StartWithBoom"].is_boolean())
            startWithBoom = data["StartWithBoom"].get<bool>();
        startWithBoom = !(progElementals && !startWithBoom);

        if (data.find("HubTheggCounts") != data.end() && data["HubTheggCounts"].is_number_integer())
            hubTheggCounts = data["HubTheggCounts"].get<int>();

        if (deathlink) ap->ConnectUpdate(false, 0b111, true, { "DeathLink" });
        ap->StatusUpdate(APClient::ClientStatus::PLAYING);

        GameHandler::SetupOnConnect(ap->get_seed());

        LoggerWindow::Log("Connected as " + ap->get_player_alias(ap->get_player_number()));

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

    ap->set_slot_refused_handler([login](const std::list<std::string>& errors) {
        ap_connect_sent = false;
        if (std::find(errors.begin(), errors.end(), "InvalidSlot") != errors.end()) {
            LoggerWindow::Log("Invalid slot specified");
            login->SetMessage("Invalid slot specified");
        }
        else {
            LoggerWindow::Log("Connection refused: ");
            for (const auto& error : errors)
                LoggerWindow::Log(error);
            SetAPStatus(errors.front().c_str(), 1);
        }
    });

    ap->set_items_received_handler([](const std::list<APClient::NetworkItem>& items) {
        auto me = ap->get_player_number();
        if (!ap->is_data_package_valid()) {
            if (!ap_sync_queued) 
                ap->Sync();
            ap_sync_queued = true;
            return;
        }
        for (const auto& item : items) {
            auto itemname = ap->get_item_name(item.item, ap->get_player_game(item.player));
            auto sender = item.player ? (ap->get_player_alias(item.player) + "'s world") : "out of nowhere";
            auto location = ap->get_location_name(item.location);

            printf("  #%d: %s (%" PRId64 ") from %s - %s\n",
                item.index, itemname.c_str(), item.item,
                sender.c_str(), location.c_str());

            if (item.index < nextCheckToGet) {
                printf("Ignoring\n");
                continue;
            }
            nextCheckToGet = item.index + 1;

            // HANDLE ITEMS
            //ItemHandler->receivedItemsQueue.push_front(item);
        }
    });

    //ap->set_location_info_handler([](const std::list<APClient::NetworkItem>& items) {
    //    auto me = ap->get_player_number();
    //    for (auto item : items) {
    //        if (item.player != me) {
    //            auto itemname = ap->get_item_name(item.item, ap->get_player_game(item.player));
    //            auto recipient = ap->get_player_alias(item.player);

    //            ReportSentItem(IDToLocation(item.location), recipient.c_str(), itemname.c_str());
    //        }
    //    }
    //});

    ap->set_data_package_changed_handler([](const json& data) {
        // NOTE: what to do when the data package changes (probably doesn't need to change this code)
        FILE* f = fopen(OLD_DATAPACKAGE_CACHE, "wb");
        if (f) {
            std::string s = data.dump();
            fwrite(s.c_str(), 1, s.length(), f);
            fclose(f);
        }
    });

    ap->set_print_handler([](const std::string& msg) {
        LoggerWindow::Log(msg);
    });

    ap->set_print_json_handler([](const std::list<APClient::TextNode>& msg) {
        printf("%s\n", ap->render_json(msg, APClient::RenderFormat::ANSI).c_str());
    });

    ap->set_bounced_handler([](const json& cmd) {
        if (deathlink) {
            auto tagsIt = cmd.find("tags");
            auto dataIt = cmd.find("data");
            if (tagsIt != cmd.end() && tagsIt->is_array()
                && std::find(tagsIt->begin(), tagsIt->end(), "DeathLink") != tagsIt->end())
            {
                if (dataIt != cmd.end() && dataIt->is_object()) {
                    json data = *dataIt;
                    if (data["time"].is_number() && isEqual(data["time"].get<double>(), deathtime)) {
                        deathtime = -1;
                    }
                    else {
                        //LoggerWindow::Log()
                        Hero::kill();
                    }
                }
                else {
                    printf("Bad deathlink packet!\n");
                }
            }
        }
    });
}

void ArchipelagoHandler::Poll() {
    if (polling)
        ap->poll();
}

void ArchipelagoHandler::SetAPStatus(std::string status, char important) {
    API::LogPluginMessage("AP status: " + status);
    //apLastStatusUpdate = important ? 0 : time(NULL);
}

bool ArchipelagoHandler::IsConnected() {
    return ap_connected;
}