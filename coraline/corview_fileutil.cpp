/*
 * corview_fileutil.cpp
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


#include "coraline/webview/corview_fileutil.h"

#include "coraline/corviewConfig.h"
#include "coraline/webview/corview_fileutil.hpp"
#include "coraline/corviewDebug.h"
#include "coraline/Configuration.h"

// generated:
#include "../include/coraline/coralineDirs.h"
#include "coraline/coralineLocations.h"
#include "coraline/webview/coraline.h"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
// #include <libgen.h> -- dirname duh


#include <gtk/gtk.h>
#include <gtk/gtkwindow.h>
#include <gdk/gdkpixbuf.h>

#include <sys/stat.h>


bool fileExists(const std::string & fullpath) {
	return fileExists(fullpath.c_str());
}

int fileExists(const char * fullpath) {

	struct stat buffer;

	return (stat(fullpath, &buffer) == 0) ? 1 : 0;

}



static GtkWidget * dwinForImg(GtkWidget * image) {

	GtkWidget * dialog_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	gtk_window_set_position(GTK_WINDOW(dialog_window),GTK_WIN_POS_CENTER_ALWAYS);

	gtk_window_set_type_hint(GTK_WINDOW(dialog_window), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	gtk_window_set_opacity(GTK_WINDOW(dialog_window), 0.5);
	// gtk_window_set_opacity(GTK_WINDOW(image), 0.5);


	gtk_container_add(GTK_CONTAINER(dialog_window), image);

	return dialog_window;

}

gboolean close_screen_simple(gpointer data)
{
  gtk_widget_destroy((GtkWidget*)data);

  return (0);
}



static gboolean showUserSplashScreen(const std::string & imgPath) {

	GtkWidget *dialog_window, *image;

	image = gtk_image_new_from_file(imgPath.c_str());

	CVDEBUG_OUT("gots me image from file " << imgPath);

	dialog_window = dwinForImg(image);

	gtk_widget_show_all(dialog_window);

	g_timeout_add(1500, close_screen_simple, dialog_window);


	return 0;


}


gboolean close_screen_showsplash(gpointer data)
{
	gtk_widget_destroy((GtkWidget*)data);
	Coraline::Configuration * config = Coraline::Configuration::getInstance();

	CVDEBUG_OUT("showsplash close screen 1...");
	if (config->hadConfigFile() && config->splash().size()) {
		std::string fullPath(config->contentDir() + CORVIEW_PATH_SEP );
		fullPath += config->splash();


		CVDEBUG_OUT("Checking for " << fullPath);

		if (fileExists(fullPath))
		{

			showUserSplashScreen(fullPath);
		}
	}

	return (0);
}


void startSplsh() {

	GtkWidget *dialog_window, *image;
	Coraline::Configuration* config = Coraline::Configuration::getInstance();


	coraline_register_resource();
	GResource * res = coraline_get_resource();



	image = gtk_image_new_from_resource(
			"/com/psychogenic/coraline/icon/splash.png");

	dialog_window = dwinForImg(image);

	gtk_widget_show_all(dialog_window);

	if (config->hadConfigFile()
			&& config->splash().size()) {

		CVDEBUG_OUT("Have a custom splash...");
		g_timeout_add(1200, close_screen_showsplash, dialog_window);

	} else {
		CVDEBUG_OUT("No custom splash...");
		g_timeout_add(2000, close_screen_simple, dialog_window);
	}

}



const char * rootFile(int argc, char * argv[]) {
	static char fname[1024] = {0,};
	static bool is_ready = false;
	bool need_basedir_prefix = false;
	std::string bdir = CONTENT_INSTALL_DIR;
	std::string topFile;

	CVDEBUG_OUT("figuring out rootFile...");
	if (is_ready) {
		return fname;
	}


#ifdef TOPFILE
	strncpy(fname, TOPFILE, 1000);
#else

	is_ready = true;

	if (argc > 1 && argv[1]) {
		CVDEBUG_OUT("have an arg...");

		topFile = argv[1];
		if (argv[1][0] != '/') {
			CVDEBUG_OUT("need content dir");
			need_basedir_prefix = true;
		}
	} else {

		// no argument...
		// attempt to auto-select based on calling name
		std::string defTopFile(CORVIEW_DEFAULT_TOPFILE);
		if (argv[0]) {
			topFile = argv[0];

			// get rid of any prefix /path/to/bla/NAME and keep only name
			std::string::size_type lastSlash = topFile.find_last_of(CORVIEW_PATH_SEP);
			if (lastSlash != std::string::npos) {
				// have some slashes in there
				topFile = topFile.substr(lastSlash + 1, topFile.size() - lastSlash);
			}

			topFile += CORVIEW_PATH_SEP + defTopFile;
		}

		if (! (topFile.size() && fileExists(bdir + CORVIEW_PATH_SEP + topFile))) {
			CVDEBUG_OUT("using default topfile");
			topFile = DEFAULT_TOPFILE;
		}

		if (topFile[0] != '/') {
			need_basedir_prefix = true;
		}

	}

	if (need_basedir_prefix) {

		if (bdir.size()){
			strncat(fname, bdir.c_str(), 500);
			strncat(fname, CORVIEW_PATH_SEP, 1);
		}

		CVDEBUG_OUTLN("Base content dir is:" << bdir);

	}


	strncat(fname, topFile.c_str(), 500);

#endif

	CVDEBUG_OUTLN("returning fname:" << fname);
	return fname;

}



/*function... might want it in some class?*/
int getdir (std::string dir, FilesInDirList &files, std::string prefix, int maxnum)
{
    DIR *dp;
    static int numRead = 0;
    struct dirent *dirp;
    CVDEBUG_OUTLN("Scanning dir " << dir);

    if (maxnum < 1) {
    	numRead = 0;
    	maxnum = 200;
    }

    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cerr << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while (((dirp = readdir(dp)) != NULL) && (numRead++ < maxnum)) {
    	std::string relname(dirp->d_name);


    	if (relname.size() && relname.at(0) != '.' ) {
    		std::string fname(prefix);
    		if (prefix.size())
    		{
    			fname += CORVIEW_PATH_SEP;

    		}

    		fname += relname;

        	if (dirp->d_type == DT_DIR) {
        		getdir(dir + CORVIEW_PATH_SEP + relname, files, fname, (maxnum - numRead));
        	} else if (dirp->d_type == DT_REG) {
                files.push_back(fname);
        	}

    	}
    }
    closedir(dp);
    return 0;
}

std::string basedir(std::string fullpath) {

	char cwd[1024];
	std::string::size_type lastSlashPos;
	std::string bdir;
	std::string prefix;

	if (fullpath.size() < 2) {
		return bdir;
	}
	if (fullpath.at(0) == '.') {
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			prefix = std::string(cwd) + CORVIEW_PATH_SEP;
		}
	}

	lastSlashPos = fullpath.find_last_of(CORVIEW_PATH_SEP);
	if (lastSlashPos != std::string::npos && lastSlashPos) {

		bdir = prefix + fullpath.substr(0, lastSlashPos);

	} else {
		bdir = prefix;

	}

	return bdir;

}

