/*
 * TestPlugin.cpp
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

#include "coraline/builtin/TestPlugin.h"
#include "coraline/corviewDebug.h"
#include "coraline/corviewDefs.h"

namespace Coraline {
namespace Plugin {

TestPlugin::TestPlugin(const Context & ctx) : Coraline::Plugin::Base(ctx) {

}

TestPlugin::~TestPlugin() {

}

Coraline::Version TestPlugin::version() {
	Coraline::Version v(CORVIEW_PLUGINS_VERSION_MAJOR,
			CORVIEW_PLUGINS_VERSION_MINOR,
			CORVIEW_PLUGINS_VERSION_PATCH);
	return v;

}
void TestPlugin::registerAllMethods() {


	CVDEBUG_OUTLN("TestPlugin::registerAllMethods.");
	PLUGINREGMETH(willSucceed);
	PLUGINREGMETH(bornToFail);

}

AboutString TestPlugin::about() {
	return "Built-in Test Plugin, (C) 2017 Pat Deegan, psychogenic.com";
}

AboutString TestPlugin::usage() {
	return AboutString("Built-in Test Plugin, (C) 2017 Pat Deegan, psychogenic.com\n"
			"Usage: call "
			"\tcordova.Test.willSucceed(successCb, failCb) or \n"
			"\tcordova.Test.bornToFail(successCb, failCb) to try it out."
			);

}


bool TestPlugin::willSucceed(const StandardCallbackIDs & callbacks, const ArgsList & args) {

	CVDEBUG_OUTLN("TestPlugin::test1!");
	json ho;
	ho["success"] = true;
	this->triggerCallback(callbacks.success, ho);
	return true;

}
bool TestPlugin::bornToFail(const StandardCallbackIDs & callbacks, const ArgsList & args) {
	CVDEBUG_OUTLN("TestPlugin::test2!");

	json ho;
	ho["success"] = false;
	ho["error"] = "this sux, but that was expected";
	this->triggerCallback(callbacks.error, ho);
	return false;

}

} /* namespace Plugin */
} /* namespace Coraline */
