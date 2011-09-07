/* vim: set ts=4 sw=4 tw=0: */
/*  aprsmap APRS map display software
	(C) 2010-2011 Gordon JC Pearce MM0YEQ and others
	
    mapgui.c
    Creates the main app window, and any necessary callbacks
	
	This file is part of aprsmap, a simple APRS map viewer using a modular
	and lightweight design
	
	aprsmap is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	any later version.

	aprsmap is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with aprsmap.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtk/gtk.h>
#include <mapgui.h>

void mainwindow() {
    // load the glade file, display main window
    GError *error = NULL;
    GtkWidget *widget;
  
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "aprsmap.ui", &error);
    if (error)
        g_error ("ERROR: %s\n", error->message);

    widget = GTK_WIDGET(gtk_builder_get_object(builder, "mainwindow"));
    gtk_widget_show_all (widget);
}
