/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef SCHEMAUPDATER_H
#define SCHEMAUPDATER_H

#include <cstdint>
#include <string>

namespace greenthumb {
namespace db {

/**
 * Updates the database schema as necessary.
 */
class SchemaUpdater {
    public:
        /**
         * Applies any necessary updates.
         */
        void Update() const;
    private:
        /**
         * Gets the current database version (i.e PRAGMA user_version).
         *
         * @return The current database version.
         */
        const int32_t GetDatabaseVersion() const;

        /**
         * Sets the database version.
         *
         * @param databaseVersion The database version.
         */
        const void SetDatabaseVersion(const int32_t databaseVersion) const;

        /**
         * Whether or not the specified table exists in the database.
         *
         * @param tableName The table name.
         * @return True if the table exists else false.
         */
        const bool TableExists(const std::string& tableName) const;

        /**
         * Initialise the database.
         */
        const void UpdateToVersion1() const;
};

}
}

#endif // SCHEMAUPDATER_H
