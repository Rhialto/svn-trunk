/** \file   sidenginemodelwidget.c
 * \brief   Widget to select SID engine and model
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

/*
 * $VICERES SidModel    all
 *  (all = if a SidCart is installed)
 * $VICERES SidEngine   all
 *  (all = if a SidCart is installed)
 */

/*
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
 *
 */

#include "vice.h"

#include <gtk/gtk.h>

#include "vice_gtk3.h"
#include "resources.h"
#include "sid.h"

#include "sidenginemodelwidget.h"



static void on_radio_toggled(GtkWidget *radio, gpointer data)
{
    unsigned int engine;
    unsigned int model;

    /* bit 15-8: engine */
    engine = (GPOINTER_TO_UINT(data) >> 8) & 0xff;
    /* bit 7-0: model */
    model = GPOINTER_TO_INT(data) & 0xff;

    debug_gtk3("called with engine %02x, model %02x.", engine, model);

    sid_set_engine_model(engine, model);


}



GtkWidget *sid_engine_model_widget_create(void)
{
    GtkWidget *grid;
    GtkRadioButton *last = NULL;
    GSList *group = NULL;
    GtkWidget *label;
    sid_engine_model_t **list;
    int i;
    int model;
    int engine;
    unsigned int current;

    if (resources_get_int("SidEngine", &engine) < 0) {
        engine = 0;
    }
    if (resources_get_int("SidModel", &model) < 0) {
        model = 0;
    }

    current = (unsigned int)((engine << 8) | model);


    grid = gtk_grid_new();

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<b>SID Engine and Model</b>");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    g_object_set(label, "margin-left", 8, "margin-bottom", 8, NULL);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    list = sid_get_engine_model_list();
    for (i = 0; list[i] != NULL; i++) {
        GtkWidget *radio;

        debug_gtk3("Adding item (%s, %d).", list[i]->name, list[i]->value);
        radio = gtk_radio_button_new_with_label(group, list[i]->name);
        gtk_radio_button_join_group(GTK_RADIO_BUTTON(radio), last);
        g_object_set(radio, "margin-left", 16, NULL);

        if (list[i]->value == current) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUE);
        }

        g_signal_connect(radio, "toggled",
                G_CALLBACK(on_radio_toggled),
                GINT_TO_POINTER(list[i]->value));

        gtk_grid_attach(GTK_GRID(grid), radio, 0, i + 1, 1, 1);

        last = GTK_RADIO_BUTTON(radio);
    }

    gtk_widget_show_all(grid);
    return grid;
}



