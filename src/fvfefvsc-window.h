/* fvfefvsc-window.h
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

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define FVFEFVSC_TYPE_WINDOW (fvfefvsc_window_get_type())

G_DECLARE_FINAL_TYPE (FvfefvscWindow, fvfefvsc_window, FVFEFVSC, WINDOW, AdwApplicationWindow)

struct _FvfefvscWindow
{
  AdwApplicationWindow  parent_instance;

  /* Template widgets */
  GtkHeaderBar        *header_bar;
  GtkLabel            *label;
  GtkTextView         *text_view;
  GtkTextBuffer       *text_buffer;
};

G_END_DECLS