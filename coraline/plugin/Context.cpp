/*
 * Context.cpp
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


#include "coraline/corviewDebug.h"
#include "coraline/plugins/Context.h"
#include "coraline/webview/corview_fileutil.hpp"
#include "coraline/corviewConfig.h"
#include "coraline/extlib/sha512.h"
#include "coraline/webview/webview.h"
#include "coraline/Configuration.h"
#include "../include/coraline/coralineDirs.h"

namespace Coraline {
namespace Plugin {


const Context & ContextFactory::get() {
	return ctx_singleton;
}

void ContextFactory::setStartupArgs(int argc, char* argv[]) {
	std::string rootFileName(rootFile(argc, argv));
	static std::string rootc;
	static std::string appc;
	static std::string appdir;
	static std::string contentdir;
	static std::string rootfname;

	std::string::size_type lastSlashPos;
	ctx_singleton.application.name = APPLICATION_NAME;

	if (argc) {
		std::string calledApp(argv[0]);

		if (calledApp.size()) {

			ctx_singleton.application.called_as = argv[0];

			CVDEBUG_OUT("Loading app  " << ctx_singleton.application.called_as);
			std::string fullPathAttempt = calledApp;
			if (! fileExists(calledApp)) {

				fullPathAttempt = CORALINE_BIN_DIR;
				fullPathAttempt = fullPathAttempt + CORVIEW_PATH_SEP + calledApp;

			}



			appc = sw::sha512::file(fullPathAttempt);
			ctx_singleton.application.appc = appc.c_str();

			CVDEBUG_OUTLN(" @ \n" << appc);

			appdir = basedir(calledApp);
			if (appdir.size()) {
				ctx_singleton.application.app_dir = appdir.c_str();
			}
		}


	}

	CVDEBUG_OUT("Loading root  " << rootFileName);
	lastSlashPos = rootFileName.find_last_of(CORVIEW_PATH_SEP);
	rootfname = rootFileName;

	if (lastSlashPos == std::string::npos || lastSlashPos < 2) {

		rootc = sw::sha512::file(rootFileName);
	} else {

		std::string rootDirName(rootFileName.substr(0, lastSlashPos));
		contentdir = rootDirName;
		FilesInDirList allFiles;
		getdir(rootDirName, allFiles);
		std::string combinedSha("");
		for (FilesInDirList::iterator iter = allFiles.begin();
				iter != allFiles.end(); iter++)
		{
			if ((*iter).find(std::string("/fonts/")) == std::string::npos) {

				std::string fname(rootDirName + CORVIEW_PATH_SEP + *iter);
				CVDEBUG_OUTLN("Have file " << *iter);
				combinedSha += sw::sha512::file(fname);
			}
		}

		// CVDEBUG_OUTLN("COMBINED SHAW \n" << combinedSha << "\n\n");

		rootc = sw::sha512::calculate(combinedSha);
	}

	CVDEBUG_OUTLN(" @ \n" << rootc);
	ctx_singleton.application.rootc = rootc.c_str();
	ctx_singleton.application.content_root = rootfname.c_str();
	ctx_singleton.application.content_dir = contentdir.c_str();


	Configuration::getInstance()->setup(ctx_singleton.application);



}
void ContextFactory::setView(void * w) {
	ctx_singleton.view = w;
	struct webview * wv = (struct webview *)w;
	ctx_singleton.application.title = wv->title;
	ctx_singleton.application.url = wv->url;
	ctx_singleton.topWindow = (wv->priv).window;


}


void ContextFactory::setIcon(const char * iconFilePath) {
	CVDEBUG_OUTLN("SETTING ICON PATH TO " << iconFilePath);
	ctx_singleton.application.icon = iconFilePath;
}

void ContextFactory::refreshConfig() {
	static std::string iconPath;

	Coraline::Configuration * config = Coraline::Configuration::getInstance();
	ctx_singleton.application.name = config->name().c_str();
	ctx_singleton.application.title = config->title().c_str();

	if (config->hadConfigFile() && config->icon().size()) {

		iconPath = config->fullPathForContent(config->icon());
		CVDEBUG_OUTLN("ICON OVERRIDE! " << iconPath);

		ctx_singleton.application.icon = iconPath.c_str();
	}

	struct webview * wv = (struct webview *)ctx_singleton.view;
	wv->title = ctx_singleton.application.title;


}
Context ContextFactory::ctx_singleton;


} /* namespace Plugin */
} /* namespace Coraline */



