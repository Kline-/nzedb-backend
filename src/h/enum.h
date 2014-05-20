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
 * @file enum.h
 * @brief All numerical and boolean defines for every class and namespace.
 *
 * This file contains all numerical and boolean defines for each class and
 * namespace. All items of similar purpose are grouped together with a
 * comment to denote the class or namespace they are significant to.
 */
#ifndef DEC_ENUM_H
#define DEC_ENUM_H

/** @name DBConn */ /**@{*/
/**
 * @enum DBCONN_STATUS
 */
enum DBCONN_STATUS
{
    DBCONN_STATUS_NONE  = 0, /**< A newly initialized connector. */
    DBCONN_STATUS_ERROR = 1, /**< Connector failed to initialize or connect to the database. */
    DBCONN_STATUS_READY = 2, /**< Connector successfully connected to the database and is available for work. */
    DBCONN_STATUS_CLOSE = 3, /**< Connector is shutting down. */
    DBCONN_STATUS_BUSY  = 4, /**< Connector is busy processing a task. */
    MAX_DBCONN_STATUS   = 5  /**< Safety limit for looping. */
};

/**
 * @enum DBCONN_TYPE
 */
enum DBCONN_TYPE
{
    DBCONN_TYPE_MYSQL = 0, /**< Use the MySQL connector. */
    MAX_DBCONN_TYPE   = 1  /**< Safety limit for looping. */
};
/**@}*/

/** @name Utils */ /**@{*/
/**
 * @enum UTILS_OPTS
 */
enum UTILS_OPTS
{
    UTILS_DEBUG       = 0, /**< Enables debugging output in Utils::_Logger(). This typically includes the calling function's file and line number. */
    UTILS_RAW         = 1, /**< Raw data output. Skips prepending / appending anything on Utils::_Logger writes (no timestamp, etc). */
    UTILS_TYPE_ERROR  = 2, /**< Indicates an error and prepends #CFG_STR_UTILS_ERROR to Utils::_Logger() output. */
    UTILS_TYPE_INFO   = 3, /**< Indicates an info message and prepends #CFG_STR_UTILS_INFO to Utils::_Logger() output. */
    UTILS_TYPE_SOCKET = 4, /**< Indicates a socket related message and prepends #CFG_STR_UTILS_SOCKET to Utils::_Logger() output. */
    UTILS_RET_ERROR   = 5, /**< Returned to indicate an error that the calling function should handle. */
    UTILS_RET_FALSE   = 6, /**< Returned to indicate a false value that the calling function should handle. */
    UTILS_RET_TRUE    = 7, /**< Returned to indicate a true value that the calling function should handle. */
    MAX_UTILS         = 8  /**< Safety limit for looping. */
};

/**
 * @def UTILS_IS_DIRECTORY
 */
#define UTILS_IS_DIRECTORY true

/**
 * @def UTILS_IS_FILE
 */
#define UTILS_IS_FILE      false
/**@}*/

#endif
