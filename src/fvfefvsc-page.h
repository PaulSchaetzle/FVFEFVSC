/* fvfefvsc-page.h
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
#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define FVFEFVSC_TYPE_PAGE (fvfefvsc_page_get_type())

G_DECLARE_FINAL_TYPE (FvfefvscPage, fvfefvsc_page, FVFEFVSC, PAGE, GtkWidget)

struct _FvfefvscPage
{
  GtkWidget           parent_instance;

  /* Template widgets */
  GtkWidget           *box;
  GtkScrolledWindow   *scroller;
  GtkSourceView       *source_view;
  GtkSourceBuffer     *source_buffer;
  gchar               *file_path;
  gchar               *title;
};

void load_file(FvfefvscPage *self, GFile *file);
void save_file(FvfefvscPage* self);
void set_filepath(FvfefvscPage *self, GFile* file);
FvfefvscPage * fvfefvsc_page_new(void);

static void fvfefvsc_page_dispose (GObject *object);


G_END_DECLS
