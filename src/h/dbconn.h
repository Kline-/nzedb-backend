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
 * @brief Functions and common interface to all database connectors.
 */
class DBConn
{
    public:
        virtual const vector<vector<string>> Query( const string& query ) = 0;
        const string gDatabase();
        const string gHost();
        const string gPass();
        const string gSocket();
        const uint_t gStatus();
        const uint_t gType();
        const string gUser();

        DBConn( const uint_t& type, const string& host, const string& socket, const string& user, const string& pass, const string& database );
        virtual ~DBConn();

    private:
        virtual const void Run() = 0;

        uint_t m_type; /**< The type of connector to utilize from #DBCONN_TYPE. */
        string m_host; /**< Hostname of the database server. */
        string m_socket; /**< Unix socket or port number of the database server. */
        string m_user; /**< Username to login to the database server with. */
        string m_pass; /**< Password to login to the database server with. */
        string m_database; /**< Database to access on the database server. */
        uint_t m_status; /**< Callback to check if the thread made a successful connection. */
};

#endif
