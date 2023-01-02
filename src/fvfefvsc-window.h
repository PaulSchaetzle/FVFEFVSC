/* fvfefvsc-window.h
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

#pragma once

#include <adwaita.h>

#include "fvfefvsc-page.h"

G_BEGIN_DECLS

#define FVFEFVSC_TYPE_WINDOW (fvfefvsc_window_get_type())

G_DECLARE_FINAL_TYPE (FvfefvscWindow, fvfefvsc_window, FVFEFVSC, WINDOW, AdwApplicationWindow)

struct _FvfefvscWindow
{
  AdwApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  AdwTabView          *tab_view;
  AdwTabBar           *tab_bar;
  FvfefvscPage        *visible_page;
  GtkStack            *stack;
  GtkBox              *pages;
  AdwStatusPage       *welcome_page;
  GtkDropTarget       *drop_target;
};

// GObject Functions
static void fvfefvsc_window_dispose (GObject *object);
// static void fvfefvsc_window_finalize (GObject *object);

// Callbacks
void action_show_pages(FvfefvscWindow *self);
void action_on_selected_page_changed (FvfefvscWindow *self);
gboolean action_on_drop (FvfefvscWindow *self,
                         const GValue *value,
                         double x,
                         double y,
                         GtkDropTarget *target);
void
action_on_page_attached (FvfefvscWindow *self,
                         AdwTabPage *tab_page,
                         gint position,
                         AdwTabView *tab_view);

// Fvfefvsc-actions constructor call
void _fvfefvsc_window_class_actions_init (FvfefvscWindowClass *klass);

void action_open_file (FvfefvscWindow *self,
                       GFile          *file);

G_END_DECLS
