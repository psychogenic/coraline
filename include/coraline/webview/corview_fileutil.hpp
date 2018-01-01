/*
 * corview_fileutil.hpp
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

#ifndef INCLUDES_WEBVIEW_CORVIEW_FILEUTIL_HPP_
#define INCLUDES_WEBVIEW_CORVIEW_FILEUTIL_HPP_
#include "../corviewTypes.h"
#include "corview_fileutil.h"

#include <vector>
#include <string>
#include <iostream>

typedef std::vector<std::string> FilesInDirList;

int getdir (std::string dir, FilesInDirList &files, std::string prefix="", int maxnum=-1);

bool fileExists(const std::string & fullpath);

std::string basedir(std::string fullpath);


#endif /* INCLUDES_WEBVIEW_CORVIEW_FILEUTIL_HPP_ */
