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

/* Core */
/**
 * @brief Unload a MySQL database connector from memory that was previously loaded via DBConn::MySQL::New().
 * @retval void
 */
const void DBConn::MySQL::Delete()
{
    delete this;

    return;
}

/**
 * @brief Create a new MySQL connector.
 * @param[in] DBConn* A pointer to the parent DBConn object for callbacks.
 * @retval false Returned if there was an error creating the connector.
 * @retval true Returned if the connector was successfully created.
 */
const bool DBConn::MySQL::New( DBConn* dbconn )
{
    UFLAGS_DE( flags );
    pthread_attr_t res_attr;
    pthread_t res_thread;

    if ( dbconn == NULL )
    {
        LOGSTR( flags, "DBConn::MySQL::New()-> called with NULL dbconn" );
        return false;
    }

    m_dbconn = dbconn;

    if ( ::pthread_attr_init( &res_attr ) != 0 )
    {
        LOGERRNO( flags, "DBConn::MySQL::New()->pthread_attr_init()->" );
        return false;
    }

    if ( ::pthread_attr_setdetachstate( &res_attr, PTHREAD_CREATE_DETACHED ) != 0 )
    {
        LOGERRNO( flags, "DBConn::MySQL::New()->pthread_attr_setdetachstate()->" );
        return false;
    }

    if ( ::pthread_create( &res_thread, &res_attr, &DBConn::MySQL::Thread, this ) != 0 )
    {
        LOGERRNO( flags, "DBConn::MySQL::New()->pthread_create()->" );
        return false;
    }

    if ( ::pthread_attr_destroy( &res_attr ) != 0 )
    {
        LOGERRNO( flags, "DBConn::MySQL::New()->pthread_attr_destroy()->" );
        return false;
    }

    // Assume the thread will be successful; push the obj to list and validate it
    // during routine update loops to avoid blocking here
    dbconn_list.push_back( this->m_dbconn );

    return true;
}

/**
 * @brief Unload a database connector from memory that was previously loaded via DBConn::New().
 * @retval void
 */
const void DBConn::Delete()
{
    m_status = DBCONN_STATUS_CLOSE;

    if ( find( dbconn_list.begin(), dbconn_list.end(), this ) != dbconn_list.end() )
        g_global->m_next_dbconn = dbconn_list.erase( find( dbconn_list.begin(), dbconn_list.end(), this ) );

    if ( m_mysql != NULL )
        m_mysql->Delete();

    return;
}

/**
 * @brief Create a new database connection.
 * @param[in] type The database connector to use, from #DBCONN_TYPE.
 * @param[in] host The hostname or IP address of the server to connect to.
 * @param[in] socket The unix socket file or port number of the server to connect to.
 * @param[in] user Username to use when connecting to the database server.
 * @param[in] pass Password to use when connecting to the database server.
 * @param[in] database The name of the specific database to utilize.
 * @retval false Returned if there was an error connecting to the database.
 * @retval true Returned if a databaes connection was successfully established.
 */
const bool DBConn::New( const uint_t& type, const string& host, const string& socket, const string& user, const string& pass, const string& database )
{
    UFLAGS_DE( flags );
    bool valid = false;

    if ( host.empty() )
    {
        LOGSTR( flags, "DBConn::New()-> called with empty host" );
        return false;
    }
    else
        m_host = host;

    if ( socket.empty() )
    {
        LOGSTR( flags, "DBConn::New()-> called with empty socket" );
        return false;
    }
    else
        m_socket = socket;

    if ( user.empty() )
    {
        LOGSTR( flags, "DBConn::New()-> called with empty user" );
        return false;
    }
    else
        m_user = user;

    if ( pass.empty() )
    {
        LOGSTR( flags, "DBConn::New()-> called with empty pass" );
        return false;
    }
    else
        m_pass = pass;

    if ( database.empty() )
    {
        LOGSTR( flags, "DBConn::New()-> called with empty database" );
        return false;
    }
    else
        m_database = database;

    switch ( type )
    {
        case DBCONN_TYPE_MYSQL:
            m_mysql = new DBConn::MySQL();

            if ( !m_mysql->New( this ) )
                LOGSTR( flags, "DBConn::New()->DBConn::MySQL()->New() returned false" );
            else
                valid = true;
        break;

        default:
            LOGFMT( flags, "DBConn::New()-> called with invalid type: %lu", type );
        break;
    }

    if ( valid )
        m_type = type;

    return true;
}

/* Query */
/**
 * @brief Returns the current status of the database connector from #DBCONN_STATUS.
 * @retval uint_t A uint_t associated to #DBCONN_STATUS.
 */
const uint_t DBConn::gStatus()
{
    return m_status;
}

/* Manipulate */
/**
 * @brief Launches the MySQL connector into its own thread to avoid blocking.
 * @param[in] data A self-reference passed via this to use for callback.
 * @retval void
 */
void* DBConn::MySQL::Thread( void* data )
{
    UFLAGS_DE( flags );
    uint_t port = uintmin_t;
    bool valid = true;
    DBConn::MySQL* dbconn_mysql = reinterpret_cast<DBConn::MySQL*>( data );

    if ( mysql_init( &dbconn_mysql->m_sql ) == NULL )
    {
        dbconn_mysql->m_dbconn->m_status = DBCONN_STATUS_ERROR;
        LOGFMT( flags, "DBConn::MySQL::Thread()->mysql_init()-> %s", mysql_error( &dbconn_mysql->m_sql ) );
        mysql_thread_end();

        ::pthread_exit( reinterpret_cast<void*>( EXIT_FAILURE ) );
    }

    if ( mysql_options( &dbconn_mysql->m_sql, MYSQL_OPT_RECONNECT, &dbconn_mysql->m_reconnect ) != 0 )
    {
        dbconn_mysql->m_dbconn->m_status = DBCONN_STATUS_ERROR;
        LOGFMT( flags, "DBConn::MySQL::Thread()->mysql_options()-> %s", mysql_error( &dbconn_mysql->m_sql ) );
        mysql_thread_end();

        ::pthread_exit( reinterpret_cast<void*>( EXIT_FAILURE ) );
    }

    // Safer than ::stoi(), will output 0 for anything invalid
    stringstream( dbconn_mysql->m_dbconn->m_socket ) >> port;

    // If port is 0, connect via unix socket
    if ( port == 0 )
    {
        if ( mysql_real_connect( &dbconn_mysql->m_sql, CSTR( dbconn_mysql->m_dbconn->m_host ), CSTR( dbconn_mysql->m_dbconn->m_user ), CSTR( dbconn_mysql->m_dbconn->m_pass ), CSTR( dbconn_mysql->m_dbconn->m_database ), 0, CSTR( dbconn_mysql->m_dbconn->m_socket ), 0 ) == NULL )
            valid = false;
    }
    else
    {
        if ( mysql_real_connect( &dbconn_mysql->m_sql, CSTR( dbconn_mysql->m_dbconn->m_host ), CSTR( dbconn_mysql->m_dbconn->m_user ), CSTR( dbconn_mysql->m_dbconn->m_pass ), CSTR( dbconn_mysql->m_dbconn->m_database ), port, NULL, 0 ) == NULL )
            valid = false;
    }

    if ( !valid )
    {
        dbconn_mysql->m_dbconn->m_status = DBCONN_STATUS_ERROR;
        LOGFMT( flags, "DBConn::MySQL::Thread()->mysql_real_connect()-> %s", mysql_error( &dbconn_mysql->m_sql ));
        mysql_close( &dbconn_mysql->m_sql );
        mysql_thread_end();

        ::pthread_exit( reinterpret_cast<void*>( EXIT_FAILURE ) );
    }

    while ( dbconn_mysql->m_dbconn->m_status != DBCONN_STATUS_CLOSE )
    {
        ::usleep( CFG_THR_SLEEP );
    }

    mysql_close( &dbconn_mysql->m_sql );
    mysql_thread_end();

    ::pthread_exit( reinterpret_cast<void*>( EXIT_SUCCESS ) );
}

/* Internal */
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
    delete m_dbconn;

    return;
}

/**
 * @brief Constructor for the DBConn class.
 */
DBConn::DBConn()
{
    m_busy = false;
    m_database.clear();
    m_host.clear();
    m_mysql = NULL;
    m_pass.clear();
    m_socket.clear();
    m_status = uintmin_t;
    m_type = uintmin_t;
    m_user.clear();

    return;
}

/**
 * @brief Destructor for the DBConn class.
 */
DBConn::~DBConn()
{
    return;
}
