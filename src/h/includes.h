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
 * @file includes.h
 * @brief All non-system includes that need to be within each .cpp file.
 *
 * This file is a "header of headers" in that it only references other
 * headers within nzedb-backend that should be included to each .cpp file.
 * This header file should be the first file included within every .cpp file.
 */
#ifndef DEC_INCLUDES_H
#define DEC_INCLUDES_H

#include "sysincludes.h"
#include "config.h"
#include "class.h"

#endif
