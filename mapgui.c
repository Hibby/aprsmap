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

#include "aprsmap.h"
#include "mapgui.h"


static GtkWidget *prefs_window;
static APRSMap_Settings *t_prefs;

void on_close(GtkWidget *widget, gpointer user_data) {
	printf("on_close()\n");
    gtk_widget_destroy(widget);
    gtk_main_quit();
}

G_MODULE_EXPORT void on_menuitem_prefs_activate() {
	char buf[20];
	t_prefs = g_slice_copy(sizeof(APRSMap_Settings), conf);
	
	printf("t_prefs->lon = %f\n", t_prefs->lon);
	
	sprintf(buf,"%f", t_prefs->lat);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "lat_entry")), buf);
	sprintf(buf,"%f", t_prefs->lon);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "lon_entry")), buf);
	sprintf(buf,"%d", t_prefs->zoom);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "zoom_entry")), buf);
	
	gtk_widget_show(prefs_window);
	
	//

}

G_MODULE_EXPORT void on_sethome_clicked() {
	gfloat lat, lon;
	gint zoom;
	char buf[20];
	
	g_object_get(map, "latitude", &lat, "longitude", &lon, "zoom", &zoom, NULL);
	
	sprintf(buf,"%f", lat);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "lat_entry")), buf);
	sprintf(buf,"%f", lon);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "lon_entry")), buf);
	sprintf(buf,"%d", zoom);
	gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(builder, "zoom_entry")), buf);
}

G_MODULE_EXPORT void on_prefs_cancel_clicked() {
	printf("cancelled\n");
	g_slice_free(APRSMap_Settings, t_prefs);
	gtk_widget_hide(prefs_window);	
}

G_MODULE_EXPORT void on_prefs_ok_clicked() {
	printf("ok\n");
	const gchar *buf;
	
	
	GKeyFile *keyfile;
    GError *error = NULL;
    gsize *length;
    guint group, key;
    	
	APRSMap_Settings *t;
	
	t = conf;
	conf = t_prefs;
	
	buf = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "lat_entry")));
	conf->lat = g_strtod(buf, NULL);
	
	buf = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "lon_entry")));
	conf->lon = g_strtod(buf, NULL);

	buf = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "zoom_entry")));
	conf->zoom = atoi(buf);

	g_slice_free(APRSMap_Settings, t);
	  keyfile = g_key_file_new ();
printf("doing key file thing\n");
    if(!g_key_file_load_from_file(keyfile,
                                  "settings.conf",
                                  G_KEY_FILE_KEEP_COMMENTS | 
                                  G_KEY_FILE_KEEP_TRANSLATIONS,
                                  &error))
    {
		printf("g_debug\n");
        g_debug("%s", error->message);
    }
    else
    {	
    printf("setting values\n");
	g_key_file_set_double(keyfile, "Home", "lat", conf->lat);
	g_key_file_set_double(keyfile, "Home", "lon", conf->lon);
	g_key_file_set_integer(keyfile, "Home", "zoom", conf->zoom);

	}
	
	gchar *f;
	
	printf("about to do a keyfile");
	f = g_key_file_to_data(keyfile, length, &error);
	printf("(hello)\n");
	printf("%s\n",f);
	printf("about to free\n");
	g_free(f);
	
	gtk_widget_hide(prefs_window);
	printf("%f\n", conf->lat);
}

void set_map_home(APRSMap_Settings *conf) {
	osm_gps_map_set_center_and_zoom(map, conf->lat, conf->lon, conf->zoom);
}

void mainwindow() {
    // load the glade file, display main window
    GError *error = NULL;
    GtkWidget *widget;
  
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "aprsmap.ui", &error);
    if (error)
        g_error ("ERROR: %s\n", error->message);

	gtk_builder_connect_signals(builder, NULL);
    map = g_object_new (OSM_TYPE_GPS_MAP,
        //"map-source",opt_map_provider,
        //"tile-cache",cachedir,
        //"tile-cache-base", cachebasedir,
        //"proxy-uri",g_getenv("http_proxy"),
        NULL);

    gtk_box_pack_start (
        GTK_BOX(gtk_builder_get_object(builder, "map_box")),
        GTK_WIDGET(map), TRUE, TRUE, 0);
  

    widget = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
   	prefs_window = GTK_WIDGET(gtk_builder_get_object(builder, "prefs_window"));


    gtk_widget_show_all (widget);

}
