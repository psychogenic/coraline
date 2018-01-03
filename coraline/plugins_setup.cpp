/*
 * plugins_setup.cpp
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


#include "coraline/corviewDebug.h"
#include "coraline/webview/corview_plugins.h"
#include "coraline/plugins/pluginDev.h"
#include "coraline/webview/Registry.h"
#include "coraline/webview/corview_fileutil.hpp"
#include <dlfcn.h>


#include "coraline/builtin/TestPlugin.h"
#include "coraline/builtin/NativeBridge.h"
#include "coraline/Configuration.h"


#include "../include/coraline/coralineDirs.h"
#include "coraline/coralineLocations.h"

void plugins_context_init(int argc, char* argv[], struct webview *w) {

	Coraline::Plugin::ContextFactory::setStartupArgs(argc, argv);
	Coraline::Plugin::ContextFactory::setView(w);
	Coraline::Configuration* config = Coraline::Configuration::getInstance();

	if (config->hadConfigFile()) {
		Coraline::Plugin::ContextFactory::refreshConfig();

	}
}

void config_icon_init() {
	static std::string iconImg;
	bool iconSet = false;
	GError * err = NULL;
	Coraline::Configuration* config = Coraline::Configuration::getInstance();
	if (config->hadConfigFile() && config->icon().size()) {
		CVDEBUG_OUT("Have icon in config: " << config->icon());
		iconImg = config->fullPathForContent(config->icon());
		if (fileExists(iconImg)) {
			CVDEBUG_OUTLN(" and file EXISTS. setting.");
			iconSet = true;

		} else {
			CVDEBUG_OUTLN(" but file DNE");
		}

	}

	if (! iconSet) {
		iconImg = CORALINE_SYSTEM_RESOURCES;
		iconImg += CORVIEW_PATH_SEP + std::string("icon.png");
		CVDEBUG_OUTLN("Using default icon.");
	}

	gtk_window_set_default_icon_from_file(iconImg.c_str(), &err);
	if (Coraline::Plugin::ContextFactory::get().topWindow) {
	gtk_window_set_icon_from_file(GTK_WINDOW(Coraline::Plugin::ContextFactory::get().topWindow), 
					iconImg.c_str(), &err);
	}
	Coraline::Plugin::ContextFactory::setIcon(iconImg.c_str());
}



static Coraline::Plugin::PluginList plugins_load_dynamic() {
	CVDEBUG_OUTLN("loading dynamic plugins");

	FilesInDirList filesInDir;
	std::string pathSep(CORVIEW_PATH_SEP);

	Coraline::Configuration * config = Coraline::Configuration::getInstance();

	const Coraline::Plugin::Context & ctx = Coraline::Plugin::ContextFactory::get();
	std::string pluginsDir(PLUGINS_INSTALL_DIR);

	/*
	if (ctx.application.app_dir) {
		// pluginsPrefix =  std::string(ctx.application.app_dir);
		pluginsDir = std::string(ctx.application.app_dir)
					+ pathSep
					+ pluginsDir;
	}
	*/
	Coraline::Plugin::PluginList myPlugins;
	std::string contentPluginsDir(config->contentDir() + "/plugins");
	std::map<std::string, bool> loadedAlready;

	if (getdir(contentPluginsDir, filesInDir) == 0) {
		CVERROR_OUTLN("have a plugindir in content...");
		for (FilesInDirList::iterator iter = filesInDir.begin();
					iter != filesInDir.end(); iter++) {
				if ((*iter).find(std::string(".so")) == std::string::npos) {
					CVDEBUG_OUTLN("skipping file " << *iter);
					continue;
				}
				std::string pPath(contentPluginsDir + pathSep);
				pPath += *iter;
				myPlugins.push_back(Coraline::Plugin::LoadablePlugin(pPath));
				loadedAlready[*iter] = true;


			}

	}
	if (getdir(pluginsDir, filesInDir) != 0) {
		CVERROR_OUTLN("Problem listing plugins...");
		return myPlugins;
	}

	for (FilesInDirList::iterator iter = filesInDir.begin();
			iter != filesInDir.end(); iter++) {

		if (loadedAlready[*iter]) {

			CVDEBUG_OUTLN("have override for " << *iter);
			continue;
		}
		if ((*iter).find(std::string(".so")) == std::string::npos) {
			CVDEBUG_OUTLN("skipping file " << *iter);
			continue;
		}
		std::string pPath(pluginsDir + pathSep);
		pPath += *iter;
		myPlugins.push_back(Coraline::Plugin::LoadablePlugin(pPath));

	}

	Coraline::Version thisVersion; // auto-initialized

	for (Coraline::Plugin::PluginList::iterator iter = myPlugins.begin();
			iter != myPlugins.end(); iter++) {

		Coraline::Plugin::LoadablePlugin & plg = *iter;
		CVDEBUG_OUT("Attempting to load dyn plugin " << (*iter).name << "... ");

		plg.handle = dlopen(plg.name.c_str(), RTLD_LAZY);
		if (! plg.handle) {

			CVDEBUG_OUTLN("ERROR: " << dlerror());
			CVDEBUG_OUTLN("Could not load!");
			continue;
		}

		Coraline::Plugin::SupportsVersion supported = (Coraline::Plugin::SupportsVersion)dlsym(plg.handle, DYNPLUGIN_FUNCTIONNAME_SUPPORTSVERSION);

		if (! supported) {
			CVERROR_OUTLN("Plugin " << (*iter).name << " doesn't have required function: " << DYNPLUGIN_FUNCTIONNAME_SUPPORTSVERSION);
			continue;
		}


		if (! supported(thisVersion)) {
			CVERROR_OUTLN("Plugin " << (*iter).name
					<< " does not support current version (v"
					<< (int)thisVersion.maj << '.'
					<< (int)thisVersion.min << '.'
					<< (int)thisVersion.patch << ')' );
			continue;

		}


		plg.create = (Coraline::Plugin::Creator)dlsym(plg.handle, DYNPLUGIN_FUNCTIONNAME_CREATE);
		plg.destroy =(Coraline::Plugin::Destroyer)dlsym(plg.handle, DYNPLUGIN_FUNCTIONNAME_DESTROY);
		plg.version = (Coraline::Plugin::VersionReport)dlsym(plg.handle, DYNPLUGIN_FUNCTIONNAME_VERSION);

		if (plg.version && plg.create && plg.destroy) {

			Coraline::Version plgVers = plg.version();
			CVDEBUG_OUTLN("Success! loaded version "
							<< (int)plgVers.maj << '.'
							<< (int)plgVers.min << '.'
							<< (int)plgVers.patch);
			plg.valid = true;
		} else {

			CVDEBUG_OUTLN("Missing create/destroy?");
			// leaving plg.valid = false...
		}




	}

	return myPlugins;
}

static void plugins_load_builtin(const Coraline::Plugin::Context & ctx) {
	CVDEBUG_OUTLN("loading built-in plugins");
	Coraline::Plugin::NativeBridge * nativeBrg = new Coraline::Plugin::NativeBridge(ctx);
	nativeBrg->initAndRegister();
	nativeBrg->injectCode();



	Coraline::Plugin::TestPlugin * tPlugin = new Coraline::Plugin::TestPlugin(ctx);
	tPlugin->initAndRegister();
	tPlugin->injectCode();

}


void plugins_register_all(struct webview *w) {

	const Coraline::Plugin::Context & ctx = Coraline::Plugin::ContextFactory::get();


	CVDEBUG_OUTLN("Application " <<
				ctx.application.name
				<< " (called as "
				<< ctx.application.called_as
				<< ") loading plugins.");

	plugins_load_builtin(ctx);

	Coraline::Configuration* config = Coraline::Configuration::getInstance();

	Coraline::Plugin::PluginList loadedPlugins = plugins_load_dynamic();
	for (Coraline::Plugin::PluginList::iterator iter = loadedPlugins.begin();
			iter != loadedPlugins.end(); iter++) {
		CVDEBUG_OUT("attempting to register dyn plugin " << (*iter).name << "... ");
		if (!(*iter).valid) {
			CVDEBUG_OUTLN("but is not valid.");
			continue;
		}
		Coraline::Plugin::Plugin * aPlug = (*iter).create(ctx);
		if (!aPlug) {

			CVDEBUG_OUTLN("Could not construct.");
			continue;
		}
		if (!config->shouldLoad(aPlug)) {
			CVDEBUG_OUTLN("But not required by config, destroying.");
			(*iter).destroy(aPlug);
			dlclose((*iter).handle);
			(*iter).valid = false;
			continue;

		}

		config->didLoad(aPlug);
		CVDEBUG_OUTLN("Yes, registering.");
		aPlug->initAndRegister();

		Coraline::Plugin::Base * bPlugin =
				dynamic_cast<Coraline::Plugin::Base*>(aPlug);
		if (bPlugin) {
			CVDEBUG_OUT("setting dyn loaded... ");
			bPlugin->setDynamicallyLoaded(*iter);
			bPlugin->injectCode();
		}


	}

	if (config->didLoadAllRequested())  {
		CVDEBUG_OUTLN("All requested plugins loaded for content in "
				<< Coraline::Configuration::getInstance()->contentDir()
				);
	} else {
	CVERROR_OUTLN("Some required plugins not loaded?  check config.xml in "
			<< config->contentDir()
			<<
			" and your setup!");
	}



}


void plugins_start_all(struct webview *w) {

	Coraline::Plugin::Registry * reg = Coraline::Plugin::Registry::getInstance();

	for (Coraline::Plugin::PluginMap::iterator iter
			= reg->iterator();
			iter != reg->iteratorEnd();
			iter++)
	{
		((*iter).second)->startUp();
	}


}

void plugins_deviceready_signal() {

	Coraline::Plugin::Registry * reg = Coraline::Plugin::Registry::getInstance();

	Coraline::Plugin::NativeBridge * nb =
			reg->findAs<Coraline::Plugin::NativeBridge>(CORVIEW_NATIVEBRIDGE_PLUGINNAME);

	if (nb) {
		nb->notifyDeviceReady();
	}
}
void plugins_update_all() {

	Coraline::Plugin::Registry * reg = Coraline::Plugin::Registry::getInstance();

	for (Coraline::Plugin::PluginMap::iterator iter
			= reg->iterator();
			iter != reg->iteratorEnd();
			iter++)
	{
		((*iter).second)->update();
	}
}



void plugins_shutdown_all() {

	CVDEBUG_OUTLN("Shutting down all plugins");

	Coraline::Plugin::Registry * reg = Coraline::Plugin::Registry::getInstance();

	for (Coraline::Plugin::PluginMap::iterator iter
			= reg->iterator();
			iter != reg->iteratorEnd();
			iter++)
	{
		((*iter).second)->shutdown();
		reg->remove((*iter).second);

	}
}




