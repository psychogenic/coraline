#
# Coraline shared resources
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

install(FILES shortcuts/coraline.desktop DESTINATION /usr/share/applications)

install(DIRECTORY content DESTINATION share/coraline)

install(DIRECTORY system DESTINATION share/coraline)
install(DIRECTORY plugins DESTINATION share/coraline)
install(DIRECTORY plugin-resources DESTINATION share/coraline)



