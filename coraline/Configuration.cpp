/*
 * Configuration.cpp
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
#include "coraline/extlib/system.h"
#include "coraline/Configuration.h"
#include "coraline/extlib/system.h"
#include "coraline/corviewDefs.h"
#include "coraline/corviewDebug.h"
#include "coraline/extlib/xml2json/xml2json.hpp"

namespace Coraline {

Configuration * Configuration::config_singleton = NULL;

Configuration * Configuration::getInstance() {
	if (config_singleton) {
		return config_singleton;
	}
	config_singleton = new Configuration();

	return config_singleton;

}

Configuration::Configuration() : config_file_present(false) {


}

std::string Configuration::fullPathForContent(const std::string & relativePath) {
	std::string retPath(app_contentdir);

	if (! app_contentdir.size()) {
		return retPath;
	}

	retPath += CORVIEW_PATH_SEP + relativePath;

	return retPath;


}

void Configuration::setup(const ApplicationDetails & dets) {

	config_file_present = false;

	if (dets.app_dir) {
		app_dir = std::string(dets.app_dir);
	}

	if (dets.appc) {
		app_calc = std::string(dets.appc);

	}
	if (dets.called_as) {
		app_calledas = std::string(dets.called_as);

	}
	if (dets.name) {
		app_name = std::string(dets.name);

	}
	if (dets.rootc) {
		app_rootcalc = std::string(dets.rootc);

	}
	if (dets.title) {
		app_title = std::string(dets.title);

	}
	if (dets.url) {
		app_url = std::string(dets.url);

	}

	if (dets.content_dir) {
		app_contentdir = std::string(dets.content_dir);
	}
	if (dets.content_root) {
		app_contentroot = std::string(dets.content_root);
	}

	readConfigXML();
}

#define CONFIGXML_KEY_TOPLEVEL	"widget"
#define CONFIGXML_KEY_PLUGIN	"plugin"
#define CONFIGXML_KEY_NAME		"name"
#define CONFIGXML_KEY_ICON		"icon"
#define CONFIGXML_KEY_SPLASH	"splash"

#define CONFIGXML_ATTRIBKEY_WIDGETID		"@id"
#define CONFIGXML_ATTRIBKEY_NAME			"@name"
#define CONFIGXML_ATTRIBKEY_SOURCE			"@src"

bool Configuration::readConfigXML() {

	if (! app_contentdir.size()) {
		CVDEBUG_OUTLN("Config -- no contentroot dir");
		return false;
	}
	std::string confFileName(app_contentdir + CORVIEW_PATH_SEP + std::string(CORVIEW_APPCONFIG_FILENAME));
	std::ifstream inXML;
	inXML.open(confFileName);

	std::string asJSONStr;
	if (! inXML) {

		CVDEBUG_OUTLN("Config -- could not locate conf file: " << confFileName);
		return false;
	} else {

		std::stringstream buffer;
		buffer << inXML.rdbuf();

		asJSONStr = xml2json(buffer.str().c_str());
	}

	inXML.close();

	if (! asJSONStr.size() ) {

		CVDEBUG_OUTLN("could not parse " << confFileName);
		return false;
	}

	CVDEBUG_OUTLN("GOT JSON from  XML: " << asJSONStr);
	json inJson;
	try {
        inJson = json::parse(asJSONStr);
	} catch (std::invalid_argument&) {
        CVDEBUG_OUTLN("invalid json... boo");
        return false;
	}

	if (! inJson.count(CONFIGXML_KEY_TOPLEVEL))
	{


        CVDEBUG_OUTLN("no top level parent " <<  CONFIGXML_KEY_TOPLEVEL);
        return false;
	}

	config_file_present = true;

	json & topLev = inJson[CONFIGXML_KEY_TOPLEVEL];
	if (topLev.count(CONFIGXML_ATTRIBKEY_WIDGETID))
	{
		app_name = topLev[CONFIGXML_ATTRIBKEY_WIDGETID];
	}

	if (topLev.count(CONFIGXML_KEY_NAME)) {
		if (topLev[CONFIGXML_KEY_NAME].is_string()) {
			app_title = topLev[CONFIGXML_KEY_NAME];
		}
	}

	if (topLev.count(CONFIGXML_KEY_ICON)) {
		if (topLev[CONFIGXML_KEY_ICON].count(CONFIGXML_ATTRIBKEY_SOURCE)
				&&
				topLev[CONFIGXML_KEY_ICON][CONFIGXML_ATTRIBKEY_SOURCE].is_string()) {
			config_icon = topLev[CONFIGXML_KEY_ICON][CONFIGXML_ATTRIBKEY_SOURCE];

		}
	}

	if (topLev.count(CONFIGXML_KEY_SPLASH)) {
		if (topLev[CONFIGXML_KEY_SPLASH].count(CONFIGXML_ATTRIBKEY_SOURCE)
				&&
				topLev[CONFIGXML_KEY_SPLASH][CONFIGXML_ATTRIBKEY_SOURCE].is_string()) {
			config_splash = topLev[CONFIGXML_KEY_SPLASH][CONFIGXML_ATTRIBKEY_SOURCE];

		}

	}

	// json::size_type numPlugins = inJson[CONFIGXML_KEY_TOPLEVEL].count(CONFIGXML_KEY_PLUGIN);
	if (inJson[CONFIGXML_KEY_TOPLEVEL].count(CONFIGXML_KEY_PLUGIN)) {

        CVDEBUG_OUTLN("have some plugin requests!");

		if (inJson[CONFIGXML_KEY_TOPLEVEL][CONFIGXML_KEY_PLUGIN].is_array())
		{
			// iterate the array
			for (json::iterator it = inJson[CONFIGXML_KEY_TOPLEVEL][CONFIGXML_KEY_PLUGIN].begin();
					it != inJson[CONFIGXML_KEY_TOPLEVEL][CONFIGXML_KEY_PLUGIN].end(); ++it) {
				if ((*it).is_object()) {
					noteRequestedPlugin((*it));
				} else {
					CVERROR_OUTLN("Got weird non-object in configuration plugin array");
				}
			}

		} else if (inJson[CONFIGXML_KEY_TOPLEVEL][CONFIGXML_KEY_PLUGIN].is_object()) {
			noteRequestedPlugin(inJson[CONFIGXML_KEY_TOPLEVEL][CONFIGXML_KEY_PLUGIN]);
		}

	}

	return true;




}


bool Configuration::shouldLoad(Coraline::Plugin::Plugin* plugin)
{
	if (! config_file_present) {
		return true;
	}

	std::string shortName = plugin->shortName();
	std::string fullName = plugin->fullName();
	/*
	std::string sharedLibTerm(".so");

	if (pluginFname.size() >= sharedLibTerm.size()) {
		if (std::equal(sharedLibTerm.rbegin(), sharedLibTerm.rend(), pluginFname.rbegin()))
		{
			// ends with .so...

			pluginFname = pluginFname.substr(0, pluginFname.size() - sharedLibTerm.size());
		}
	}
	*/

	CVDEBUG_OUT("Config should load " << fullName << " ? ");

	if ( (requested_plugins.find(fullName) == requested_plugins.end()) &&
			(requested_plugins.find(shortName) == requested_plugins.end()) )
	{
		CVDEBUG_OUTLN("NO");
		return false;
	}

	CVDEBUG_OUTLN("YES");
	return true;

}

void Configuration::didLoad(Coraline::Plugin::Plugin * plugin) {
	if (! config_file_present ) {
		return;
	}
	RequestedPluginDetails & dets = requested_plugins[plugin->fullName()];
	dets.name = plugin->fullName();
	dets.version = plugin->version();
	dets.loaded = true;


}


bool Configuration::didLoadAllRequested() {
	for (RequestedPlugins::iterator it = requested_plugins.begin();
			it != requested_plugins.end(); it++){
		if (! ((*it).second.loaded)) {
			return false;
		}
	}
	return true;
}
void Configuration::noteRequestedPlugin(const json & inVal) {
	// "plugin2":[{"@name":"cordova-plugin-whitelist","@spec":"1"},{"@name":"cordova-plugin-file","@spec":"~4.2.0"}],"plugin":{"@name":"cordova-plugin-dialogs","@spec":"~1.2.1"}}}
	if (! inVal.count(CONFIGXML_ATTRIBKEY_NAME)) {
		CVDEBUG_OUTLN("No 'name' attrib for requested plugin... skipping");
		return;
	}
	std::string pName = inVal[CONFIGXML_ATTRIBKEY_NAME];
	CVDEBUG_OUTLN("Config file requesting plugin: " << pName);
	requested_plugins[pName] = RequestedPluginDetailsStruct();

	return;

}

Configuration::~Configuration() {

}

} /* namespace Coraline */
