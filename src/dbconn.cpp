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

/* Core */
/**
 * @brief Unload a MySQL database connector from memory that was previously loaded via DBConn::MySQL::New().
 * @retval void
 */
const void DBConn::MySQL::Delete()
{
    return;
}

/**
 * @brief Create a new MySQL connector.
 * @param[in] host The hostname or IP address of the server to connect to.
 * @param[in] socket The unix socket file or port number of the server to connect to.
 * @param[in] user Username to use when connecting to the database server.
 * @param[in] pass Password to use when connecting to the database server.
 * @param[in] database The name of the specific database to utilize.
 * @retval false Returned if there was an error creating the connector.
 * @retval true Returned if the connector was successfully created.
 */
const bool DBConn::MySQL::New( const string& host, const string& socket, const string& user, const string& pass, const string& database )
{
    UFLAGS_DE( flags );

    if ( host.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty host" );
    if ( socket.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty socket" );
    if ( user.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty user" );
    if ( pass.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty pass" );
    if ( database.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty database" );

    return true;
}

/**
 * @brief Unload a database connector from memory that was previously loaded via DBConn::New().
 * @retval void
 */
const void DBConn::Delete()
{
    if ( m_mysql != NULL )
        delete m_mysql;

    delete this;

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

    switch ( type )
    {
        case DBCONN_TYPE_MYSQL:
            m_mysql = new DBConn::MySQL();

            if ( !m_mysql->New( host, socket, user, pass, database ) )
                LOGSTR( flags, "DBConn::New()->DBConn::MySQL()->New() returned false" );
            else
                valid = true;
        break;

        default:
            LOGFMT( flags, "DBConn::New()-> called with invalid type: %lu", type );
        break;
    }

    if ( host.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty host" );
    if ( socket.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty socket" );
    if ( user.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty user" );
    if ( pass.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty pass" );
    if ( database.empty() )
        LOGSTR( flags, "DBConn::New()-> called with empty database" );

    if ( valid )
        m_type = type;

    return valid;
}

/* Query */

/* Manipulate */

/* Internal */
/**
 * @brief Constructor for the DBConn::MySQL clasas.
 */
DBConn::MySQL::MySQL()
{
    m_reconnect = true;

    return;
}

/**
 * @brief Destructor for the DBConn::MySQL class.
 */
DBConn::MySQL::~MySQL()
{
    return;
}

/**
 * @brief Constructor for the DBConn class.
 */
DBConn::DBConn()
{
    m_busy = false;
    m_mysql = NULL;
    m_type = uintmin_t;

    return;
}

/**
 * @brief Destructor for the DBConn class.
 */
DBConn::~DBConn()
{
    return;
}
