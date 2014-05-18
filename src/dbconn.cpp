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
 * @brief Create a new database connection.
 * @param[in] host The hostname or IP address of the server to connect to.
 * @param[in] socket The unix socket file or port number of the server to connect to.
 * @param[in] user Username to use when connecting to the database server.
 * @param[in] pass Password to use when connecting to the database server.
 * @param[in] database The name of the specific database to utilize.
 * @retval false Returned if there was an error connecting to the database.
 * @retval true Returned if a databaes connection was successfully established.
 */
const bool DBConn::New( const string& host, const string& socket, const string& user, const string& pass, const string& database )
{
    return true;
}

/* Query */

/* Manipulate */

/* Internal */
