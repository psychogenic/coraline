/*
 * dynPlugin.h
 *
 *  Created on: Dec 4, 2017
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

#ifndef INCLUDES_PLUGINS_DYNPLUGIN_H_
#define INCLUDES_PLUGINS_DYNPLUGIN_H_

#include "../corviewTypes.h"
#include "Plugin.h"
#include "Context.h"


namespace Coraline {
namespace Plugin {


typedef bool (*SupportsVersion)(const Coraline::Version & v);
typedef Coraline::Version (*VersionReport)();
typedef Coraline::Plugin::Plugin * (*Creator)(const Coraline::Plugin::Context & ctx);
typedef void (*Destroyer)(Coraline::Plugin::Plugin*);
typedef struct LoadablePluginStruct {
	std::string name;
	bool valid;
	void * handle;
	VersionReport version;
	Creator create;
	Destroyer destroy;

	LoadablePluginStruct() : name(""), valid(false),
			handle(NULL), version(NULL),
			create(NULL), destroy(NULL) {

	}
	LoadablePluginStruct(std::string nm, bool isValid=false) :
		name(nm), valid(isValid),
		handle(NULL), version(NULL),
		create(NULL), destroy(NULL) {

	}

} LoadablePlugin;

typedef std::vector<LoadablePlugin> PluginList;





} /* namespace Plugin */
} /* namespace Coraline */





#endif /* INCLUDES_PLUGINS_DYNPLUGIN_H_ */
