/*
 * Context.h
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

#ifndef INCLUDES_PLUGINS_CONTEXT_H_
#define INCLUDES_PLUGINS_CONTEXT_H_


#include "../corviewTypes.h"

namespace Coraline {
namespace Plugin {


class ContextFactory {

public:
	static const Context & get();
	static void setStartupArgs(int argc, char* argv[]);
	static void setView(void * w);
	static void setIcon(const char * iconFilePath);

	static void refreshConfig();


private:
	static Context ctx_singleton;

	ContextFactory();
};



} /* namespace Plugin */
} /* namespace Coraline */





#endif /* INCLUDES_PLUGINS_CONTEXT_H_ */
