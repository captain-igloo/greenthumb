#include <sstream>

#include "entity/Config.h"
#include "Db.h"

namespace greenthumb {
namespace entity {

std::map<std::string, Config> Config::configCache = std::map<std::string, Config>();

Config::Config() : id(0) {
}

template <>
void Config::SetConfigValue(const std::string& configKey, const std::string& configValue) {

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
void Config::SetConfigValue(const std::string& configKey, const bool& configValue) {
    std::string strValue = configValue ? "true" : "false";
    SetConfigValue(configKey, strValue);
}

template <>
void Config::SetConfigValue(const std::string& configKey, const int& configValue) {
    std::ostringstream intStream;
    intStream << configValue;
    SetConfigValue(configKey, intStream.str());

}

Config Config::GetConfig(const std::string& configKey) {

    std::map<std::string, Config>::iterator it = configCache.find(configKey);

    if (it == configCache.end()) {

        Config config;
        soci::indicator ind;

        soci::session session(Db::GetConnectionPool());

        session << "SELECT * FROM config WHERE config_key = :configKey LIMIT 1", soci::into(config, ind), soci::use(configKey);

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
std::string Config::GetConfigValue(const std::string& configKey, const std::string& defaultValue) {

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
int Config::GetConfigValue(const std::string& configKey, const int& defaultValue) {

    try {
        std::istringstream intStream(GetConfigValue(configKey));
        int result;

        if (!(intStream >> result)) {
            return 0;
        }

        return result;

    } catch (const std::exception& e) {

        std::ostringstream intStream;
        intStream << defaultValue;

        Config config;
        config.configKey = configKey;
        config.configValue = intStream.str();
        config.Save();

        configCache[configKey] = config;

    }

    return defaultValue;
}

template <>
bool Config::GetConfigValue(const std::string& configKey, const bool& defaultValue) {

    try {
        std::string boolValue = GetConfigValue(configKey);
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

std::string Config::GetConfigValue(const std::string& configKey) {

    Config config = GetConfig(configKey);
    return config.configValue;

}

void Config::Insert() {

    soci::session session(Db::GetConnectionPool());

    session << "INSERT INTO config (config_key, config_value) "
            "VALUES (:config_key, :config_value)", soci::use(configKey), soci::use(configValue);

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
        soci::use(configValue), soci::use(id);
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

void Config::SetConfigKey(const std::string& configKey) {
    this->configKey = configKey;
}
const std::string& Config::GetConfigKey() const {
    return configKey;
}

void Config::SetConfigValue(const std::string& configValue) {
    this->configValue = configValue;
}
const std::string& Config::GetConfigValue() const {
    return configValue;
}

}
}

