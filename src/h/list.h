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
 * @file list.h
 * @brief All globally referenced list / map / vector types.
 *
 * All list-type objects that are global in scope are referenced via this
 * file. These are usually, but not always, containers that reference pointer
 * objects to ensure proper tracking and memory cleanup later.
 */
#ifndef DEC_LIST_H
#define DEC_LIST_H

using namespace std;

/**
 * @var dbconn_list
 * @brief All database connectors that exist within the server.
 * @param DBConn* A pointer to a DBConn object in memory.
 */
extern vector<DBConn*> dbconn_list;

#endif
