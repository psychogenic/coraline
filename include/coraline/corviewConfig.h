/*
 * corviewConfig.h
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

#ifndef INCLUDES_CORVIEWCONFIG_H_
#define INCLUDES_CORVIEWCONFIG_H_



//define CORVIEWDEBUG

#define APPLICATION_NAME		"com.psychogenic.coraline"

#define VIEWTITLE				"Coraline"

#define DEFAULT_TOPFILE			"default/index.html"

#define STARTWIDTH		1150
#define STARTHEIGHT		900

#define VIEWRESIZABLE	1
#ifdef CORVIEWDEBUG
#define VIEWDEBUG		1
#else
#define VIEWDEBUG		0
#endif

#endif /* INCLUDES_CORVIEWCONFIG_H_ */
