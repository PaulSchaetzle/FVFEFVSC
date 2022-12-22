/* fvfefvsc-page.c
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

#include "fvfefvsc-page.h"


G_DEFINE_FINAL_TYPE (FvfefvscPage, fvfefvsc_page, GTK_TYPE_WIDGET)

FvfefvscPage *
fvfefvsc_new_page (void)
{
  return g_object_new(FVFEFVSC_TYPE_PAGE,
                      NULL);
}

void
load_file (FvfefvscPage *self, GFile *file)
{
  gchar* file_buffer;
  self->file_name = g_file_get_path(file);
  g_file_get_contents (self->file_name, &file_buffer, NULL, NULL);
  self->text_buffer = gtk_text_view_get_buffer(self->text_view);
  gtk_text_buffer_set_text(self->text_buffer, file_buffer, -1);
}

void
save_file (FvfefvscPage* self)
{
  GtkTextBuffer *buffer;
  gboolean result;
  gchar *text;
  GtkTextIter start;
  GtkTextIter end;

  buffer = self->text_buffer;
  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);
  text = gtk_text_buffer_get_text (buffer, &start, &end, false);
  result = g_file_set_contents (self->file_name, text, -1, NULL);
}

static void
fvfefvsc_page_class_init (FvfefvscPageClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BIN_LAYOUT);
  gtk_widget_class_set_template_from_resource (widget_class, "/xyz/schaetzle/Fvfefvsc/fvfefvsc-page.ui");
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, box);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, scroller);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, text_view);
}

static void
fvfefvsc_page_init (FvfefvscPage *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}


