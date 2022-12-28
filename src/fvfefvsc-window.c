/* fvfefvsc-window.c
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

// header
#include "fvfefvsc-window.h"

G_DEFINE_FINAL_TYPE (FvfefvscWindow, fvfefvsc_window, ADW_TYPE_APPLICATION_WINDOW)

static void
fvfefvsc_window_class_init (FvfefvscWindowClass *klass)
{

  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  // GtkWindowClass *window_calss = GTK_WINDOW_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/xyz/schaetzle/Fvfefvsc/fvfefvsc-window.ui");
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, header_bar);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, tab_view);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, tab_bar);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, stack);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, pages);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscWindow, welcome_page);

  // Keybindings
  gtk_widget_class_add_binding_action (widget_class, GDK_KEY_o, GDK_CONTROL_MASK, "win.open", NULL);
  gtk_widget_class_add_binding_action (widget_class, GDK_KEY_s, GDK_CONTROL_MASK, "win.save", NULL);
  gtk_widget_class_add_binding_action (widget_class, GDK_KEY_s, GDK_CONTROL_MASK | GDK_SHIFT_MASK, "win.save_as", NULL);
  gtk_widget_class_add_binding_action (widget_class, GDK_KEY_n, GDK_CONTROL_MASK, "win.new", NULL);

  g_type_ensure(FVFEFVSC_TYPE_PAGE);

  _fvfefvsc_window_class_actions_init (klass);  // intialize fvfefvsc-actions
}

static void
fvfefvsc_window_init (FvfefvscWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  GType drop_types[1] = {G_TYPE_FILE};
  self->drop_target = gtk_drop_target_new (G_TYPE_FILE, GDK_ACTION_COPY);
  gtk_drop_target_set_gtypes (self->drop_target, drop_types, 1);

  g_signal_connect (self->drop_target, "drop", G_CALLBACK (on_drop), self);
  g_signal_connect_swapped (adw_tab_view_get_pages (self->tab_view), "items-changed", G_CALLBACK (show_pages), self);
  g_signal_connect_swapped (self->tab_view, "notify::selected-page", G_CALLBACK (update_window), self);

  gtk_widget_add_controller (GTK_WIDGET (self), GTK_EVENT_CONTROLLER (self->drop_target));

  gtk_stack_set_visible_child (self->stack, GTK_WIDGET(self->welcome_page));
}

static void
show_pages (FvfefvscWindow *self)
{
  g_assert(FVFEFVSC_IS_WINDOW (self));
  gtk_stack_set_visible_child (self->stack, GTK_WIDGET(self->pages));
}

static void
update_window (FvfefvscWindow *self)
{
  g_assert(FVFEFVSC_IS_WINDOW (self));

  AdwTabPage *selected_page;
  AdwTabView *tab_view = self->tab_view;

  selected_page = adw_tab_view_get_selected_page (tab_view);
  self->visible_page = FVFEFVSC_PAGE (adw_tab_page_get_child (selected_page));
  gtk_window_set_title (GTK_WINDOW(self), self->visible_page->title);
}

static gboolean on_drop (GtkDropTarget *target,
                         const GValue *value,
                         double x,
                         double y,
                         gpointer data)
{
  FvfefvscWindow* self = (FvfefvscWindow*) data;
  g_assert(FVFEFVSC_IS_WINDOW (self));

  if (G_VALUE_HOLDS (value, G_TYPE_FILE))
    {
      action_open_file(self, g_value_get_object (value));
    }
  else
    return FALSE;

  return TRUE;
}
