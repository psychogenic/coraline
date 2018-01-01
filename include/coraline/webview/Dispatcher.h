/*
 * Dispatcher.h
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

#ifndef INCLUDES_DISPATCHER_H_
#define INCLUDES_DISPATCHER_H_
#include "../corviewTypes.h"

namespace Coraline {

class Dispatcher {
public:
	static Dispatcher * getInstance();
	// bool dispatch(const char * arg);
	bool dispatch(const std::string & str);

	~Dispatcher();

private:
	static Dispatcher * disp_singleton;
	Dispatcher();


	bool isValid(const json & inVal);
	bool checkForArray(const std::string & name, const json & inVal, uint8_t minSize=1);

	bool destinationExists(const json::array_t & dest);


};

} /* namespace Coraline */

#endif /* INCLUDES_DISPATCHER_H_ */
