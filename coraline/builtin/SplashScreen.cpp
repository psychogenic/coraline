/*
 * SplashScreen.cpp
 *
 *  Created on: Jan 6, 2018
 *        Copyright (C) 2018 Pat Deegan, https://psychogenic.com
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

#include "coraline/builtin/SplashScreen.h"
#include "coraline/corviewDebug.h"
#include "coraline/corviewDefs.h"
#include "coraline/Configuration.h"
#include "coraline/webview/corview_fileutil.hpp"
namespace Coraline {
namespace Plugin {

SplashScreen::SplashScreen(const Context& ctx) :
		Coraline::Plugin::Base(ctx),
		splashWindow(NULL)
{
}

SplashScreen::~SplashScreen() {

	if (splashWindow) {
		UI::destroyWindow(splashWindow);
		splashWindow = NULL;
	}

}



Coraline::Version SplashScreen::version() {
	Coraline::Version v(CORVIEW_PLUGINS_VERSION_MAJOR,
			CORVIEW_PLUGINS_VERSION_MINOR,
			CORVIEW_PLUGINS_VERSION_PATCH);
	return v;

}

void SplashScreen::startUp() {
	this->Coraline::Plugin::Base::startUp();

	this->executeJS("var splashscreen = nativebridge.SplashScreen;navigator.splashscreen = splashscreen;");

}

ClientCode SplashScreen::clientSideSupport() {
	return "function SplashScreen() {"
			"  this.show = function() {"
			"		cordova.exec(null, null, \"SplashScreen\", \"show\", []);"
			"  };"
			"  this.hide = function() {"
			"		cordova.exec(null, null, \"SplashScreen\", \"hide\", []);"
			"  };"
			"}";
}

AboutString SplashScreen::about() {
	return "Built-in SplashScreen Plugin, (C) 2018 Pat Deegan, psychogenic.com";
}

AboutString SplashScreen::usage() {
	return AboutString("Built-in SplashScreen Plugin, (C) 2018 Pat Deegan, psychogenic.com\n"
				"Usage: call "
				"\tsplashscreen.show() or \n"
				"\tsplashscreen.hide().\n"
				"Must have a suitable splash setup in config.xml."
				);

}

void SplashScreen::registerAllMethods() {
	PLUGINREGMETH(show);
	PLUGINREGMETH(hide);
}

bool SplashScreen::show(const StandardCallbackIDs& callbacks,
		const ArgsList& args) {

	CVDEBUG_OUT("Splashscreen::show called...");

	if (! splashWindow) {

		Configuration * config = Configuration::getInstance();
		if (config->splash().size()) {
			std::string splashPath = config->fullPathForContent(config->splash());
			if (fileExists(splashPath)) {
				CVDEBUG_OUT("constructing for " <<  splashPath );
				splashWindow = UI::windowForImageFile(splashPath);
			}
		}

	}

	if (splashWindow) {
		CVDEBUG_OUTLN("showing");
		UI::showWindow(splashWindow, CORALINE_IMAGEWINDOW_NOTIMEOUT);

		reportSuccess(callbacks);
	} else {
		CVDEBUG_OUTLN("nothing to show");
		reportError(callbacks);
	}

	return true;


}

bool SplashScreen::hide(const StandardCallbackIDs& callbacks,
		const ArgsList& args) {
	if (splashWindow) {
		CVDEBUG_OUTLN("SplashScreen::hide(): hiding window");
		UI::hideWindow(splashWindow);
	}

	CVDEBUG_OUTLN("SplashScreen::hide() -- nothing to hide");
	reportSuccess(callbacks);
	return true;


}

} /* namespace Plugin */
} /* namespace Coraline */
