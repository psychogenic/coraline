/*
 * corview_plugins.h
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

#ifndef INCLUDES_WEBVIEW_CORVIEW_PLUGINS_H_
#define INCLUDES_WEBVIEW_CORVIEW_PLUGINS_H_


#include "webview.h"

#ifdef __cplusplus
extern "C" {
#endif

void plugins_context_init(int argc, char* argv[], struct webview *w);

void plugins_register_all(struct webview *w);
void plugins_start_all(struct webview *w);
void plugins_deviceready_signal();

void plugins_update_all();

void plugins_shutdown_all();



#ifdef __cplusplus
}
#endif






#endif /* INCLUDES_WEBVIEW_CORVIEW_PLUGINS_H_ */
