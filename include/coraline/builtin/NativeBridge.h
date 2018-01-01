/*
 * NativeBridge.h
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

#ifndef INCLUDES_NATIVEBRIDGE_H_
#define INCLUDES_NATIVEBRIDGE_H_

#include "../plugins/pluginDev.h"


namespace Coraline {
namespace Plugin {

class NativeBridge : public Base{
public:
	NativeBridge(const Context & ctx);
	virtual ~NativeBridge();

	Coraline::Version version() { return Coraline::Version();}
    virtual const PluginName fullName() { return CORVIEW_NATIVEBRIDGE_PLUGINNAME; }
    virtual const PluginName shortName() { return "NativeBridge";}

	virtual ClientCode clientSideSupport();
	virtual void injectCode();



	virtual AboutString about();
	virtual AboutString usage();

	void notifyDeviceReady();

protected:
	virtual void registerAllMethods();

private:
	// bool test1(const StandardCallbackIDs & callbacks, const ArgsList & args);
	bool pluginAvailable(const StandardCallbackIDs & callbacks, const ArgsList & args);


};

} /* namespace Plugin */
} /* namespace Coraline */

#endif /* INCLUDES_NATIVEBRIDGE_H_ */
