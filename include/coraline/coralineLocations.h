/*
 * coralineLocations.h
 *
 *  Created on: Dec 23, 2017
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

#ifndef CORALINE_INCLUDE_CORALINE_CORALINELOCATIONS_H_
#define CORALINE_INCLUDE_CORALINE_CORALINELOCATIONS_H_


#ifndef CORALINE_SHARED_DIR
#warning "CORALINE_SHARED_DIR not set?"
#define CORALINE_SHARED_DIR		"/usr/local/share/coraline"
#endif


#ifndef CORALINE_SYSTEM_RESOURCES
#warning "CORALINE_SYSTEM_RESOURCES not set?"
#define CORALINE_SYSTEM_RESOURCES		"/usr/local/share/coraline/system"
#endif

#ifndef CORALINE_BIN_DIR
#define CORALINE_BIN_DIR				"/usr/local/bin"
#endif

#ifndef CONTENT_INSTALL_DIR
#warning "CONTENT_INSTALL_DIR not set?"
#define CONTENT_INSTALL_DIR		"/usr/local/share/coraline/content"
#endif


#ifndef PLUGINS_INSTALL_DIR
#define PLUGINS_INSTALL_DIR		"/usr/local/share/coraline/plugins"
#endif

#ifndef PLUGINS_RESOURCES_DIR
#define PLUGINS_RESOURCES_DIR		"/usr/local/share/coraline/plugin-resources"
#endif



#endif /* CORALINE_INCLUDE_CORALINE_CORALINELOCATIONS_H_ */
