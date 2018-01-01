/*
 * PluginBase.h
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

#ifndef INCLUDES_PLUGINS_PLUGINBASE_H_
#define INCLUDES_PLUGINS_PLUGINBASE_H_
#include "Plugin.h"
#include "Context.h"
#include "dynPlugin.h"
#include "../extlib/system.h"

#define PLUGINSTRINGIFY(x) #x
#define PLUGINTOSTRING(x) PLUGINSTRINGIFY(x)
#define PLUGINREGMETH(name) { \
		 Coraline::Plugin::CallableMethod ho = [ this ](const Coraline::Plugin::StandardCallbackIDs & cb, const Coraline::Plugin::ArgsList & args) -> bool { \
	         return this->name(cb, args); }; \
	    registerMethod(PLUGINTOSTRING(name), ho); \
		}






// PLUGIN_CHECKARGSCOUNT (FUNCTION_NAMESTR, MIN_NUM_ARGS, ARGS_OBJ)
#define PLUGIN_CHECKARGSCOUNT(fname, minNum, args) \
    CVDEBUG_OUT(fname << "..."); \
    if ( (args.size() < minNum))  { CVDEBUG_OUTLN("Not enough args"); return false; }

// PLUGIN_CHECKARGTYPE ( ARGS_INDEX, ARG_TYPECHECK_METHOD, ARGS_OBJ)
#define PLUGIN_CHECKARGTYPE(argIdx, argType, args) \
    if (! args[argIdx].argType()) { CVDEBUG_OUTLN("Arg " << argIdx << " wrong type"); return false; }

// PLUGIN_OPTARG ( ARGS_INDEX, ARG_TYPECHECK_METHOD, ARGS_OBJ, ASSIGN_TO_VARIABLE)
#define PLUGIN_OPTARG(idx, argType, args, assignTo) \
    if (args.size() > (idx) && args[idx].argType()) { \
        assignTo = args[idx]; \
    }

namespace Coraline {
namespace Plugin {


typedef std::map<MethodName, CallableMethod> MethodsMap;
typedef std::function<void(void)> UpdateAction;
typedef std::vector<UpdateAction> UpdateActionQueue;

class Base : public Coraline::Plugin::Plugin {
public:
	Base(const Context & ctx);
	virtual ~Base();

	// do override these
	virtual Coraline::Version version() = 0;
    virtual const PluginName fullName()  = 0;
    virtual const PluginName shortName() = 0;


    virtual const std::string merges() { return "";}
    virtual const std::string clobbers() { return "";}

	// lifecycle default implementations: do nothing

    /*
     * Supporting code -- on plugin instantiation you may provide:
     *  * a JS resource file to load
     *  * some JS code, as a string
     *
     * and these will loaded file first, if it exists, then
     * JS code string (if it is non-empty).
     *
     * *IF* any such code is loaded, it is assumed that we can instantiate
     * a variable based on shortName(), meaning if shortName() returns
     * "Device", we will be running
     * 	 somevar = new Device();
     * after the supporting code loads.
     */
    // clientSideSupport actual JS code to
	virtual ClientCode clientSideSupport() { return "";}

	virtual ClientCodeFileName clientSideSupportFile() { return "";}



	virtual AboutString usage() { return ""; }
	virtual AboutString about();

	virtual void startUp() {} // basic startup
	virtual void update(); // periodic update
	virtual void shutdown() {} // prior to quit



    virtual bool hasMethod(const MethodName & name);
    virtual bool callMethod(const MethodName & name,
    		const StandardCallbackIDs & callbacks,
    		const ArgsList & args);

    virtual bool dynamicallyLoaded() {
    	return dynSettings.valid;
    }

    void setDynamicallyLoaded(const LoadablePlugin & s) {
    	dynSettings = s;
    }
    const LoadablePlugin & dynamicSettings() {
    	return dynSettings;
    }


	virtual void initAndRegister();
	virtual void injectCode();

protected:
	// must register your accessible methods
	virtual void registerAllMethods() = 0;

	/* utilities to trigger callbacks within plug-methods */
	inline bool reportSuccess(const StandardCallbackIDs & callbacks, const json & retArg) {
		triggerCallback(callbacks.success, retArg);
		return true;
	}
	inline bool reportSuccess(const StandardCallbackIDs & callbacks) {
		json succ = true;
		return reportSuccess(callbacks, succ);
	}
	inline bool reportError(const StandardCallbackIDs & callbacks, const json & retArg) {
			triggerCallback(callbacks.error, retArg);
			return false;
	}
	inline bool reportError(const StandardCallbackIDs & callbacks) {
				triggerCallback(callbacks.error);
				return false;
	}
	inline bool reportError(const StandardCallbackIDs & callbacks, const std::string & errStr) {
		json err = errStr;
		return reportError(callbacks, err);
	}

	void triggerCallback(CallbackID cbid);
	void triggerCallbackNoRemove(CallbackID cbid);


	void triggerCallback(CallbackID cbid, const json & retArg);
	void triggerCallbackNoRemove(CallbackID cbid, const json & retArg);

	/* registerMethod: used in registerAllMethods to expose API */
    void registerMethod(const MethodName & name, CallableMethod & meth);


	void executeJS(const std::string & evalThis);
	bool executeResourceJS(const std::string & name);

	std::ifstream resourceFile(const std::string & name);
	std::string resourceFileToString(const std::string & name);
	void queueAction(UpdateAction act);

private:

	bool aboutInfo(const StandardCallbackIDs & callbacks, const ArgsList & args);

    std::string callbackTriggerString(const std::string & method,
    		CallbackID cbid);

    std::string callbackTriggerString(const std::string & method,
    		CallbackID cbid, const json & retArg);
    // struct webview * theView;
    void* theView; // void* to shield derivatives from including the whole webview
    MethodsMap methodNamesToCallableMap;

    LoadablePlugin dynSettings;
    UpdateActionQueue updateActions;

    bool reserved1;
    float reserved2;
    uint8_t reserved[24];

};

} /* namespace Plugin */
} /* namespace Coraline */

#endif /* INCLUDES_PLUGINS_PLUGINBASE_H_ */
