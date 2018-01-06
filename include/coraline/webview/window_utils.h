/*
 * window_utils.h
 *
 *  Created on: Jan 6, 2018
 *      Author: malcalypse
 */

#ifndef CORALINE_INCLUDE_CORALINE_WEBVIEW_WINDOW_UTILS_H_
#define CORALINE_INCLUDE_CORALINE_WEBVIEW_WINDOW_UTILS_H_

#include "../extlib/system.h"

#define CORALINE_IMAGEWINDOW_NOTIMEOUT		0

namespace Coraline {
namespace UI {

typedef GtkWidget DialogWindow;
typedef GtkWidget ImageWidget;


DialogWindow * windowForImage(ImageWidget * image, double opacity=1.0);
DialogWindow * windowForImageFile(const std::string & imgPath, double opacity=1.0);
DialogWindow * showWindow(DialogWindow * window,
		unsigned int timeout=CORALINE_IMAGEWINDOW_NOTIMEOUT);
DialogWindow * showImageWindowForFile(const std::string & imgPath,
		unsigned int timeout=CORALINE_IMAGEWINDOW_NOTIMEOUT);


void hideWindow(DialogWindow *);

void destroyWindow(DialogWindow * w);

}
}



#endif /* CORALINE_INCLUDE_CORALINE_WEBVIEW_WINDOW_UTILS_H_ */
