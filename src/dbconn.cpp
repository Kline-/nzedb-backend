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
 * @file dbconn.cpp
 * @brief All non-template member functions of the DBConn class.
 *
 * The DBConn class provides an abstraction interface for the backend
 * database such as MariaDB, MySQL, etc.
 */
#include "h/includes.h"
#include "h/dbconn.h"

#include "h/list.h"

/**
 * @brief Connect to a MySQL database host.
 * @param[in] DBConn* A pointer to the parent DBConn object for callbacks.
 * @retval false Returned if there was an error creating the connector.
 * @retval true Returned if the connector was successfully created.
 */
const bool DBConn::MySQL::Connect( DBConn* dbconn )
{
    UFLAGS_DE( flags );
    uint_t port = uintmin_t;
    bool valid = true;

    if ( dbconn == NULL )
    {
        LOGSTR( flags, "DBConn::MySQL::Connect()-> called with NULL dbconn" );
        return false;
    }

    m_dbconn = dbconn;

    if ( mysql_init( &m_sql ) == NULL )
    {
        m_dbconn->m_status = DBCONN_STATUS_ERROR;
        LOGFMT( flags, "DBConn::MySQL::Thread()->mysql_init()-> %s", mysql_error( &m_sql ) );

        return false;
    }

    if ( mysql_options( &m_sql, MYSQL_OPT_RECONNECT, &m_reconnect ) != 0 )
    {
        m_dbconn->m_status = DBCONN_STATUS_ERROR;
        LOGFMT( flags, "DBConn::MySQL::Thread()->mysql_options()-> %s", mysql_error( &m_sql ) );

        return false;
    }

    // Safer than ::stoi(), will output 0 for anything invalid
    stringstream( m_dbconn->m_socket ) >> port;

    // If port is 0, connect via unix socket
    if ( port == 0 )
    {
        if ( mysql_real_connect( &m_sql, CSTR( m_dbconn->m_host ), CSTR( m_dbconn->m_user ), CSTR( m_dbconn->m_pass ), CSTR( m_dbconn->m_database ), 0, CSTR( m_dbconn->m_socket ), 0 ) == NULL )
            valid = false;
    }
    else
    {
        if ( mysql_real_connect( &m_sql, CSTR( m_dbconn->m_host ), CSTR( m_dbconn->m_user ), CSTR( m_dbconn->m_pass ), CSTR( m_dbconn->m_database ), port, NULL, 0 ) == NULL )
            valid = false;
    }

    if ( !valid )
    {
        m_dbconn->m_status = DBCONN_STATUS_ERROR;
        LOGFMT( flags, "DBConn::MySQL::Connect()->mysql_real_connect()-> %s", mysql_error( &m_sql ) );

        return false;
    }

    // Assume the thread will be successful; push the obj to list and validate it
    // during routine update loops to avoid blocking here
    m_dbconn->m_status = DBCONN_STATUS_READY;
    dbconn_list.push_back( m_dbconn );

    return true;
}

/**
 * @brief Returns the current status of the database connector from #DBCONN_STATUS.
 * @retval uint_t A uint_t associated to #DBCONN_STATUS.
 */
const uint_t DBConn::gStatus()
{
    return m_status;
}

/**
 * @brief Sanity checks the parameters then threads off the actual connection.
 * @retval void
 */
const void DBConn::Connect()
{
    UFLAGS_DE( flags );

    if ( m_host.empty() )
    {
        LOGSTR( flags, "DBConn::Connect()-> called with empty host" );
        return;
    }

    if ( m_socket.empty() )
    {
        LOGSTR( flags, "DBConn::Connect()-> called with empty socket" );
        return;
    }

    if ( m_user.empty() )
    {
        LOGSTR( flags, "DBConn::Connect()-> called with empty user" );
        return;
    }

    if ( m_pass.empty() )
    {
        LOGSTR( flags, "DBConn::Connect()-> called with empty pass" );
        return;
    }

    if ( m_database.empty() )
    {
        LOGSTR( flags, "DBConn::Connect()-> called with empty database" );
        return;
    }

    switch ( m_type )
    {
        case DBCONN_TYPE_MYSQL:
            m_mysql = new DBConn::MySQL();
        break;

        default:
            LOGFMT( flags, "DBConn::Connect()-> called with invalid type: %lu", m_type );
        break;
    }

    g_global->m_threads.push_back( thread( tConnect, this ) );

    return;
}

/**
 * @brief Initializes the database connector via the appropriate handler.
 * @retval void
 */
const void DBConn::tConnect( DBConn* dbconn )
{
    UFLAGS_DE( flags );

    switch ( dbconn->m_type )
    {
        case DBCONN_TYPE_MYSQL:
            if ( !dbconn->m_mysql->Connect( dbconn ) )
            {
                LOGSTR( flags, "DBConn::tConnect()->DBConn::MySQL::Connect()-> returned false" );
                delete dbconn;
                return;
            }
        break;

        default:
            LOGFMT( flags, "DBConn::tConnect()-> called with invalid type: %lu", dbconn->m_type );
        break;
    }

    while ( dbconn->m_status != DBCONN_STATUS_BUSY  )
    {
        this_thread::yield();
        sleep( 1 );
    }

    return;
}

/**
 * @brief Constructor for the DBConn::MySQL clasas.
 */
DBConn::MySQL::MySQL()
{
    m_dbconn = NULL;
    m_reconnect = true;

    return;
}

/**
 * @brief Destructor for the DBConn::MySQL class.
 */
DBConn::MySQL::~MySQL()
{
    mysql_close( &m_sql );
    mysql_thread_end();

    return;
}

/**
 * @brief Constructor for the DBConn class.
 */
DBConn::DBConn( const uint_t& type, const string& host, const string& socket, const string& user, const string& pass, const string& database ) :
    m_type( type ), m_host( host ), m_socket( socket ), m_user( user ), m_pass( pass ), m_database( database )
{
    m_mysql = NULL;
    m_status = uintmin_t;

    Connect();

    return;
}

/**
 * @brief Destructor for the DBConn class.
 */
DBConn::~DBConn()
{
    m_status = DBCONN_STATUS_CLOSE;

    if ( find( dbconn_list.begin(), dbconn_list.end(), this ) != dbconn_list.end() )
        g_global->m_next_dbconn = dbconn_list.erase( find( dbconn_list.begin(), dbconn_list.end(), this ) );

    if ( m_mysql != NULL )
        delete m_mysql;

    return;
}
