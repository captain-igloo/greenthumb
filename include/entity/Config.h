/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef ENTITY_CONFIG_H
#define ENTITY_CONFIG_H

#include <map>
#include <soci.h>

namespace greenthumb {
namespace entity {

/**
 * Represents a row in the "config" table.
 */
class Config {

    public:
        /**
         * Constructor.
         */
        Config();

        /**
         * Gets the specified config from the database or cache.
         *
         * @param configKey The config key.
         * @throw std::out_of_range If the config is not found.
         */
        static Config GetConfig(const std::string& configKey);

        /**
         * Gets the specified config value
         *
         * @param configKey The config key.
         * @throw std::out_of_range If the config is not found.
         */
        static std::string GetConfigValue(const std::string& configKey);

        /**
         * Gets the specified config value.  If it isn't found in the existing config, set to the
         * specified default and return that.
         *
         * @param configKey The config key.
         * @param defaultValue The default value (std::string, int, bool).
         */
        template<typename T>
        static T GetConfigValue(const std::string& configKey, const T& defaultValue);

        /**
         * Sets the specified config key / value in the database and cache.  If it already exists
         * it is updated else it is inserted.
         *
         * @param configKey The config key.
         * @param configValue The config value (std::string, int, bool).
         */
        template<typename T>
        static void SetConfigValue(const std::string& configKey, const T& configValue);

        /**
         * Inserts a row in the "config" table.
         */
        void Insert();

        /**
         * Updates the row in the "config" table.
         */
        void Update();

        /**
         * If the row hasn't yet been inserted, insert; else update.
         */
        void Save();

        /**
         * Sets the id
         *
         * @param id The id.
         */
        void SetId(const unsigned id);

        /**
         * Gets the id.
         */
        const int GetId() const;

        /**
         * Sets the config key.
         *
         * @param configKey The config key.
         */
        void SetConfigKey(const std::string& configKey);

        /**
         * Gets the config key.
         */
        const std::string& GetConfigKey() const;

        /**
         * Sets the config value.
         *
         * @param configValue The config value.
         */
        void SetConfigValue(const std::string& configValue);

        /**
         * Gets the config value.
         */
        const std::string& GetConfigValue() const;

    protected:
    private:
        unsigned id;
        std::string configKey;
        std::string configValue;

        static std::map<std::string, Config> configCache;
};

}
}

namespace soci
{
    template<>
    struct type_conversion<greenthumb::entity::Config>
    {

        typedef values base_type;

        static void from_base(values const & v, indicator ind, greenthumb::entity::Config & c)
        {
            c.SetId(v.get<int>("id"));
            c.SetConfigKey(v.get<std::string>("config_key"));
            c.SetConfigValue(v.get<std::string>("config_value"));
        }

        static void to_base(const greenthumb::entity::Config & c, values & v, indicator & ind)
        {
            v.set("id", c.GetId());
            v.set("config_key", c.GetConfigKey());
            v.set("config_value", c.GetConfigValue());

            ind = i_ok;
        }
    };
}

#endif // ENTITY_CONFIG_H
