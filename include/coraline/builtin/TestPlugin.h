/*
 * TestPlugin.h
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

#ifndef INCLUDES_TESTPLUGIN_H_
#define INCLUDES_TESTPLUGIN_H_
#include "../plugins/pluginDev.h"

namespace Coraline {
namespace Plugin {

class TestPlugin : public Coraline::Plugin::Base {
public:
	TestPlugin(const Context & ctx);
	virtual ~TestPlugin();
	Coraline::Version version();
    virtual const PluginName fullName() { return "com.psychogenic.Test"; }
    virtual const PluginName shortName() { return "Test";}


	virtual AboutString about();
	virtual AboutString usage();

protected:
	virtual void registerAllMethods();

private:
	bool willSucceed(const StandardCallbackIDs & callbacks, const ArgsList & args);
	bool bornToFail(const StandardCallbackIDs & callbacks, const ArgsList & args);


};

} /* namespace Plugin */
} /* namespace Coraline */

#endif /* INCLUDES_TESTPLUGIN_H_ */
