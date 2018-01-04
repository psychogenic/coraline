/*
 * corviewDefs.h
 *
 *  Created on: Dec 3, 2017
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

#ifndef INCLUDES_WEBVIEW_CORVIEWDEFS_H_
#define INCLUDES_WEBVIEW_CORVIEWDEFS_H_

#define CORVIEW_VERSION_MAJOR		1
#define CORVIEW_VERSION_MINOR		0
#define CORVIEW_VERSION_PATCH		9

#define CORVIEW_PLUGINS_VERSION_MAJOR	1
#define CORVIEW_PLUGINS_VERSION_MINOR	1
#define CORVIEW_PLUGINS_VERSION_PATCH	2

#define CORVIEW_NATIVEBRIDGE_PLUGINNAME		"NativeBridge"
#define CORVIEW_MAINLOOP_SLEEPUS			2200

//define CORVIEW_PLUGINS_DIRNAME				"plugins"
//define CORVIEW_PLUGINRESOURCES_DIRNAME		"plugin-resources"
#define CORVIEW_APPCONFIG_FILENAME			"config.xml"

#define CORVIEW_PACKEDSTRUCT struct __attribute__ ((__packed__))

#define CORVIEW_PATH_SEP			"/"
#define CORVIEW_DEFAULT_TOPFILE		"index.html"


#endif /* INCLUDES_WEBVIEW_CORVIEWDEFS_H_ */
