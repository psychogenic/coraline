/*
 * StatusBar.cpp
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

#include "coraline/builtin/StatusBar.h"

namespace Coraline {
namespace Plugin {

StatusBar::StatusBar(const Context & ctx) : Coraline::Plugin::Base(ctx){

}

StatusBar::~StatusBar() {
}

Coraline::Version StatusBar::version() {
	Coraline::Version v(CORVIEW_PLUGINS_VERSION_MAJOR,
			CORVIEW_PLUGINS_VERSION_MINOR,
			CORVIEW_PLUGINS_VERSION_PATCH);
	return v;

}

void StatusBar::startUp() {
	this->Coraline::Plugin::Base::startUp();
	executeResourceJS("init.js");

}

void StatusBar::registerAllMethods() {
	// nothing to do...
}

} /* namespace Plugin */
} /* namespace Coraline */
