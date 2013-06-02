/*
 *   Copyright 2009 Ian McIntosh <ian@openanswers.org>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <math.h>
#include <vector>

using namespace std;    // saves us typing std:: before vector

#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/glut.h>

#include <gtkmm.h>
#include <gtk/gtkgl.h>
#include <gtkmm/gl/init.h>
#include <gtkmm/gl/widget.h>

#include <lo/lo.h>

// TODO add Leap includes

#include "message-bus.h"

#include "utils.h"

#define STANDARD_WIDGET_SPACING (6)

#define APPLICATION_NAME ("Hand Tracker")
#define APPLICATION_VERSION ("0.01")
#define APPLICATION_COPYRIGHT ("Copyright 2013 Ian McIntosh")
#define UNIQUE_APP_GUID ("org.openanswers.hand-tracker")

#define RC_FILE_PATH ("hand-tracker.rc")
#define XML_PATH ("openni-config.xml")

#define PNG_ICON_FILE_PATH ("hand-tracker-status-icon.png")
#define SVG_ICON_FILE_PATH ("hand-icon.svg")

#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480

extern MessageBus* g_message_bus;
extern bool g_time_to_quit;
