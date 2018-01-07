/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#include <sqlite3/soci-sqlite3.h>

#include "Db.h"
#include "db/SchemaUpdater.h"

namespace greenthumb {

soci::connection_pool& Db::GetConnectionPool() {
    static soci::connection_pool pool(poolSize);
    return pool;
}

void Db::InitialisePool() {
    soci::connection_pool& pool = GetConnectionPool();

    wxStandardPaths standardPaths = wxStandardPaths::Get();
    wxString dbFilename = standardPaths.GetUserDataDir();

    if (!wxDirExists(dbFilename)) {
        wxMkdir(dbFilename);
    }

    dbFilename += wxFileName::GetPathSeparator() + _("db.sqlite3");

    for (unsigned int i = 0; i < poolSize; ++i) {
        soci::session& sql = pool.at(i);
        sql.open(soci::sqlite3, "dbname=" + dbFilename.ToStdString() + " timeout=30");
    }

    db::SchemaUpdater su;
    su.Update();
}


}
