/*
 * uiabout.c - GTK3 about dialog
 *
 * Written by
 *  Bas Wassink <b.wassink@ziggo.nl>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 */


#include "vice.h"

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "info.h"
#include "lib.h"
#include "version.h"
#ifdef USE_SVN_REVISION
#include "svnversion.h"
#endif

#include "uiabout.h"


/** \brief  List of current team members
 *
 * This list is allocated in create_current_team_list() and deallocated in
 * the "destroy" callback of the About dialog
 */
static char **authors;


/** \brief  Create list of current team members
 *
 * \return  heap-allocated list of strings
 */
static char **create_current_team_list(void)
{
    char **list;
    size_t i;

    /* get proper size of list (sizeof doesn't work here) */
    for (i = 0; core_team[i].name != NULL; i++) {
        /* NOP */
    }
    list = lib_malloc(sizeof *list * (i + 1));

    g_print("%s(): team members = %d\n", __func__, (int)i);

    for (i = 0; core_team[i].name != NULL; i++) {
        list[i] = core_team[i].name;
    }
    list[i] = NULL;
    return list;
}


#if 0
static char **create_copyright_list(void)
{
    char **list = lib_malloc(sizeof *list * 256);
    size_t i;

    while (core_team[i].name != NULL) {
        char *member = lib_malloc(256);
        snprintf(member, 256, "%s - %s", core_team[i].years, core_team[i].name);
        list[i++] = member;
    }
    /* TODO: add ex-team */
    list[i] = NULL;
    return list;
}
#endif

#if 0
static char **create_translators_list(void)
{
    char **list = lib_malloc(sizeof *list * 256);
    size_t i;

    while (trans_team[i].name != NULL) {
        char *member = lib_malloc(256);
        snprintf(member, 256, "%s - %s (%s)",
                trans_team[i].years,
                trans_team[i].name,
                trans_team[i].language);
        list[i++] = member;
    }
    list[i] = NULL;
    return list;
}
#endif


static void destroy_current_team_list(char **list)
{
    lib_free(list);
}


#if 0
static void destroy_copyright_list(char **list)
{
    size_t i = 0;

    while (list[i] != NULL) {
        lib_free(list[i]);
        i++;
    }
    lib_free(list);
}
#endif


/** \brief  Create VICE logo
 *
 * \return  GdkPixbuf instance
 */
static GdkPixbuf *get_vice_logo(void)
{
    GdkPixbuf *logo;
    GError *err = NULL;

    /* TODO:    use sysfile.c to locate logo file, probably needs adjusting
     *          archdep_default_sysfile_pathlist()
     */
    logo = gdk_pixbuf_new_from_file(
            "/usr/local/lib64/vice/doc/vice-logo-small.png", &err);
    if (logo == NULL || err != NULL) {
        g_print("%s(): %s\n", __func__, err->message);
    }
    return logo;
}



static void about_destroy_callback(GtkWidget *widget, gpointer user_data)
{
    destroy_current_team_list(authors);
    /* GdkPixbuf mentions setting refcount to 1, but it appears the about
     * dialog parent cleans it up somehow -- compyx */
#if 0
    g_object_unref(widget);
#endif
}


static void about_response_callback(GtkWidget *widget, gpointer user_data)
{
    gint response_id = GPOINTER_TO_INT(user_data);

    g_print("[debug] %s(): response id: %d\n", __func__, response_id);

    /* the GTK_RESPONSE_DELETE_EVENT is sent when the user clicks 'Close', but
     * also when the user clicks the 'X' */
    if (response_id == GTK_RESPONSE_DELETE_EVENT) {
        g_print("[debug %s(): CLOSE button clicked\n", __func__);
        gtk_widget_destroy(widget);
    }
}


/** \brief  Callback to show the 'About' dialog
 *
 * \param[in,out]   widget      widget triggering the event
 * \param[in]       user_data   data for the event (unused)
 */
void ui_about_dialog_callback(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *about = gtk_about_dialog_new();
    GdkPixbuf *logo = get_vice_logo();
    GtkWidget *window;

    /* get toplevel window, Gtk doesn't like dialogs without parents */
    window = gtk_widget_get_toplevel(widget);
    /* if no toplevel is found, the widget itself is returned, we don't want
     * that */
    if (gtk_widget_is_toplevel(window)) {
        gtk_window_set_transient_for(GTK_WINDOW(about), GTK_WINDOW(window));
    }

    authors = create_current_team_list();

    g_print("[debug] %s() called\n", __func__);


    gtk_window_set_title(GTK_WINDOW(about), "About VICE");

    /* set version string */
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about),
#ifdef USE_SVN_REVISION
            VERSION " r" VICE_SVN_REV_STRING " (Gkt3)"
#else
            VERSION " (Gtk3)"
#endif
            );

    /* set license text */
    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(about), info_license_text);
    /* set website link and title */
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about),
            "http://vice.pokefinder.org");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(about),
            "http://vice.pokefinder.org");
    /* set list of current team members */
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about), (const gchar **)authors);
    /* set copyright string */
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about),
            "Copyright 1996-2017 VICE TEAM");

    /* set logo. XXX: find proper logo */
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about), logo);


    g_signal_connect(about, "destroy", G_CALLBACK(about_destroy_callback),
            (gpointer)logo);

    g_signal_connect(about, "response", G_CALLBACK(about_response_callback),
            NULL);

    gtk_widget_show(about);
}