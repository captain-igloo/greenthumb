#ifndef DB_H
#define DB_H

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <iostream>
#include <soci.h>
#include <sqlite3/soci-sqlite3.h>

namespace greenthumb {

class Db {

    public:

        static soci::connection_pool& GetConnectionPool();
        static void InitialisePool();

    private:

        static const unsigned poolSize = 10;

        Db() {}
        Db(Db const&);
        void operator=(Db const&);

        static void CreateTables();
        static bool TableExists(const std::string& tableName);
};


}

#endif // DB_H
