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
 * @file utils.cpp
 * @brief All non-template member functions of the Utils namespace.
 *
 * The Utils namespace contains all general purpose, multi-use, and non-class
 * functions. Classes that are implemented to extend default operators or
 * designed to be used in a utility sense, rather than actual objects, are also
 * contained within this namespace.
 */
#include "h/includes.h"
#include "h/utils.h"

/**
 * @brief Returns the number of a specific character in a given string.
 * @param[in] input A string value to search.
 * @param[in] item The character to search for within input.
 * @retval uint_t The total count of item within input.
 */
const uint_t Utils::NumChar( const string& input, const string& item )
{
    UFLAGS_DE( flags );
    uint_t amount = 0, i = 0;

    if ( input.empty() )
    {
        LOGSTR( flags, "Utils::NumChar()-> called with empty input" );
        return amount;
    }

    for ( i = 0; i < input.length(); i++ )
        if ( input[i] == item[0] )
            amount++;

    return amount;
}

/**
 * @brief Returns a given time as a string.
 * @param[in] now A time_t to be formatted into a string.
 * @retval string A string value containing the human readable form of the contents of now.
 */
const string Utils::StrTime( const time_t& now )
{
    UFLAGS_DE( flags );
    string output;

    if ( ( output = ::ctime( &now ) ).empty() )
    {
        LOGSTR( flags, "Utils::StrTime()->ctime()-> returned NULL" );
        return output;
    }

    // Strip the newline off the end
    output.resize( output.length() - 1 );

    return output;
}

/**
 * @brief Returns a vector of strings split at spaces based on input.
 * @param[in] input A string to split on space characters.
 * @param[in] quiet If true, don't output error messages on fail.
 * @retval vector<string> A vector of strings that were split on the spaces detected from input.
 */
const vector<string> Utils::StrTokens( const string& input, const bool& quiet )
{
    UFLAGS_DE( flags );

    if ( input.empty() )
    {
        if ( !quiet )
            LOGSTR( flags, "Utils::StrTokens()-> called with empty input" );
        return vector<string>();
    }

    stringstream ss( input );
    istream_iterator<string> si( ss );
    istream_iterator<string> end;
    vector<string> output( si, end );

    return output;
}

/**
 * @brief This is a nested wrapper for Utils::__FormatString and should not be called directly.
 * @param[in] narg A #uint_t variable of the total number of arguments passed. Handled automatically.
 * @param[in] flags Any number of flags from #UTILS_OPTS to control output formatting and options.
 * @param[in] caller A string value containing the calling function. Handled automatically.
 * @param[in] fmt A string value containing a printf-style format string.
 * @param[in] ... A variable arguments list to populate fmt with.
 * @retval string A printf-style formatted string.
 */
const string Utils::_FormatString( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, ... )
{
    UFLAGS_DE( uflags );
    va_list args;
    string output;

    if ( fmt.empty() )
    {
        LOGSTR( uflags, "Utils::_FormatString()-> called with empty fmt" );
        return output;
    }

    va_start( args, fmt );
    output = __FormatString( narg, flags, caller, fmt, args );
    va_end( args );

    return output;
}

/**
 * @brief This is the printf-style string formatter. It should not be invoked directly, but rather by using Utils::FormatString() to ensure proper argument count and caller passing.
 * @param[in] narg A #uint_t variable of the total number of arguments passed. Handled automatically.
 * @param[in] flags Any number of flags from #UTILS_OPTS to control output formatting and options.
 * @param[in] caller A string value containing the calling function. Handled automatically.
 * @param[in] fmt A string value containing a printf-style format string.
 * @param[in] val A variable arguments list to populate fmt with.
 * @retval string A printf-style formatted string.
 */
const string Utils::__FormatString( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, va_list& val ) // Thanks go to Darien @ MudBytes.net for the start of this
{
    UFLAGS_DE( uflags );
    va_list args;
    vector<string> arguments;
    ITER( vector, string, si );
    vector<char> buf;
    string output, token;
    sint_t size = 0;

    if ( fmt.empty() )
    {
        LOGSTR( uflags, "Utils::__FormatString()-> called with empty fmt" );
        return output;
    }

    arguments = StrTokens( fmt );
    for ( si = arguments.begin(); si != arguments.end(); si++ )
    {
        token = *si;
        if ( token.find( "%" ) != string::npos ) // not foolproof, but it should catch some worst cases by attempting
            size++;                              // to ensure a matching narg : format specifier count
    }

    if ( narg != 1 && narg != static_cast<uint_t>( size ) && narg != NumChar( fmt, "%" ) ) // if narg == 1 invocation was func( flags, string )
    {
        bitset<CFG_MEM_MAX_BITSET> eflags;

        eflags.set( UTILS_TYPE_ERROR );
        Logger( eflags, "Number of arguments (%lu) did not match number of format specifiers (%lu) at: %s", narg, size, CSTR( caller ) );
        return output = "";
    }

    va_copy( args, val );
    size = vsnprintf( NULL, 0, CSTR( fmt ), args );
    va_end( args );

    va_copy( args, val );
    buf.resize( size + 1 );
    vsnprintf( &buf[0], ( size + 1 ), CSTR( fmt ), args );
    va_end( args );

    return output = &buf[0];
}

/**
 * @brief This is the logging output engine. It should not be invoked directly, but rather by calling Utils::Logger() to ensure proper argument count and caller passing.
 * @param[in] narg A #uint_t variable of the total number of arguments passed. Handled automatically.
 * @param[in] flags Any number of flags from #UTILS_OPTS to control output formatting and options.
 * @param[in] caller A string value containing the calling function. Handled automatically.
 * @param[in] fmt A string value containing a printf-style format string.
 * @param[in] ... A variable arguments list to populate fmt with.
 * @retval string A printf-style formatted string.
 */
const void Utils::_Logger( const uint_t& narg, const bitset<CFG_MEM_MAX_BITSET>& flags, const string& caller, const string& fmt, ... )
{
    UFLAGS_DE( uflags );
    va_list args;
    string pre, post, output;
    uint_t i = 0;

    if ( fmt.empty() )
    {
        LOGSTR( uflags, "Utils::_Logger()-> called with empty fmt" );
        return;
    }

    va_start( args, fmt );
    output = __FormatString( narg, flags, caller, fmt, args );
    va_end( args );

    if ( output.empty() )
        return;

    // prepend timestamp
    pre = StrTime( chrono::high_resolution_clock::to_time_t( g_global->m_time_current ) );
    pre.append( " :: " );

    for ( i = 0; i < MAX_UTILS; i++ )
    {
        if ( flags.test( i ) )
        {
            switch( i )
            {
                case UTILS_DEBUG: //output caller
                    post.append( " [" );
                    post.append( caller );
                    post.append( "]" );
                break;

                case UTILS_RAW: //no extraneous data applied
                    pre.clear();
                    post.clear();
                    i = MAX_UTILS;
                break;

                case UTILS_TYPE_ERROR: //so fancy!
                    pre.append( CFG_STR_UTILS_ERROR );
                break;

                case UTILS_TYPE_INFO: //so fancy!
                    pre.append( CFG_STR_UTILS_INFO );
                break;

                case UTILS_TYPE_SOCKET: //so fancy!
                    pre.append( CFG_STR_UTILS_SOCKET );
                break;

                default:
                break;
            }
        }
    }

    clog << pre << output << post << endl;
    /** @todo Add monitor channel support */

    return;
}
