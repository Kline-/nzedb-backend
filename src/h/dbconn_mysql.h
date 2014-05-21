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
 * @file dbconn_mysql.h
 * @brief The DBConnMySQL class.
 *
 * This file contains the DBConnMySQL class and template functions.
 */
#ifndef DEC_DBCONNMYSQL_H
#define DEC_DBCONNMYSQL_H

#include "dbconn.h"

using namespace std;

/**
 * @brief DBConnMySQL extends the DBConn class to implement an instance of a MySQL database connector.
 */
class DBConnMySQL : public DBConn
{
    public:
        const vector< vector<string> > Query( const string& query );

        DBConnMySQL( const uint_t& type, const string& host, const string& socket, const string& user, const string& pass, const string& database );
        ~DBConnMySQL();

    private:
        const void Connect();
        static const void tConnect();
        const void Run();

        MYSQL m_sql; /**< Connection to the MySQL database. */
        MYSQL_RES* m_res; /**< The result set from a query. */
        MYSQL_ROW m_row; /**< One row of data from a query. */
        my_bool m_reconnect; /**< Determine if the handler will attempt to reconnect when disconnected. */
};

#endif
