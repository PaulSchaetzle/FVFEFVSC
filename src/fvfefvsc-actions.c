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
action_show_pages_cb (FvfefvscWindow *self)
{
  g_assert (FVFEFVSC_IS_WINDOW (self));
  if (adw_tab_view_get_n_pages (self->tab_view) > 0)
      gtk_stack_set_visible_child (self->stack, GTK_WIDGET (self->pages));
  else
      gtk_stack_set_visible_child (self->stack, GTK_WIDGET (self->welcome_page));
}

void
action_on_selected_page_changed_cb (FvfefvscWindow *self)
{
  g_assert (FVFEFVSC_IS_WINDOW (self));

  AdwTabPage *selected_page;

  selected_page = adw_tab_view_get_selected_page (self->tab_view);
  self->visible_page = FVFEFVSC_PAGE (adw_tab_page_get_child (selected_page));
}

void
action_on_page_attached_cb (FvfefvscWindow *self,
                            AdwTabPage *tab_page,
                            gint position,
                            AdwTabView *tab_view)
{
  g_assert (FVFEFVSC_IS_WINDOW (self));

  g_object_bind_property (adw_tab_page_get_child (tab_page), "title",
                          tab_page, "title",
                          G_BINDING_SYNC_CREATE);
}

void
action_open_file (FvfefvscWindow *self,
                  GFile          *file)
{
  FvfefvscPage *page = NULL;
  FvfefvscBuffer *buffer = NULL;
  g_assert (FVFEFVSC_IS_WINDOW (self));
  g_assert (G_IS_FILE (file));

  buffer = fvfefvsc_buffer_new_for_file (file);
  page = fvfefvsc_page_new_for_buffer (buffer);

  self->visible_page = page;
  fvfefvsc_buffer_load (buffer);
  g_object_unref (file);
  adw_tab_view_append (self->tab_view, GTK_WIDGET(self->visible_page));
}

static void
action_open_response_cb (FvfefvscWindow *self,
                         int response,
                         GtkFileChooserNative *native)
{
  g_assert (FVFEFVSC_IS_WINDOW (self));

  if (response == GTK_RESPONSE_ACCEPT)
    {
      g_autoptr(GListModel) files = gtk_file_chooser_get_files (GTK_FILE_CHOOSER (native));
      guint i = 0;
      GFile *file = NULL;

      g_assert (g_list_model_get_item_type (files) == G_TYPE_FILE);

      while ((file = G_FILE (g_list_model_get_object (files, i++))))
        {
          action_open_file (self, file);
          g_object_unref (file);
        }
    }
    gtk_native_dialog_destroy (GTK_NATIVE_DIALOG (native));
}

static void
action_save_as_response_cb (FvfefvscWindow *self,
                            int response,
                            GtkFileChooserNative *native)
{
  g_assert (FVFEFVSC_IS_WINDOW (self));

  if (response == GTK_RESPONSE_ACCEPT)
    {
      GFile *file =  gtk_file_chooser_get_file (GTK_FILE_CHOOSER (native));
      FvfefvscBuffer *buffer;
      g_object_get (self->visible_page,
                    "buffer", &buffer,
                    NULL);

      g_object_set (buffer,
                    "file", file,
                    NULL);

      fvfefvsc_buffer_save (buffer);
      g_object_unref (file);

    }
    gtk_native_dialog_destroy (GTK_NATIVE_DIALOG (native));
}

static void
action_open (GtkWidget *widget,
             const char *action_name,
             GVariant *param)
{
  GtkFileChooserNative *native;
  FvfefvscWindow *self = FVFEFVSC_WINDOW (widget);

  g_assert(FVFEFVSC_IS_WINDOW (self));

  native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "_Open",
                                        "_Cancel");

  gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER (native), TRUE);
  g_signal_connect_object (native, "response", G_CALLBACK (action_open_response_cb), self, G_CONNECT_SWAPPED);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

static void
action_save_as (GtkWidget *widget,
                const char *action_name,
                GVariant   *param)
{
  GtkFileChooserNative *native;
  FvfefvscWindow *self = FVFEFVSC_WINDOW (widget);

  g_assert(FVFEFVSC_IS_WINDOW (self));

  native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        "_Save",
                                        "_Cancel");
  g_signal_connect_object (native, "response", G_CALLBACK (action_save_as_response_cb), self, G_CONNECT_SWAPPED);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

static void
action_save (GtkWidget *widget,
             const char *action_name,
             GVariant   *param)
{
  FvfefvscWindow *self = FVFEFVSC_WINDOW (widget);
  g_assert(FVFEFVSC_IS_WINDOW (self));

  gboolean is_draft;
  FvfefvscBuffer *buffer;

  g_object_get (self->visible_page,
                "is_draft", &is_draft,
                "buffer", &buffer,
                NULL);

  if (is_draft)
    action_save_as (GTK_WIDGET (self), NULL, NULL);  // Works for now, maybe implement cleaner later?
  else
    fvfefvsc_buffer_save (buffer);
}

static void
action_new (GtkWidget *widget,
            const char *action_name,
            GVariant *param)
{
  FvfefvscPage *page = NULL;
  FvfefvscBuffer *buffer = NULL;
  FvfefvscWindow *self = FVFEFVSC_WINDOW (widget);
  g_assert(FVFEFVSC_WINDOW (self));

  buffer = fvfefvsc_buffer_new ();
  page = fvfefvsc_page_new_for_buffer (buffer);

  self->visible_page = page;
  adw_tab_view_append(self->tab_view, GTK_WIDGET (self->visible_page));
}

void
fvfefvsc_window_class_actions_init (FvfefvscWindowClass *klass)
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
