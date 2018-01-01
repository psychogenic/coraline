#
#  GattLib++ - GATT Library C++ Wrapper
#
#  Copyright (C) 2017  Pat Deegan, https://psychogenic.com
#
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig REQUIRED)

 
include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

set(coraline_SRCS main.c
	plugins_setup.cpp
	Configuration.cpp
	builtin/NativeBridge.cpp
	builtin/TestPlugin.cpp
	plugin/Registry.cpp
	plugin/Dispatcher.cpp
	plugin/PluginBase.cpp
	plugin/Plugin.cpp
	plugin/Context.cpp
	corview_fileutil.cpp
	webview/coraline.c 
	webview/webview_callback.cpp
)

set(coraline_LIBS ${GTK_LDFLAGS} ${WEBKITGTK_LDFLAGS} ${GLIB_LDFLAGS})

add_executable(coraline ${coraline_SRCS})
target_link_libraries(coraline -ldl ${coraline_LIBS})

install(TARGETS coraline RUNTIME DESTINATION bin)
