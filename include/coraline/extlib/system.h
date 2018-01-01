/*
 * system.h
 *
 *  Created on: Dec 15, 2017
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

#ifndef INCLUDES_EXTLIB_SYSTEM_H_
#define INCLUDES_EXTLIB_SYSTEM_H_

#include <inttypes.h>
#include <stdint.h>
#include <dlfcn.h>


#include <fstream>

#include <sstream>      // std::stringstream
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <iostream>

#ifndef CORALINE_GTK_AVAILABLE
#ifdef WEBVIEW_GTK
#define CORALINE_GTK_AVAILABLE
#endif
#endif

#ifdef CORALINE_GTK_AVAILABLE
#include <gtk/gtk.h>
#endif


#endif /* INCLUDES_EXTLIB_SYSTEM_H_ */
