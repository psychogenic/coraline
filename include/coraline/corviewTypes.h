/*
 * corviewTypes.h
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

#ifndef INCLUDES_CORVIEWTYPES_H_
#define INCLUDES_CORVIEWTYPES_H_

#include "./extlib/system.h"
#include "./extlib/json.hpp"
#include "./corviewDefs.h"


using json = nlohmann::json;

namespace Coraline {
#ifdef CORALINE_GTK_AVAILABLE
typedef GtkWidget TopLevelWindow;
#else
typedef void 	TopLevelWindow;
#endif


typedef CORVIEW_PACKEDSTRUCT ApplicationDetsStruct {

	const char * name;
	const char * title;
	const char * url;
	const char * content_root;
	const char * content_dir;
	const char * app_dir;
	const char * called_as;
	const char * icon;
	const char * appc;
	const char * rootc;
	char reserved[32];

	ApplicationDetsStruct() : name(NULL),
			title(NULL),
			url(NULL),
			content_root(NULL),
			content_dir(NULL),
			app_dir(NULL),
			called_as(NULL),
			icon(NULL),
			appc(NULL),
			rootc(NULL)
	{

	}

} ApplicationDetails;

typedef CORVIEW_PACKEDSTRUCT VersionStruct {
	uint8_t maj;
	uint8_t min;
	uint8_t patch;

	VersionStruct() :
		maj(CORVIEW_VERSION_MAJOR), min(CORVIEW_VERSION_MINOR),
		patch(CORVIEW_VERSION_PATCH) {}

	VersionStruct(uint8_t mj, uint8_t mn, uint8_t p) :
		maj(mj), min(mn), patch(p) {

	}
} Version;


typedef std::string PluginString;
typedef PluginString MethodName;
typedef PluginString PluginName;
typedef int CallbackID;

namespace Plugin {

typedef std::string ClientCode;
typedef std::string ClientCodeFileName;
typedef std::string AboutString;


typedef CORVIEW_PACKEDSTRUCT PluginContextStruct {
	Version driver_version;
	ApplicationDetails application;
	void* view; // void* to shield derivatives
	TopLevelWindow * topWindow;
	void * reservedPtr;
	uint8_t reserved[64];
	PluginContextStruct() : driver_version(), view(NULL), topWindow(NULL),
			reservedPtr(NULL) {

	}

} Context;

typedef struct StandardCallbacksStruct {
	CallbackID success;
	CallbackID error;
} StandardCallbackIDs;


typedef json::array_t ArgsList;
typedef std::function<bool(const StandardCallbackIDs &, const ArgsList &)> CallableMethod;
// typedef lambda(const ArgsList & args) CallableMethod;


}

} /* namespace Coraline */






#endif /* INCLUDES_CORVIEWTYPES_H_ */
