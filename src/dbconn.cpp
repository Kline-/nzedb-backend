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
 * @brief Returns the current database of the database connector.
 * @retval string The current database of the database connector.
 */
const string DBConn::gDatabase()
{
    return m_database;
}

/**
 * @brief Returns the current host of the database connector.
 * @retval string The current host of the database connector.
 */
const string DBConn::gHost()
{
    return m_host;
}

/**
 * @brief Returns the current pass of the database connector.
 * @retval string The current pass of the database connector.
 */
const string DBConn::gPass()
{
    return m_pass;
}

/**
 * @brief Returns the current type of the database connector from #DBCONN_TYPE.
 * @retval uint_t A uint_t associated to #DBCONN_TYPE.
 */
const uint_t DBConn::gType()
{
    return m_type;
}

/**
 * @brief Returns the current socket of the database connector.
 * @retval string The current socket of the database connector.
 */
const string DBConn::gSocket()
{
    return m_socket;
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
 * @brief Returns the current user of the database connector.
 * @retval string The current user of the database connector.
 */
const string DBConn::gUser()
{
    return m_user;
}

/**
 * @brief Sets the current status of the database connector from #DBCONN_STATUS.
 * @param[in] status The current status of the database connector from #DBCONN_STATUS.
 * @retval void
 */
const void DBConn::sStatus( const uint_t& status )
{
    UFLAGS_DE( flags );

    if ( status < uintmin_t || status >= MAX_DBCONN_STATUS )
    {
        LOGFMT( flags, "DBConn::sStatus()-> called with invalid status: %lu", status );
        return;
    }

    m_status = status;

    return;
}

/**
 * @brief Constructor for the DBConn class.
 */
DBConn::DBConn( const uint_t& type, const string& host, const string& socket, const string& user, const string& pass, const string& database ) :
    m_type( type ), m_host( host ), m_socket( socket ), m_user( user ), m_pass( pass ), m_database( database )
{
    m_status = uintmin_t;

    return;
}

/**
 * @brief Destructor for the DBConn class.
 */
DBConn::~DBConn()
{
    return;
}
