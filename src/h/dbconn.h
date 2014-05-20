/**
 * nzedb-backend
 * Copyright (c) 2012-2014 Matthew Goff <matt@goff.cc>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 **/
/**
 * @file dbconn.h
 * @brief The DBConn class.
 *
 * This file contains the DBConn class and template functions.
 */
#ifndef DEC_DBCONN_H
#define DEC_DBCONN_H

using namespace std;

/**
 * @brief An instance of a database connector.
 */
class DBConn
{
    /**
     * @brief MySQL backend.
     */
    class MySQL
    {
        public:
            /** @name Core */ /**@{*/
            const void Delete();
            const bool New( DBConn* dbconn );
            /**@}*/

            /** @name Query */ /**@{*/
            /**@}*/

            /** @name Manipulate */ /**@{*/
            static void* Thread( void* data );
            /**@}*/

            /** @name Internal */ /**@{*/
            MySQL();
            ~MySQL();
            /**@}*/

        private:
            DBConn* m_dbconn; /**< Parent DBConn for callback. */
            MYSQL m_sql; /**< Connection to the MySQL database. */
            MYSQL_RES* m_res; /**< The result set from a query. */
            MYSQL_ROW m_row; /**< One row of data from a query. */
            my_bool m_reconnect; /**< Determine if the handler will attempt to reconnect when disconnected. */
    };

    public:
        /** @name Core */ /**@{*/
        const void Delete();
        const bool New( const uint_t& type, const string& host, const string& socket, const string& user, const string& pass, const string& database );
        /**@}*/

        /** @name Query */ /**@{*/
        const uint_t gStatus();
        /**@}*/

        /** @name Manipulate */ /**@{*/
        /**@}*/

        /** @name Internal */ /**@{*/
        DBConn();
        ~DBConn();
        /**@}*/

    private:
        bool m_busy; /**< Indicate if the connector is pending a query response. */
        string m_database; /**< Database to access on the database server. */
        string m_host; /**< Hostname of the database server. */
        DBConn::MySQL* m_mysql; /**< MySQL connector. */
        string m_pass; /**< Password to login to the database server with. */
        string m_socket; /**< Unix socket or port number of the database server. */
        uint_t m_status; /**< Callback to check if the thread made a successful connection. */
        uint_t m_type; /**< The type of connector to utilize from #DBCONN_TYPE. */
        string m_user; /**< Username to login to the database server with. */
};

#endif
