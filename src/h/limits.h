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
 * @file limits.h
 * @brief Custom defined numeric types and system limits.
 *
 * nzedb-backend utilizes two numeric types, both defined within this file.
 * These types, #sint_t (signed) and #uint_t (unsigned) are implemented as
 * the numeric maximums of the host operating system. Further OS-specific
 * limitations may be placed here in the future.
 */
#ifndef DEC_LIMITS_H
#define DEC_LIMITS_H

using namespace std;

#if __WORDSIZE == 64
    /** @name Signed */ /**@{*/
    /**
     * @typedef sint_t
     * @brief A signed integer value based upon system limits.
     */
    typedef signed long int sint_t;

    /**
     * @def sintmax_t
     * @brief The maximum value of any #sint_t variable.
     */
    #define sintmax_t numeric_limits<signed long int>::max()

    /**
     * @def sintmin_t
     * @brief The minimum value of any #sint_t variable.
     */
    #define sintmin_t numeric_limits<signed long int>::min()
    /**@}*/

    /** @name Unsigned */ /**@{*/
    /**
     * @typedef uint_t
     * @brief An unsigned integer value based upon system limits.
     */
    typedef unsigned long int uint_t;

    /**
     * @def uintmax_t
     * @brief The maximum value of any #uint_t variable.
     */
    #define uintmax_t numeric_limits<unsigned long int>::max()

    /**
     * @def uintmin_t
     * @brief The minimum value of any #uint_t variable.
     */
    #define uintmin_t numeric_limits<unsigned long int>::min()
    /**@}*/
#else
    /** @name Signed */ /**@{*/
    /**
     * @typedef sint_t
     * @brief A signed integer value based upon system limits.
     */
    typedef signed int sint_t;

    /**
     * @def sintmax_t
     * @brief The maximum value of any #sint_t variable.
     */
    #define sintmax_t numeric_limits<signed int>::max()

    /**
     * @def sintmin_t
     * @brief The minimum value of any #sint_t variable.
     */
    #define sintmin_t numeric_limits<signed int>::min()
    /**@}*/

    /** @name Unsigned */ /**@{*/
    /**
     * @typedef uint_t
     * @brief An unsigned integer value based upon system limits.
     */
    typedef unsigned int uint_t;

    /**
     * @def uintmax_t
     * @brief The maximum value of any #uint_t variable.
     */
    #define uintmax_t numeric_limits<unsigned int>::max()

    /**
     * @def uintmin_t
     * @brief The minimum value of any #uint_t variable.
     */
    #define uintmin_t numeric_limits<unsigned int>::min()
    /**@}*/
#endif

#endif
