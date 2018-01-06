/*
 * window_utils.cpp
 *
 *  Created on: Jan 6, 2018
 *      Author: malcalypse
 */

#include "coraline/webview/window_utils.h"
#include "coraline/corviewDebug.h"

namespace Coraline {
namespace UI {


DialogWindow * windowForImageFile(const std::string & imgPath, double opacity) {

	GtkWidget *dialog_window, *image;

	image = gtk_image_new_from_file(imgPath.c_str());

	if (image) {

		CVDEBUG_OUT("gots me image from file " << imgPath);

		dialog_window = windowForImage(image);
	} else {
		CVERROR_OUTLN("image file not converted to img widget for " << imgPath);
		return NULL;
	}

	return dialog_window;
}

DialogWindow * windowForImage(ImageWidget * image, double opacity) {

	GtkWidget * dialog_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(dialog_window),GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_type_hint(GTK_WINDOW(dialog_window), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);

	// gtk_window_set_opacity -- deprecated, not sure what to use...
	gtk_window_set_opacity(GTK_WINDOW(dialog_window), opacity);

	gtk_container_add(GTK_CONTAINER(dialog_window), image);

	return dialog_window;

}

static gboolean close_window_simple(gpointer data)
{
	destroyWindow((DialogWindow*)data);

  return (0);
}




DialogWindow * showImageWindowForFile(const std::string & imgPath, unsigned int timeout) {

	GtkWidget *	dialog_window = windowForImageFile(imgPath);

	if (! dialog_window) {
		return NULL;
	}

	return showWindow(dialog_window, timeout);

}

DialogWindow * showWindow(DialogWindow * window, unsigned int timeout) {
	gtk_widget_show_all(window);

	if (timeout != CORALINE_IMAGEWINDOW_NOTIMEOUT) {

		g_timeout_add(timeout, close_window_simple, window);
	}

	return window;

}


void hideWindow(DialogWindow * w) {
	gtk_widget_hide(w);
}

void destroyWindow(DialogWindow * w) {
	 gtk_widget_destroy((GtkWidget*)w);
}


} /* namespace UI */
} /* namespace Coraline */


