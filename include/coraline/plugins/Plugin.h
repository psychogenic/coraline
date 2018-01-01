/*
 * Plugin.h
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

#ifndef INCLUDES_PLUGIN_H_
#define INCLUDES_PLUGIN_H_

#include "../corviewTypes.h"




namespace Coraline {
namespace Plugin {



class Plugin {
public:
	// lifecycle (override as required)
	virtual void startUp() = 0; // basic startup, called just prior to ondeviceready
	virtual void update() = 0; // periodic update
	virtual void shutdown() = 0; // prior to quit

	// query (override)
	virtual Coraline::Version version() = 0;


	// fullname, eg com.psychogenic.device
	// this is the name of the loadable plugin file ("com.psychogenic.device.so")
    virtual const PluginName fullName()  = 0;

    virtual const std::string merges() = 0;
    virtual const std::string clobbers() = 0;

    // shortname, e.g. Device
    // this is the name of the "class" -- something we'll be able to
    // call new on (new Device()).  Such an object is created and is then
    // available as nativebridge.Device (so we're doing nativebridge.Device = new Device()
    // in this example)
    virtual const PluginName shortName() = 0;

    /* internal usage */

    // accessing plugin methods
    virtual bool hasMethod(const MethodName & name) = 0;
    virtual bool callMethod(const MethodName & name,
    		const StandardCallbackIDs & callbacks,
    		const ArgsList & args) = 0;


	// * management
	virtual void initAndRegister() = 0;
	virtual bool dynamicallyLoaded() = 0;

	 /*
	     * Supporting code -- on plugin instantiation you may provide:
	     *  * a JS resource file to load
	     *  * some JS code, as a string
	     *
	     * and these will loaded file first, if it exists, then
	     * JS code string (if it is non-empty).
	     *
	     * *IF* any such code is loaded, it is assumed that we can instantiate
	     * a variable based on shortName(), meaning if shortName() returns
	     * "Device", we will be running
	     * 	 somevar = new Device();
	     * after the supporting code loads.
	     */
	virtual ClientCode clientSideSupport() = 0;
	virtual ClientCodeFileName clientSideSupportFile() = 0;

	virtual AboutString about() = 0;
	virtual AboutString usage() = 0;

	Plugin() {}
	virtual ~Plugin() {}

	Coraline::Version interfaceVersion() {

		return Coraline::Version(CORVIEW_PLUGINS_VERSION_MAJOR,
				CORVIEW_PLUGINS_VERSION_MINOR,
				CORVIEW_PLUGINS_VERSION_PATCH);
	}

};


} /* namespace Plugin */
} /* namespace Coraline */

#endif /* INCLUDES_PLUGIN_H_ */
