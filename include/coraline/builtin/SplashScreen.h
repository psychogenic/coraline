/*
 * SplashScreen.h
 *
 *  Created on: Jan 6, 2018
 *
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

#ifndef CORALINE_CORALINE_BUILTIN_SPLASHSCREEN_H_
#define CORALINE_CORALINE_BUILTIN_SPLASHSCREEN_H_

#include "../plugins/pluginDev.h"
#include "../webview/window_utils.h"

namespace Coraline {
namespace Plugin {

class SplashScreen : public Coraline::Plugin::Base{
public:
	SplashScreen(const Context & ctx);
	virtual ~SplashScreen();

	Coraline::Version version();
    virtual const PluginName fullName() { return "cordova-plugin-splashscreen"; }
    virtual const PluginName shortName() { return "SplashScreen";}



	virtual void startUp();
	virtual ClientCode clientSideSupport();

	virtual AboutString about();
	virtual AboutString usage();

protected:
	virtual void registerAllMethods();

private:
	bool show(const StandardCallbackIDs & callbacks, const ArgsList & args);
	bool hide(const StandardCallbackIDs & callbacks, const ArgsList & args);

	UI::DialogWindow * splashWindow;

};

} /* namespace Plugin */
} /* namespace Coraline */

#endif /* CORALINE_CORALINE_BUILTIN_SPLASHSCREEN_H_ */
