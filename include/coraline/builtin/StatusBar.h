/*
 * SatusBar.h
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

#ifndef CORALINE_CORALINE_BUILTIN_SATUSBAR_H_
#define CORALINE_CORALINE_BUILTIN_SATUSBAR_H_

#include "../plugins/pluginDev.h"
namespace Coraline {
namespace Plugin {

class StatusBar : public Coraline::Plugin::Base {
public:
	StatusBar(const Context & ctx);
	virtual ~StatusBar();

	Coraline::Version version();
    virtual const PluginName fullName() { return "cordova-plugin-statusbar"; }
    virtual const PluginName shortName() { return "StatusBar";}

	virtual void startUp();


protected:
	virtual void registerAllMethods();

};

} /* namespace Plugin */
} /* namespace Coraline */

#endif /* CORALINE_CORALINE_BUILTIN_SATUSBAR_H_ */
