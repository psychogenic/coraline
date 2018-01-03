/*
 * main.c
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



#include "coraline/webview/webview.h"
#include "coraline/webview/webview_callback.h"
#include "coraline/webview/corview_plugins.h"
#include "coraline/corviewConfig.h"
#include "coraline/webview/corview_fileutil.h"
#include "coraline/corviewDefs.h"

#include <unistd.h>
#define STARTUP_LOOPS	3000

#if defined(WEBVIEW_GTK)
#define HAVE_PENDING_SYSUI_EVENTS()		gtk_events_pending()
#else
#define HAVE_PENDING_SYSUI_EVENTS()		false
#endif

#include <gtk/gtk.h>
#include <gtk/gtkwindow.h>


int main(int argc, char * argv[]) {

	char urlfile[1024] = {0,};
	const char * fname = rootFile(argc, argv);
	int loopCount = 0;

	strcpy(urlfile, "file://");
	strncat(urlfile, fname, 1000);

	struct webview myView = {

	.title = VIEWTITLE,
	.url = urlfile,
	.width = STARTWIDTH,
	.height = STARTHEIGHT,
	.resizable = VIEWRESIZABLE,
	.debug = VIEWDEBUG,
	.external_invoke_cb = dispatcher_webview_callback };


	// in our global context
	plugins_context_init(argc, argv, &myView);
	// init our (possibly custom) icon
	config_icon_init();
	// init our webview
	webview_init(&myView);

	// give a few spins of iteration to allow for processing
	while (loopCount++ < (STARTUP_LOOPS/100)) {
		webview_loop(&myView, 0);
	}

	// show splan
	startSplsh();

	loopCount = 0;
	plugins_register_all(&myView);
	// give a few spins of iteration to allow for processing
	while (loopCount++ < (STARTUP_LOOPS/100)) {
		webview_loop(&myView, 0);
	}

	// actually start the plugins
	plugins_start_all(&myView);
	loopCount = 0;
	// give the webview ample time to process the 
	// JS etc 
	while (loopCount++ < (STARTUP_LOOPS)) {
		webview_loop(&myView, 0);
	}

	// now call it "device ready"
	plugins_deviceready_signal();

	// and do our main loop
	do {
		loopCount = 0;
		plugins_update_all();
		// handle all events pending, now
		while (HAVE_PENDING_SYSUI_EVENTS() && loopCount++ < 1000) {

			plugins_update_all();
			webview_loop(&myView, 0); // non-blocking handle
			plugins_update_all();
		}
		webview_loop(&myView, 0); // non-blocking handle

		// all of this batch of events handled, blocking
		// so now we want to periodically update our plugins
		plugins_update_all();
		usleep(CORVIEW_MAINLOOP_SLEEPUS);
		// usleep(5000);

	} while (webview_loop(&myView, 0) == 0);

	plugins_shutdown_all();

	/* Destroy webview window, often exits the app */
	webview_exit(&myView);

	webview_terminate(&myView);

	return 0;
}
