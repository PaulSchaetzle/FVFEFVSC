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
fvfefvsc_window_dispose (GObject *object)
{
  FvfefvscWindow *self = FVFEFVSC_WINDOW (object);
  g_assert (FVFEFVSC_IS_WINDOW (self));

  g_signal_handlers_disconnect_by_func (adw_tab_view_get_pages (self->tab_view), G_CALLBACK (action_show_pages_cb), self);   // disconnect the signal so the callback wont be triggered by the destuction process

  g_clear_pointer ((GtkWidget **)&self->welcome_page, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->pages, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->tab_bar, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->tab_view, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->stack, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->header_bar, gtk_widget_unparent);

  G_OBJECT_CLASS (fvfefvsc_window_parent_class)->dispose (object);
}

static void
fvfefvsc_window_class_init (FvfefvscWindowClass *klass)
{

  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = fvfefvsc_window_dispose;


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

  fvfefvsc_window_class_actions_init (klass);  // intialize fvfefvsc-actions
}

static void
fvfefvsc_window_init (FvfefvscWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  g_signal_connect_swapped (adw_tab_view_get_pages (self->tab_view), "items-changed", G_CALLBACK (action_show_pages_cb), self);
  g_signal_connect_swapped (self->tab_view, "notify::selected-page", G_CALLBACK (action_on_selected_page_changed_cb), self);
  g_signal_connect_swapped (self->tab_view, "page-attached", G_CALLBACK (action_on_page_attached_cb), self);

  gtk_stack_set_visible_child (self->stack, GTK_WIDGET (self->welcome_page));

  fvfefvsc_window_dnd_init (self);
}
