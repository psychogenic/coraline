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
#define STARTUP_LOOPS	5000

#if defined(WEBVIEW_GTK)
#define HAVE_PENDING_SYSUI_EVENTS()		gtk_events_pending()
#else
#define HAVE_PENDING_SYSUI_EVENTS()		false
#endif

#include <gtk/gtk.h>
#include <gtk/gtkwindow.h>

static void processPendingEvents(struct webview * wv, int maxCount) {
	int loopCount = 0;
	int noEventsCount = 0;
	int maxNoEventsCount = maxCount / 10;
	webview_loop(wv, 0);
	// spare 20 ms to let the system queue anything it needs to
	usleep(20000);

	while (noEventsCount < maxNoEventsCount && (loopCount++ < maxCount)) {
		if (HAVE_PENDING_SYSUI_EVENTS()) {
			webview_loop(wv, 1);
		} else {
			noEventsCount++;
			usleep(15);
		}
	}

}

int main(int argc, char * argv[]) {

	char urlfile[1024] = {0,};
	const char * fname = rootFile(argc, argv);
	int loopCount = 0;
	int noEventsCount = 0;

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

	// show splan
	startSplsh();

	processPendingEvents(&myView, STARTUP_LOOPS/100);


	plugins_register_all(&myView);

	processPendingEvents(&myView, STARTUP_LOOPS/100);
/*
	webview_loop(&myView, 0);
	usleep(2000);
	// give a few spins of iteration to allow for processing
	while (HAVE_PENDING_SYSUI_EVENTS() && (loopCount++ < (STARTUP_LOOPS/100))) {
		webview_loop(&myView, 0);
	}

	*/
	// actually start the plugins
	plugins_start_all(&myView);

	// give the webview ample time to process the 
	// JS etc 
	processPendingEvents(&myView, STARTUP_LOOPS);

	// now call it "device ready"
	plugins_deviceready_signal();

	// and do our main loop
	do {
		loopCount = 0;
		noEventsCount = 0;
		// handle all events pending, now
		while (noEventsCount < 10 && loopCount++ < 1000) {

			if (HAVE_PENDING_SYSUI_EVENTS()) {
				webview_loop(&myView, 1);
			} else {
				noEventsCount ++;
				webview_loop(&myView, 0); 
			}

			plugins_update_all();
		}
		webview_loop(&myView, 0); // non-blocking handle

		// all of this batch of events handled, blocking
		// so now we want to periodically update our plugins
		plugins_update_all();
		usleep(CORVIEW_MAINLOOP_SLEEPUS);

	} while (webview_loop(&myView, 0) == 0);

	plugins_shutdown_all();

	/* Destroy webview window, often exits the app */
	webview_exit(&myView);

	webview_terminate(&myView);

	return 0;
}
