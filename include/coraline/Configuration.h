/*
 * Configuration.h
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

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include "extlib/system.h"
#include "./corviewTypes.h"
#include "plugins/dynPlugin.h"

namespace Coraline {

typedef struct RequestedPluginDetailsStruct {
	PluginName name;
	Version version;
	bool loaded;
	RequestedPluginDetailsStruct() : name(""), loaded(false) {

	}

} RequestedPluginDetails;

typedef std::string RequestedPluginName;

typedef std::map<RequestedPluginName, RequestedPluginDetails> RequestedPlugins;

class Configuration {
public:
	static Configuration * getInstance();

	void setup(const ApplicationDetails & dets);

	virtual ~Configuration();



	inline const std::string & name() { return app_name;}
	inline const std::string & title() { return app_title;}

	inline const std::string & contentDir() { return app_contentdir;}
	inline const std::string & contentRoot() { return app_contentroot;}

	inline const std::string & url() { return app_url;}
	inline const std::string & appDir() { return app_dir;}
	inline const std::string & calledas() { return app_calledas;}
	inline const std::string & calc() { return app_calc;}
	inline const std::string & contentcalc() { return app_rootcalc;}
	inline uint8_t versionMajor() { return driver_version.maj;}
	inline uint8_t versionMinor() { return driver_version.min;}
	inline uint8_t versionPatchLevel() { return driver_version.patch;}
	inline const Version & version() { return driver_version;}

	inline const std::string & icon() { return config_icon; }
	inline const std::string & splash() { return config_splash; }

	std::string fullPathForContent(const std::string & relativePath);


	inline bool hadConfigFile() { return config_file_present;}

	bool shouldLoad(Coraline::Plugin::Plugin* plugin);
	void didLoad(Coraline::Plugin::Plugin * plugin);
	bool didLoadAllRequested();

private:
	static Configuration * config_singleton;
	Configuration();
	bool readConfigXML();

	void noteRequestedPlugin(const json & inVal);
	RequestedPlugins requested_plugins;
	bool config_file_present;


	std::string app_name;
	std::string app_title;
	std::string app_url;
	std::string app_dir;
	std::string app_contentroot;
	std::string app_contentdir;
	std::string app_calledas;
	std::string app_calc;
	std::string app_rootcalc;
	std::string config_icon;
	std::string config_splash;

	Version driver_version;
};

} /* namespace Coraline */

#endif /* CONFIGURATION_H_ */
