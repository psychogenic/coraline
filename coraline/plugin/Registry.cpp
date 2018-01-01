/*
 * Registry.cpp
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
#include "coraline/extlib/system.h"

#include "coraline/webview/Registry.h"
#include "coraline/corviewDebug.h"
#include "coraline/plugins/PluginBase.h"


namespace Coraline {
namespace Plugin {
Registry * Registry::reg_singleton = NULL;

Registry::Registry() {


}

Registry* Registry::getInstance() {
	if (reg_singleton) {
		return reg_singleton;

	}

	reg_singleton = new Registry();
	return reg_singleton;
}

bool Registry::add(Plugin* plugin) {

	const PluginName & pname = plugin->shortName();
	if (has(pname)) {
		CVDEBUG_OUTLN("Already have a plugin '" << pname << "' -- won't register.");
		return false;
	}

	CVDEBUG_OUTLN("Registering plugin: " << pname);
	pluginNamesToPtr[pname] = plugin;


	return true;
}

bool Registry::remove(Plugin* plugin) {

	return remove(plugin->fullName());

}

bool Registry::remove(const PluginName& pname) {

	PluginMap::iterator pIter = pluginNamesToPtr.find(pname);
	if (pIter == pluginNamesToPtr.end()) {
		return false;
	}

	CVDEBUG_OUT("Removing plugin " << pname << " from registry...");


	if ( ! ((*pIter).second)->dynamicallyLoaded() ) {

		CVDEBUG_OUT("is internal, freeing... ");
		delete (*pIter).second;
	} else {

		CVDEBUG_OUT("is dyn loaded... ");
		Coraline::Plugin::Base * bPlugin = dynamic_cast<Coraline::Plugin::Base*>((*pIter).second);
		if (bPlugin) {
			const Coraline::Plugin::LoadablePlugin & dynSets = bPlugin->dynamicSettings();

			void * dlHandle = dynSets.handle;
			if (dynSets.valid && dynSets.destroy) {
				CVDEBUG_OUT("destroying... ");
				dynSets.destroy(bPlugin);
			}

			if (dlHandle) {
				dlclose(dlHandle);
			}

		}
	}

	pluginNamesToPtr.erase(pIter);
	CVDEBUG_OUTLN("Done.");
	return true;
}

bool Registry::has(const PluginName& plug) {
	if (pluginNamesToPtr.find(plug) != pluginNamesToPtr.end()){
		return true;
	}
	return false;
}

Plugin* Registry::find(const PluginName& plug) {

	PluginMap::iterator pIter = pluginNamesToPtr.find(plug);
	if (pIter == pluginNamesToPtr.end()) {
		return NULL;
	}
	return (*pIter).second;
}

Registry::~Registry() {
	// TODO Auto-generated destructor stub
}

} /* namespace Plugin */
} /* namespace Coraline */
