#include <sstream>

#include "entity/Config.h"
#include "Db.h"

namespace greenthumb {
namespace entity {

const wxString Config::KEY_ACCOUNT_PAGE_SIZE = "accountPageSize";
const wxString Config::KEY_REFRESH_INTERVAL = "refreshInterval";

std::map<wxString, Config> Config::configCache = std::map<wxString, Config>();

Config::Config() : id(0) {
}

template <>
void Config::SetConfigValue(const wxString& configKey, const wxString& configValue) {

    try {

        Config config = GetConfig(configKey);

        config.configValue = configValue;
        config.Update();

        configCache[configKey] = config;

    } catch (std::exception const &e) {

        Config config;
        config.configKey = configKey;
        config.configValue = configValue;
        config.Insert();

        configCache[configKey] = config;
    }

}

template <>
void Config::SetConfigValue(const wxString& configKey, const bool& configValue) {
    wxString strValue = configValue ? "true" : "false";
    SetConfigValue(configKey, strValue);
}

template <>
void Config::SetConfigValue(const wxString& configKey, const int& configValue) {
    SetConfigValue(configKey, wxString::Format(wxT("%i"), configValue));
}

Config Config::GetConfig(const wxString& configKey) {

    std::map<wxString, Config>::iterator it = configCache.find(configKey);

    if (it == configCache.end()) {

        Config config;
        soci::indicator ind;

        soci::session session(Db::GetConnectionPool());

        session << "SELECT * FROM config WHERE config_key = :configKey LIMIT 1",
            soci::into(config, ind), soci::use(std::string(configKey.mb_str()));

        if (config.id > 0) {
            configCache[configKey] = config;
            return config;
        }

        throw std::out_of_range("configKey not found: " + configKey);

    } else {
        return it->second;
    }
}

template <>
wxString Config::GetConfigValue(const wxString& configKey, const wxString& defaultValue) {
    try {
        return GetConfigValue(configKey);
    } catch (const std::out_of_range& e) {
        Config config;
        config.configKey = configKey;
        config.configValue = defaultValue;
        config.Save();
        configCache[configKey] = config;
    }

    return defaultValue;
}

template <>
int Config::GetConfigValue(const wxString& configKey, const int& defaultValue) {

    try {
        int result = wxAtoi(GetConfigValue(configKey));
        return result;
    } catch (const std::exception& e) {
        Config config;
        config.configKey = configKey;
        config.configValue = wxString::Format(wxT("%i"), defaultValue);
        config.Save();

        configCache[configKey] = config;
    }

    return defaultValue;
}

template <>
bool Config::GetConfigValue(const wxString& configKey, const bool& defaultValue) {
    try {
        wxString boolValue = GetConfigValue(configKey);
        return boolValue == "true";
    } catch (std::exception const &e) {

        Config config;
        config.configKey = configKey;
        config.configValue = defaultValue ? "true" : "false";
        config.Save();

        configCache[configKey] = config;
    }

    return defaultValue;
}

wxString Config::GetConfigValue(const wxString& configKey) {
    Config config = GetConfig(configKey);
    return config.configValue;
}

void Config::Insert() {
    soci::session session(Db::GetConnectionPool());

    session << "INSERT INTO config (config_key, config_value) "
        "VALUES (:config_key, :config_value)", soci::use(std::string(configKey.mb_str())),
        soci::use(std::string(configValue.mb_str()));

    long lastInsertId;

    session << "select last_insert_rowid()", soci::into(lastInsertId);
    id = static_cast<unsigned>(lastInsertId);
    // get_last_insert_id() not implemented in sqlite3 backend in SOCI version 3.2.3
    /* if (session.get_last_insert_id("config", lastInsertId)) {
        id = static_cast<unsigned>(lastInsertId);
    } */
}

void Config::Update() {
    soci::session session(Db::GetConnectionPool());
    session << "UPDATE config SET config_value = :config_value WHERE id = :id",
        soci::use(std::string(configValue.mb_str())), soci::use(id);
}

void Config::Save() {
    if (id > 0) {
        Update();
    } else {
        Insert();
    }
}

void Config::SetId(const unsigned id) {
    this->id = id;
}
const int Config::GetId() const {
    return id;
}

void Config::SetConfigKey(const wxString& configKey) {
    this->configKey = configKey;
}
const wxString& Config::GetConfigKey() const {
    return configKey;
}

void Config::SetConfigValue(const wxString& configValue) {
    this->configValue = configValue;
}
const wxString& Config::GetConfigValue() const {
    return configValue;
}

}
}
