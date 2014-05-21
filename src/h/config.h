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
 * @file config.h
 * @brief nzedb-backend configuration options.
 *
 * All nzedb-backend configuration options are kept within this file. This
 * includes defines for specific folders, memory tuning, frequently used
 * string messages, etc.
 */
#ifndef DEC_CONFIG_H
#define DEC_CONFIG_H

/***************************************************************************
 *                              MEMORY OPTIONS                             *
 ***************************************************************************/
/** @name Memory Options */ /**@{*/
/**
 * @def CFG_MEM_MAX_BITSET
 * @brief Maximum size of all bitset elements.
 * @par Default: 128
 */
#define CFG_MEM_MAX_BITSET 128

/**
 * @def CFG_MEM_MAX_DBCONN
 * @brief Maximum number of database connections to open at once.
 * @par Default: 4
 */
#define CFG_MEM_MAX_DBCONN 4
/**@}*/

/***************************************************************************
 *                              STRING OPTIONS                             *
 ***************************************************************************/
/** @name String Options */ /**@{*/
/**
 * @def CFG_STR_UTILS_ERROR
 * @brief String to prepend to logs flagged UTILS_TYPE_ERROR.
 * @par Default: "[ERROR ] "
 */
#define CFG_STR_UTILS_ERROR "[ERROR] "

/**
 * @def CFG_STR_UTILS_INFO
 * @brief String to prepend to logs flagged UTILS_TYPE_INFO.
 * @par Default: "[INFO  ] "
 */
#define CFG_STR_UTILS_INFO "[INFO  ] "

/**
 * @def CFG_STR_UTILS_SOCKET
 * @brief String to prepend to logs flagged UTILS_TYPE_SOCKET.
 * @par Default: "[SOCKET] "
 */
#define CFG_STR_UTILS_SOCKET "[SOCKET] "

/**
 * @def CFG_STR_VERSION
 * @brief The current nzedb-backend version.
 * @par Default: "nzedb-backend 0.0.0"
 */
#define CFG_STR_VERSION "nzedb-backend 0.0.0"
/**@}*/

/***************************************************************************
 *                              THREAD OPTIONS                             *
 ***************************************************************************/
/** @name Thread Options */ /**@{*/
/**
 * @def CFG_THR_SLEEP
 * @brief The amount of time (in microseconds) to sleep a thread while waiting for work.
 * @par Default: 5
 */
#define CFG_THR_SLEEP 5
/**@}*/

#endif
