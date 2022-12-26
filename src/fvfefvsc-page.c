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

#include <glib.h>
#include <glib/gstdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

G_DEFINE_FINAL_TYPE (FvfefvscPage, fvfefvsc_page, GTK_TYPE_WIDGET)

FvfefvscPage *
fvfefvsc_page_new (void)
{
  return g_object_new(FVFEFVSC_TYPE_PAGE,
                      NULL);
}

void
load_file (FvfefvscPage *self, GFile *file)
{
  g_assert(FVFEFVSC_IS_WIDGET(self));
  gchar *file_buffer;
  gchar *title;
  gchar *file_path;
  GtkTextBuffer *source_buffer = (GtkTextBuffer *) self->source_buffer;

  file_path = g_file_get_path(file);
  title = g_file_get_basename (file);
  self->file_path = file_path;
  self->title = title;
  g_file_get_contents (file_path, &file_buffer, NULL, NULL);
  gtk_text_buffer_set_text(source_buffer, file_buffer, -1);
}

void
save_file (FvfefvscPage* self)
{
  g_assert(FVFEFVSC_IS_WIDGET(self));
  GtkTextBuffer *buffer = (GtkTextBuffer *) self->source_buffer;
  gchar *text;
  GtkTextIter start;
  GtkTextIter end;

  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);
  text = gtk_text_buffer_get_text (buffer, &start, &end, false);
  g_file_set_contents (self->file_path, text, -1, NULL);
  gtk_text_buffer_set_modified (buffer, FALSE);
}

void
set_filepath(FvfefvscPage *self, gchar *file_path)
{
  g_assert(FVFEFVSC_IS_WIDGET(self));
  self->file_path = file_path;
}

static void
fvfefvsc_page_class_init (FvfefvscPageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  object_class->dispose = fvfefvsc_page_dispose;

  gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BIN_LAYOUT);
  gtk_widget_class_set_template_from_resource (widget_class, "/xyz/schaetzle/Fvfefvsc/fvfefvsc-page.ui");
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, box);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, scroller);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, source_view);

  // See https://discourse.gnome.org/t/using-gtksourceview-in-glade-template/652
  g_type_ensure(GTK_SOURCE_TYPE_VIEW);
}

static void
fvfefvsc_page_init (FvfefvscPage *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->source_buffer = (GtkSourceBuffer *) gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->source_view));
}

static void
fvfefvsc_page_dispose (GObject *object)
{
  FvfefvscPage *self = (FvfefvscPage *) object;
  g_clear_pointer ((GtkWidget **)&self->box, gtk_widget_unparent);
}
