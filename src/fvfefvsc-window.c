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
  // GtkWindowClass *window_class = GTK_WINDOW_CLASS (klass);

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

  self->drop_target = gtk_drop_target_new (GDK_TYPE_FILE_LIST, GDK_ACTION_COPY);

  g_signal_connect (self->drop_target, "drop", G_CALLBACK (action_on_drop), self);
  g_signal_connect_swapped (adw_tab_view_get_pages (self->tab_view), "items-changed", G_CALLBACK (action_show_pages), self);
  g_signal_connect_swapped (self->tab_view, "notify::selected-page", G_CALLBACK (action_update_window), self);

  gtk_widget_add_controller (GTK_WIDGET (self), GTK_EVENT_CONTROLLER (self->drop_target));

  gtk_stack_set_visible_child (self->stack, GTK_WIDGET(self->welcome_page));
}
