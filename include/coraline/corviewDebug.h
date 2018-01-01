/*
 * corviewDebug.h
 *
 *  Created on: Dec 2, 2017
 *      
 *        Copyright (C) 2017 Pat Deegan, https://psychogenic.com
 *  
 *  This file is part of Coraline [https://coraline.psychogenic.com/]
 *
 *  Coraline is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Coraline is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Coraline.  If not, see <http://www.gnu.org/licenses/>.
 *  
 */

#ifndef INCLUDES_CORVIEWDEBUG_H_
#define INCLUDES_CORVIEWDEBUG_H_

#include <iostream>
#include "corviewConfig.h"

#define CVERROR_OUT(...)		std::cerr << __VA_ARGS__
#define CVERROR_OUTLN(...)		std::cerr << __VA_ARGS__ << std::endl


#ifdef CORVIEWDEBUG
#define CVDEBUG_OUT(...)		std::cerr << __VA_ARGS__
#define CVDEBUG_OUTLN(...)		std::cerr << __VA_ARGS__ << std::endl

#else
#define CVDEBUG_OUT(...)
#define CVDEBUG_OUTLN(...)

#endif



#endif /* INCLUDES_CORVIEWDEBUG_H_ */
