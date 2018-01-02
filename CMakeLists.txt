#
#  Coraline -- Cordova/Ionic app driver for Linux
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


project(coraline)





find_package(PkgConfig REQUIRED)

pkg_search_module(GTK REQUIRED gtk+-3.0)
pkg_search_module(WEBKITGTK REQUIRED webkitgtk-3.0)
#pkg_search_module(GLIB REQUIRED glib-2.0)

#why is this list/string crap so complicate?  ugh, cmake.
set(GEN_FLAGS -Wall -DWEBVIEW_GTK=1)

set(ALLMYFLAGGIES ${CMAKE_C_FLAGS} ${GEN_FLAGS} ${GTK_CFLAGS} ${WEBKITGTK_CFLAGS})
string(REPLACE ";" " " ALLMYFLAGGIESSTR "${ALLMYFLAGGIES}")

set(CMAKE_C_FLAGS ${ALLMYFLAGGIESSTR})

set(CMAKE_CXX_FLAGS ${ALLMYFLAGGIESSTR})



if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Install directory (default: /usr/local)" FORCE)
endif()




include_directories(include)


#
# Packaging
#
set(CPACK_SET_DESTDIR true)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr/local" CACHE STRING "Install directory (default: /usr/local).")
set(CPACK_INSTALL_PREFIX ${CPACK_PACKAGE_INSTALL_DIRECTORY} CACHE STRING "Install directory (default: /usr/local).")
set(CPACK_PACKAGING_PREFIX ${CPACK_PACKAGE_INSTALL_DIRECTORY} CACHE STRING "Install directory (default: /usr/local).")



if (ENV{TRAVIS_TAG})
  set(CPACK_PACKAGE_VERSION $ENV{TRAVIS_TAG})
else()
  set(CPACK_PACKAGE_VERSION 1.0.10)
endif()
set(CPACK_PACKAGE_CONTACT "Pat Deegan, https://psychogenic.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Coraline, cordova-like native apps for Linux")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_PACKAGE_ARCHITECTURE}")
set(CPACK_PACKAGE_VENDOR "psychogenic.com")




add_subdirectory(coraline)
add_subdirectory(shared)
add_subdirectory(plugins)


# Generate pkg-config file
configure_file(coraline/coraline.pc.in ${PROJECT_BINARY_DIR}/coraline.pc @ONLY)
configure_file(include/coraline/coralineDirs.h.in include/coraline/coralineDirs.h @ONLY)

# Generate pkg-config file before building the examples
#configure_file(gattlibpp/gattlibpp.pc.in ${PROJECT_BINARY_DIR}/gattlibpp.pc @ONLY)

# Add the build directory to PKG_CONFIG_PATH
set(ENV{PKG_CONFIG_PATH} "${PROJECT_BINARY_DIR}:$ENV{PKG_CONFIG_PATH}")

# Examples
#add_subdirectory(examples/ble_scan)
#add_subdirectory(examples/read_write)


#
# Debian, RPM and ZIP packages
#
if (CMAKE_SYSROOT)
  # CPack does like RPM package in a cross-toolchain context
  set(CPACK_GENERATOR "DEB;ZIP")
else()
  set(CPACK_GENERATOR "DEB;RPM;ZIP")
endif()

if (NOT CMAKE_SYSROOT)
# Detect platform architecture to use it for the Debian package
  execute_process(COMMAND dpkg --print-architecture OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE OUTPUT_QUIET)
endif()

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.26), libgtk-3-0  (>= 3.22.25), libwebkitgtk-3.0-0 (>= 2.4.11), libglib2.0-0 (>= 2.54.1)")
#
# List of file to install
#

#install(FILES include/Coraline.h DESTINATION include)
#install(FILES ${PROJECT_BINARY_DIR}/gattlibpp.pc DESTINATION lib/pkgconfig)
install(FILES ${PROJECT_BINARY_DIR}/coraline.pc DESTINATION lib/pkgconfig)

include(CPack)
