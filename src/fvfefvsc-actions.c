/* fvfefvsc-application.c
 *
 * Copyright 2022 Paul Schaetzle
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

void
action_open_file (FvfefvscWindow *self,
                  GFile          *file)
{
  AdwTabPage *tab_page;
  g_assert (FVFEFVSC_IS_WINDOW (self));

  self->visible_page = fvfefvsc_page_new ();
  load_file(self->visible_page, file);
  g_object_unref (file);
  tab_page = adw_tab_view_append(self->tab_view, GTK_WIDGET(self->visible_page));
  adw_tab_page_set_title(tab_page, self->visible_page->title);
}

static void
action_open_response(FvfefvscWindow *self,
                     int response,
                     GtkFileChooserNative *native)
{
 if (response == GTK_RESPONSE_ACCEPT)
    {
      GFile *file =  gtk_file_chooser_get_file (GTK_FILE_CHOOSER(native));
      action_open_file(self, file);
    }

    gtk_native_dialog_destroy (GTK_NATIVE_DIALOG (native));
}

static void
action_save_as_response(FvfefvscWindow *self,
                        int response,
                        GtkFileChooserNative *native)
{
 if (response == GTK_RESPONSE_ACCEPT)
    {
      AdwTabPage *tab_page;
      GFile *file =  gtk_file_chooser_get_file (GTK_FILE_CHOOSER(native));

      set_filepath (self->visible_page, file);
      save_file(self->visible_page);
      g_object_unref (file);
      tab_page = adw_tab_view_get_selected_page (self->tab_view);
      adw_tab_page_set_title (tab_page, self->visible_page->title);

    }
    gtk_native_dialog_destroy (GTK_NATIVE_DIALOG (native));
}

void
action_open (GtkWidget *widget,
             const char *action_name,
             GVariant *param)
{
  GtkFileChooserNative *native;
  FvfefvscWindow *self = (FvfefvscWindow*) widget;

  g_assert(FVFEFVSC_WINDOW (self));

  native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "_Open",
                                        "_Cancel");
  g_signal_connect_object (native, "response", G_CALLBACK (action_open_response), self, G_CONNECT_SWAPPED);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

void
action_save_as (GtkWidget *widget,
                const char *action_name,
                GVariant   *param)
{
  GtkFileChooserNative *native;
  FvfefvscWindow *self = (FvfefvscWindow*) widget;

  g_assert(FVFEFVSC_WINDOW (self));

  native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        "_Save",
                                        "_Cancel");
  g_signal_connect_object (native, "response", G_CALLBACK (action_save_as_response), self, G_CONNECT_SWAPPED);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

void
action_save (GtkWidget *widget,
             const char *action_name,
             GVariant   *param)
{
  FvfefvscWindow *self = (FvfefvscWindow*) widget;

  g_assert(FVFEFVSC_WINDOW (self));

  if (self->visible_page->file_path)
    {
      save_file(self->visible_page);
    }
  else
    {
      action_save_as(GTK_WIDGET(self), NULL, NULL);   // Works for now, maybe implement cleaner later?
    }
}

void
action_new (GtkWidget *widget,
            const char *action_name,
            GVariant *param)
{
  AdwTabPage *tab_page;
  FvfefvscWindow *self = FVFEFVSC_WINDOW (widget);

  g_assert(FVFEFVSC_WINDOW (self));

  self->visible_page = fvfefvsc_page_new ();
  tab_page = adw_tab_view_append(self->tab_view, GTK_WIDGET(self->visible_page));
  adw_tab_page_set_title(tab_page, "New Document");
}

void
_fvfefvsc_window_class_actions_init(FvfefvscWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_install_action (widget_class,
                                  "win.open",
                                  NULL,
                                  action_open);

  gtk_widget_class_install_action (widget_class,
                                  "win.save",
                                  NULL,
                                  action_save);
  gtk_widget_class_install_action (widget_class,
                                  "win.save_as",
                                  NULL,
                                  action_save_as);

  gtk_widget_class_install_action (widget_class,
                                  "win.new",
                                  NULL,
                                  action_new);
}
