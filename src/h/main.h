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
 * @file main.h
 * @brief The Main namespace.
 *
 * This file contains the Main namespace and template functions.
 */
#ifndef DEC_MAIN_H
#define DEC_MAIN_H

using namespace std;

/**
 * @brief The Main namespace contains all central dispatch functions and templates.
 */
namespace Main
{
    /**
     * @brief Container for global variables.
     */
    class Global
    {
        public:
            const void Delete();

            Global();
            ~Global();

            vector<DBConn*>::iterator m_next_dbconn; /**< Used as the next iterator in all loops dealing with DBConn objects to prevent nested processing loop problems. */
            bool m_shutdown; /**< Control server shutdown. */
            vector<thread> m_threads; /**< All active threads. */
            chrono::high_resolution_clock::time_point m_time_current; /**< Current time from the host OS. */
    };

    const void Startup( const string& config = "" );
    const void Update();
    const void PollDBConn();
};

#endif
