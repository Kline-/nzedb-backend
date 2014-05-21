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
 * @file dbconn_mysql.cpp
 * @brief All non-template member functions of the DBConnMySQL class.
 *
 * The DBConnMySQL class provides an interface to a MySQL backend.
 */
#include "h/includes.h"
#include "h/dbconn_mysql.h"

#include "h/list.h"

/**
 * @brief Connect to a MySQL database host.
 * @param[in] DBConnMySQL* The DBConnMySQL object to connect the database to.
 * @retval void
 */
const void DBConnMySQL::Connect( DBConnMySQL* mysql )
{
    UFLAGS_DE( flags );
    uint_t port = uintmin_t;
    bool valid = true;

    if ( mysql == NULL )
    {
        LOGSTR( flags, "DBConnMySQL::Connect()-> called with NULL mysql" );
        return;
    }

    if ( mysql->gHost().empty() )
    {
        LOGSTR( flags, "DBConnMySQL::Connect()-> called with empty host" );
        return;
    }

    if ( mysql->gSocket().empty() )
    {
        LOGSTR( flags, "DBConnMySQL::Connect()-> called with empty socket" );
        return;
    }

    if ( mysql->gUser().empty() )
    {
        LOGSTR( flags, "DBConnMySQL::Connect()-> called with empty user" );
        return;
    }

    if ( mysql->gPass().empty() )
    {
        LOGSTR( flags, "DBConnMySQL::Connect()-> called with empty pass" );
        return;
    }

    if ( mysql->gDatabase().empty() )
    {
        LOGSTR( flags, "DBConnMySQL::Connect()-> called with empty database" );
        return;
    }

    if ( mysql_init( &mysql->m_sql ) == NULL )
    {
        mysql->sStatus( DBCONN_STATUS_ERROR );
        LOGFMT( flags, "DBConnMySQL::Connect()->mysql_init()-> %s", mysql_error( &mysql->m_sql ) );

        return;
    }

    if ( mysql_options( &mysql->m_sql, MYSQL_OPT_RECONNECT, &mysql->m_reconnect ) != 0 )
    {
        mysql->sStatus( DBCONN_STATUS_ERROR );
        LOGFMT( flags, "DBConnMySQL::Connect()->mysql_options()-> %s", mysql_error( &mysql->m_sql ) );

        return;
    }

    // Safer than ::stoi(), will output 0 for anything invalid
    stringstream( mysql->gSocket() ) >> port;

    // If port is 0, connect via unix socket
    if ( port == 0 )
    {
        if ( mysql_real_connect( &mysql->m_sql, CSTR( mysql->gHost() ), CSTR( mysql->gUser() ), CSTR( mysql->gPass() ), CSTR( mysql->gDatabase() ), 0, CSTR( mysql->gSocket() ), 0 ) == NULL )
            valid = false;
    }
    else
    {
        if ( mysql_real_connect( &mysql->m_sql, CSTR( mysql->gHost() ), CSTR( mysql->gUser() ), CSTR( mysql->gPass() ), CSTR( mysql->gDatabase() ), port, NULL, 0 ) == NULL )
            valid = false;
    }

    if ( !valid )
    {
        mysql->sStatus( DBCONN_STATUS_ERROR );
        LOGFMT( flags, "DBConnMySQL::Connect()->mysql_real_connect()-> %s", mysql_error( &mysql->m_sql ) );

        return;
    }

    // Now wait for events
    mysql->Run();

    return;
}

/**
 * @brief Run a query against the database and return a result set in a neutral format.
 * @param[in] query The query to execute against the database.
 * @retval vector<vector<string>> The result set of the query.
 */
const vector<vector<string>> DBConnMySQL::Query( const string& query )
{
    UFLAGS_DE( flags );
    MYSQL_RES* res;
    MYSQL_ROW row;
    uint_t rows = 0, length = 0;
    vector<vector<string>> result;

    // Busy out to ensure work goes to other threads
    sStatus( DBCONN_STATUS_BUSY );

    if ( query.empty() )
    {
        sStatus( DBCONN_STATUS_READY );
        LOGSTR( flags, "DBConnMySQL::Query-> called with empty query" );

        return result;
    }

    if ( mysql_query( &m_sql, CSTR( query ) ) )
    {
        sStatus( DBCONN_STATUS_READY );
        LOGFMT( flags, "DBConnMySQL::Query()->mysql_query()-> %s", mysql_error( &m_sql ) );

        return result;
    }
    else if ( ( res = mysql_store_result( &m_sql ) ) == NULL )
    {
        sStatus( DBCONN_STATUS_READY );
        LOGFMT( flags, "DBConnMySQL::Query()->mysql_store_result()-> %s", mysql_error( &m_sql ) );

        return result;
    }
    else if ( ( length = mysql_field_count( &m_sql ) ) == 0 )
    {
        sStatus( DBCONN_STATUS_READY );
        LOGFMT( flags, "DBConnMySQL::Query()->mysql_field_count()-> %s", mysql_error( &m_sql ) );

        return result;
    }
    else if ( ( rows = mysql_num_rows( res ) ) == 0 )
    {
        sStatus( DBCONN_STATUS_READY );
        LOGFMT( flags, "DBConnMySQL::Query()->mysql_num_rows()-> %s", mysql_error( &m_sql ) );

        return result;
    }

    // Since the first row is used to pass metadata, increment the count
    rows++;

    // Resize the result to permit access via operator[]
    result.resize( rows );

    // Need a minimum length of 2 to pass metadata, but don't modify length
    // itself to ensure proper column count in the result set
    if ( length < 2 )
    {
        for ( uint_t i = 0; i < rows; i++ )
            result[i].resize( 2 );
    }
    else
    {
        for ( uint_t i = 0; i < rows; i++ )
            result[i].resize( length );
    }

    // Store the number of columns in result[0][0] and number of rows in reslt[0][1]
    result[0][0] = length;
    result[0][1] = rows;

    // At this point there is a valid result set and each row
    // needs to be iterated through and moved to the vector
    for ( uint_t x = 1; x < rows; x++ )
    {
        if ( ( row = mysql_fetch_row( res ) ) == NULL )
        {
            sStatus( DBCONN_STATUS_READY );
            LOGFMT( flags, "DBConnMySQL::Query()->mysql_fetch_row()-> %s", mysql_error( &m_sql ) );

            return result;
        }

        for ( uint_t y = 0; y < length; y++ )
            result[x][y] = row[y];
    }

    return result;
}

/**
 * @brief Process events in the thread.
 * @retval void
 */
const void DBConnMySQL::Run()
{
    LOGFMT( 0, "MySQL server connected: %s", CSTR( gHost() ) );

    // Assume the thread will be successful; push the obj to list and validate it
    // during routine update loops to avoid blocking here
    sStatus( DBCONN_STATUS_READY );
    dbconn_list.push_back( this );

    while ( !g_global->m_shutdown )
    {
        this_thread::yield();
        ::usleep( CFG_THR_SLEEP );
    }

    return;
}

/**
 * @brief Constructor for the DBConnMySQL clasas.
 */
DBConnMySQL::DBConnMySQL( const uint_t& type, const string& host, const string& socket, const string& user, const string& pass, const string& database ) :
    DBConn::DBConn( type, host, socket, user, pass, database )
{
    m_reconnect = true;

    thread( thread( Connect, this ) ).detach();

    return;
}

/**
 * @brief Destructor for the DBConnMySQL class.
 */
DBConnMySQL::~DBConnMySQL()
{
    mysql_close( &m_sql );
    mysql_thread_end();

    return;
}
