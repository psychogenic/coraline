/*
 * pluginDev.h
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

#ifndef INCLUDES_WEBVIEW_PLUGINDEV_H_
#define INCLUDES_WEBVIEW_PLUGINDEV_H_
#include "Plugin.h"
#include "PluginBase.h"
#include "dynPlugin.h"
#include "../corviewDebug.h"
#include "../corviewTypes.h"

#define DYNPLUGIN_FUNCTIONNAME_SUPPORTSVERSION	"supports_driver"
#define DYNPLUGIN_FUNCTIONNAME_VERSION			"object_version"
#define DYNPLUGIN_FUNCTIONNAME_CREATE			"create_object"
#define DYNPLUGIN_FUNCTIONNAME_DESTROY			"destroy_object"

#endif /* INCLUDES_WEBVIEW_PLUGINDEV_H_ */
