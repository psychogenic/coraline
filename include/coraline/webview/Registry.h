/*
 * Registry.h
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

#ifndef INCLUDES_REGISTRY_H_
#define INCLUDES_REGISTRY_H_
#include "../corviewTypes.h"
#include "../plugins/Plugin.h"
#include "../extlib/system.h"

namespace Coraline {
namespace Plugin {

typedef std::map<PluginName, Plugin*> PluginMap;

class Registry {
public:
	static Registry * getInstance();

	bool add(Plugin* plugin);
	bool remove(Plugin* plugin);
	bool remove(const PluginName & plug);

	bool has(const PluginName & plug);
	Plugin* find(const PluginName & plug);

	template<class T>
	T* findAs(const PluginName & plug);

	PluginMap::iterator iterator() { return pluginNamesToPtr.begin();}
	PluginMap::iterator iteratorEnd() { return pluginNamesToPtr.end();}

private:
	static Registry * reg_singleton;
	PluginMap pluginNamesToPtr;
	Registry();
	virtual ~Registry();
};


template<class T>
T* Registry::findAs(const PluginName & plug) {
	Plugin* p = this->find(plug);
	if (! p)
	{
		return NULL;
	}

	T* asT = dynamic_cast<T*>(p);
	return asT;
}

} /* namespace Plugin */
} /* namespace Coraline */

#endif /* INCLUDES_REGISTRY_H_ */
