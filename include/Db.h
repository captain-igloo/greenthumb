/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DB_H
#define DB_H

#include <wx/filename.h>
#include <wx/stdpaths.h>

#include <soci.h>

namespace greenthumb {

/**
 * Database connection pool.
 */
class Db {

    public:
        /**
         * Gets the connection pool.
         *
         * @return The connection pool.
         */
        static soci::connection_pool& GetConnectionPool();

        /**
         * Initialises the connection pool.
         */
        static void InitialisePool();

    private:
        /** The number of connections in the pool. */
        static const unsigned poolSize = 10;

        /** Prevent instantiation. */
        Db() {}
        Db(Db const&);
        void operator=(Db const&);
};

}

#endif // DB_H
