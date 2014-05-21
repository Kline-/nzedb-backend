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
 * @file utils.h
 * @brief The Utils namespace.
 *
 * This file contains the Utils namespace and template functions.
 */
#ifndef DEC_UTILS_H
#define DEC_UTILS_H

using namespace std;

/**
 * @brief The Utils namespace contains all general purpose, multi-use, and non-class functions.
 */
namespace Utils
{
    #define FormatString( flags, fmt, ... ) _FormatString( PP_NARG( __VA_ARGS__ ), flags, _caller_, fmt, ##__VA_ARGS__ )
    #define Logger( flags, fmt, ... ) _Logger( PP_NARG( __VA_ARGS__ ), flags, _caller_, fmt, ##__VA_ARGS__ )
    const uint_t NumChar( const string& input, const string& item );
    const string StrTime( const time_t& now = chrono::high_resolution_clock::to_time_t( chrono::high_resolution_clock::now() ) );
    const vector<string> StrTokens( const string& input, const bool& quiet = false );
    const string _FormatString( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, ... );
    const string __FormatString( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, va_list& val );
    const void _Logger( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, ... );
};

#endif
