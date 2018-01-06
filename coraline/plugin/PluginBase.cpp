/*
 * PluginBase.cpp
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

#include "coraline/extlib/system.h"
#include "coraline/plugins/PluginBase.h"
#include "coraline/plugins/pluginDev.h"
#include "coraline/webview/Registry.h"
#include "coraline/webview/webview.h"
#include "coraline/corviewConfig.h"
#include "coraline/extlib/sha512.h"
#include "coraline/webview/corview_fileutil.h"
#include "coraline/Configuration.h"



#include "../include/coraline/coralineDirs.h"
#include "coraline/coralineLocations.h"

namespace Coraline {
namespace Plugin {



Base::Base(const Context & ctx): Coraline::Plugin::Plugin(),
		theView(ctx.view),
		reserved1(false),
		reserved2(0)
{

	CVDEBUG_OUTLN("Plugin c'tor called by driver v"
			<< (int)ctx.driver_version.maj
			<< '.'
			<< (int)ctx.driver_version.min
			<< '.'
			<< (int)ctx.driver_version.patch);



}

Base::~Base() {

}

AboutString Base::about() {
	AboutString about(fullName() + " plugin, part of the Coraline system by Pat Deegan, psychogenic.com");
	return about;

}

void Base::initAndRegister() {
	this->registerAllMethods();

	if (! hasMethod("aboutInfo")) {
		PLUGINREGMETH(aboutInfo);
	}
	Registry::getInstance()->add(this);


}

void Base::update() {
	if (updateActions.size())
	{
		UpdateActionQueue qCopy(updateActions);
		updateActions.clear();
		for (UpdateActionQueue::iterator iter = qCopy.begin();
				iter != qCopy.end();
				iter++)
		{
			(*iter)();
		}
	}
}
void Base::queueAction(UpdateAction act) {
	updateActions.push_back(act);
}
void Base::injectCode() {

	bool haveInjectedCode = false;
	ClientCodeFileName fname = this->clientSideSupportFile();
	if (fname.size()) {

		CVDEBUG_OUT("Have client-side support file--injecting " << fname);
		if (this->executeResourceJS(fname)) {
			CVDEBUG_OUTLN(" SUCCESS!");
			haveInjectedCode = true;
		} else {

			CVDEBUG_OUTLN(" FAIL!");
		}
	}

	ClientCode code = this->clientSideSupport();
	if (code.size()) {
		CVDEBUG_OUTLN("Have client-side support code--injecting...");
		executeJS(code);
		haveInjectedCode = true;
	}

	if (haveInjectedCode) {
		std::stringbuf buffer;             // empty buffer
		std::ostream ss(&buffer);      // associate stream buffer to stream

	    ss << "nativebridge." << this->shortName() << " = ";

	    if (this->merges().size()) {
	    	ss << this->merges();
	    } else if (this->clobbers().size()) {
	    	ss << this->clobbers();
	    } else {
	    	ss << " new " << this->shortName() << "();";
	    }

		executeJS(buffer.str());


	}


}
bool Base::hasMethod(const MethodName& name) {
	CVDEBUG_OUTLN("Checking for method " << name << " on plugin "
					<< this->fullName() );

	bool haveIt = (methodNamesToCallableMap.find(name)
						!= methodNamesToCallableMap.end());

	if (haveIt) {
		CVDEBUG_OUTLN("GOT IT");
	} else {
		CVDEBUG_OUTLN("not found");
	}

	return haveIt;
}
std::string Base::callbackTriggerString(const std::string & method,
		CallbackID cbid) {

	std::stringbuf buffer;             // empty buffer
	std::ostream ss (&buffer);      // associate stream buffer to stream

	ss << "nativebridge." << method << '(' << cbid << ",{})";

	return buffer.str();
}

std::string Base::callbackTriggerString(const std::string & method,
		CallbackID cbid, const json & retArg)
{
	std::string argsAsStr(retArg.dump());
	std::stringbuf buffer;             // empty buffer
	std::ostream ss (&buffer);      // associate stream buffer to stream

	ss << "nativebridge." << method << '(' << cbid << ',' << argsAsStr << ')';

	return buffer.str();
}

void Base::triggerCallback(CallbackID cbid) {

	if (!cbid) {
		CVDEBUG_OUTLN("No id set, skipping trigger");
		return;
	}

	std::string cmdStr(callbackTriggerString("triggerCallback", cbid));
	CVDEBUG_OUTLN("Triggering callback with eval:" << cmdStr);
	struct webview * wView = (struct webview *)theView;
	webview_eval(wView, cmdStr.c_str());
}
void Base::triggerCallbackNoRemove(CallbackID cbid) {

	if (!cbid) {
		CVDEBUG_OUTLN("No id set, skipping triggerNoRemove");
		return;
	}

	std::string cmdStr(callbackTriggerString("triggerCallbackNoRemove", cbid));
	CVDEBUG_OUTLN("Triggering callback with eval:" << cmdStr);
	struct webview * wView = (struct webview *)theView;
	webview_eval(wView, cmdStr.c_str());

}


void Base::triggerCallback(CallbackID cbid, const json & retArg) {
	std::string cmdStr(callbackTriggerString("triggerCallback", cbid, retArg));
	CVDEBUG_OUTLN("Triggering callback with eval:" << cmdStr);
	struct webview * wView = (struct webview *)theView;
	webview_eval(wView, cmdStr.c_str());
}
void Base::triggerCallbackNoRemove(CallbackID cbid, const json & retArg) {

	std::string cmdStr(callbackTriggerString("triggerCallbackNoRemove", cbid, retArg));
	CVDEBUG_OUTLN("Triggering callback with eval:" << cmdStr);

	struct webview * wView = (struct webview *)theView;
	webview_eval(wView, cmdStr.c_str());

}

void Base::executeJS(const std::string & evalThis) {

	if (! evalThis.size()) {
		return;
	}
#ifdef EXECJS_DEBUGOUT
	CVDEBUG_OUTLN("EXEC\n" << evalThis);
#endif


	struct webview * wView = (struct webview *)theView;
	webview_eval(wView, evalThis.c_str());

}


std::ifstream Base::resourceFile(const std::string & name) {
	std::ifstream inRes;

	std::string updirTerm(".." CORVIEW_PATH_SEP);
	std::string dirSep(CORVIEW_PATH_SEP);

	if (name.find(updirTerm) != std::string::npos) {
		CVDEBUG_OUTLN("resourceFile bad name");
		return inRes;
	}


	std::string baseDir(PLUGINS_RESOURCES_DIR);

	std::string fname(baseDir + dirSep + this->shortName() +  dirSep + name);

	inRes.open(fname);

	if (! inRes) {

		CVDEBUG_OUTLN("could not open " << fname);
	}

	CVDEBUG_OUTLN("Opened resource " << name << " at " << fname);
	return inRes;
}
std::string Base::resourceFileToString(const std::string & name) {
	std::string retStr;
	std::ifstream inRes = resourceFile(name);
	if (! inRes) {

		CVDEBUG_OUTLN("could not open " << name);
		return retStr;
	}

	std::stringstream buffer;
	buffer << inRes.rdbuf();

	retStr = buffer.str();

	return retStr;


}

bool Base::executeResourceJS(const std::string & name) {


	std::string jsToEval = resourceFileToString(name);
	if (! jsToEval.size()) {
		CVDEBUG_OUTLN("could load resource string " << name);
		return false;
	}


	executeJS(jsToEval);

	return true;


}


bool Base::aboutInfo(const StandardCallbackIDs & callbacks, const ArgsList & args) {

	json ho;
	ho["name"] = this->fullName();
	ho["about"] = this->about();
	ho["usage"] = this->usage();
	reportSuccess(callbacks, ho);

	return true;

}

bool Base::callMethod(const MethodName& name,
		const StandardCallbackIDs & callbacks,
		const ArgsList& args) {
	MethodsMap::iterator mIter = methodNamesToCallableMap.find(name);
	if (mIter == methodNamesToCallableMap.end()) {
		CVERROR_OUTLN("Calling method " << name << " but it's not here?");
		return false;
	}
	CVDEBUG_OUTLN("Calling method " << name);

	bool retVal = ((*mIter).second)(callbacks, args);

	CVDEBUG_OUTLN("Returned value : " << retVal);
	return retVal;
}

void Base::registerMethod(const MethodName& name,
		CallableMethod& meth) {

	CVDEBUG_OUTLN("Registering method: " << name
				<< " on plugin " << this->fullName() );

	methodNamesToCallableMap[name] = meth;
}

} /* namespace Plugin */
} /* namespace Coraline */
