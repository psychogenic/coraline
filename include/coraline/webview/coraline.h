/*
 * coraline.h
 *
 *  Created on: Dec 10, 2017
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


#ifndef __RESOURCE_coraline_H__
#define __RESOURCE_coraline_H__

#include <gio/gio.h>


#ifdef __cplusplus
extern "C" {
#endif
extern GResource *coraline_get_resource (void);

void coraline_register_resource (void);
void coraline_unregister_resource (void);




#ifdef __cplusplus
}
#endif



#endif
