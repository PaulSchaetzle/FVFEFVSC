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

#pragma once

#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define FVFEFVSC_TYPE_BUFFER (fvfefvsc_buffer_get_type())

G_DECLARE_FINAL_TYPE (FvfefvscBuffer, fvfefvsc_buffer, FVFEFVSC, BUFFER, GtkSourceBuffer)

struct _FvfefvscBuffer
{
  GtkSourceBuffer     parent_instance;

  GtkSourceFile       *file;
};

FvfefvscBuffer *fvfefvsc_buffer_new_for_file (GFile *file);
FvfefvscBuffer *fvfefvsc_buffer_new (void);

void fvfefvsc_buffer_load (FvfefvscBuffer *self);
void fvfefvsc_buffer_save (FvfefvscBuffer *self);

G_END_DECLS
