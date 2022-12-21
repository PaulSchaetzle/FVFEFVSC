/* fvfefvsc-window.c
 *
 * Copyright 2022 1000len-6578
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "fvfefvsc-window.h"

struct _FvfefvscWindow
{
  AdwApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  GtkLabel            *label;
  GtkTextView         *text_view;
  GtkTextBuffer       *text_buffer;
  GtkButton           *open_button;
  int                 response;
};

G_DEFINE_FINAL_TYPE (FvfefvscWindow, fvfefvsc_window, ADW_TYPE_APPLICATION_WINDOW)

static
void load_file(FvfefvscWindow *self, GFile *file)
{
  gchar* path_name;
  gchar* file_buffer;
  path_name = g_file_get_path(file);
  g_file_get_contents (path_name, &file_buffer, NULL, NULL);
  self->text_buffer = gtk_text_view_get_buffer(self->text_view);
  gtk_text_buffer_set_text(self->text_buffer, file_buffer, -1);
}

static void
action_open_response(FvfefvscWindow *self, int response, GtkFileChooserNative *native)
{
 if (response == GTK_RESPONSE_ACCEPT)
    {
      GFile *file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER(native));
      load_file(self, file);
      g_object_unref (file);
    }
}


static void
action_open (GtkWidget *widget, const char *action_name, GVariant *param)
{
  FvfefvscWindow *self = (FvfefvscWindow*) widget;
  GtkFileChooserNative *native;
  native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "_Open",
                                        "_Cancel");
  g_signal_connect_object (native, "response", G_CALLBACK (action_open_response), self, G_CONNECT_SWAPPED);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

static void
fvfefvsc_window_class_init (FvfefvscWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/Example/fvfefvsc-window.ui");
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, label);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, text_view);

  gtk_widget_class_install_action (widget_class,
                                   "win.open",
                                   NULL,
                                   action_open);
}

static void
fvfefvsc_window_init (FvfefvscWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}
