/*
 * Dispatcher.cpp
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

#include "coraline/extlib/system.h"
#include "coraline/corviewTypes.h"
#include "coraline/corviewDebug.h"
#include "coraline/webview/Dispatcher.h"
#include "coraline/webview/Registry.h"

#ifndef NULL
#define NULL 	0;
#endif

namespace Coraline {

Dispatcher * Dispatcher::disp_singleton = NULL;

Dispatcher::Dispatcher() {


}

Dispatcher::~Dispatcher() {
}


bool Dispatcher::dispatch(const std::string & str) {
	CVDEBUG_OUTLN("DISPATCH:" << str);

	try {
		json inJson = json::parse(str);

		//CVDEBUG_OUT("DISPATCH:" << inJson);
		for (json::iterator it = inJson.begin(); it != inJson.end(); ++it) {
			CVDEBUG_OUT("\t"  << it.key() << " : " << it.value() <<  std::endl);
		}

		if (! isValid(inJson)) {

			CVDEBUG_OUTLN("json... passed but does not meet our requirements...");
			return false;
		}

		// ok, looking good... check if the target plugin/method
		// actually exists
		if (! destinationExists(inJson["dest"])) {
			CVDEBUG_OUTLN("json... no such destination found...");
			return false;
		}


		Plugin::Plugin * plg = Plugin::Registry::getInstance()->find(inJson["dest"][0]);
		if (! plg ) {
			// shouldn't happen??
			CVERROR_OUTLN("destinationExists but plugin not returned??");
		}
		Plugin::StandardCallbackIDs cbids = {inJson["cb"][0], inJson["cb"][1]};

		if (! plg->callMethod(inJson["dest"][1], cbids, inJson["args"]) ) {

			CVERROR_OUTLN("plugin call has returned FALSE/failed?");
		}



	} catch (std::invalid_argument&) {
		CVDEBUG_OUTLN("invalid json... boo");
		return false;
	}

	return true;
}

bool Dispatcher::checkForArray(const std::string & name, const json & inVal, uint8_t minSize) {
	CVDEBUG_OUTLN("Checking JSON for value: " << name << " count says " << inVal.count(name));



	if (inVal.count(name) < 1) {
		CVDEBUG_OUTLN("Array " << name << " not present in JSON");
		return false;
	}

	if (inVal[name].is_array()) {
		if ( (! minSize ) || inVal[name].size() >= minSize) {
			CVDEBUG_OUTLN("Array " << name << " found in JSON");
			return true;
		}

		CVDEBUG_OUTLN("Array " << name << " found in JSON, but invalid size!");
		return false;
	}


	CVDEBUG_OUTLN("Value '" << name << "' found in JSON, but not an array!");
	return false;
}

typedef struct reqArraysStruct {
	std::string name;
	uint8_t minSize;
} reqArraysDetails;

bool Dispatcher::isValid(const json & inVal) {

	reqArraysDetails reqAs[] = {
			{"cb", 2},
			{"dest", 2},
			{"args", 0},
			{"", 0}
	};
	/*
    cordova.exec(
        successCallback,
        errorCallback,
        'com.cordova.Serial',
        'requestPermission',
        [{'opts': opts}]
    );
    */
	// assume a struct like
	/*
	{
		cb: [SUCCESSCB, ERRORCB],
		dest: [PLUGINNAME, METHOD]
		args: []
	}
	*/
	uint8_t i=0;
	while (reqAs[i].name.size()) {
		if (! checkForArray(reqAs[i].name, inVal, reqAs[i].minSize)) {
			return false;
		}
		i++;
	}



	return true;

}

bool Dispatcher::destinationExists(const json::array_t & dest) {
	if (! dest[0].is_string()) {
		CVERROR_OUTLN("dest[0] is not a string");
		return false;
	}


	if (! Plugin::Registry::getInstance()->has(dest[0])) {

		CVERROR_OUTLN("no plugin " << dest[0] << " found.");
		return false;
	}

	if (! dest[1].is_string()) {
		CVERROR_OUTLN("dest[1] is not a string");
		return false;
	}

	Plugin::Plugin * plg = Plugin::Registry::getInstance()->find(dest[0]);

	if (! plg ) {

		CVERROR_OUTLN("plugin " << dest[0] << " found but ret NULL???");
		return false;
	}

	if (! plg->hasMethod(dest[1])) {

		CVERROR_OUTLN("plugin " << dest[0]
								<< " doesn't have a method: "
								<< dest[1]);
		return false;
	}


	return true;

}

Dispatcher * Dispatcher::getInstance() {
	if (disp_singleton) {
		return disp_singleton;
	}

	disp_singleton = new Dispatcher();
	return disp_singleton;
}

} /* namespace Coraline */
