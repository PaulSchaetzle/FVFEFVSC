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

typedef enum
{
  PROP_FILE = 1,
  PROP_TITLE,
  N_PROPERTIES
} FvfefvscPageProperty;

static GParamSpec *object_properties[N_PROPERTIES] = {NULL, };

FvfefvscPage *
fvfefvsc_page_new_for_file (GFile *file)
{
  g_assert (G_IS_FILE (file));
  return g_object_new (FVFEFVSC_TYPE_PAGE,
                         "file", file,
                         NULL);
}

FvfefvscPage *
fvfefvsc_page_new_empty (void)
{
  return g_object_new (FVFEFVSC_TYPE_PAGE,
                       NULL);
}

void
load_file (FvfefvscPage *self)
{
  g_assert (FVFEFVSC_IS_PAGE(self));
  g_autofree gchar *file_path = g_file_get_path (self->file);
  g_autofree gchar *file_buffer;
  GtkTextBuffer *source_buffer = GTK_TEXT_BUFFER (self->source_buffer);

  g_file_get_contents (file_path, &file_buffer, NULL, NULL);
  gtk_text_buffer_set_text(source_buffer, file_buffer, -1);
}

void
save_file (FvfefvscPage* self)
{
  g_assert (FVFEFVSC_IS_PAGE(self));
  GtkTextBuffer *source_buffer = GTK_TEXT_BUFFER (self->source_buffer);
  g_autofree gchar *file_buffer;
  g_autofree gchar *file_path = g_file_get_path (self->file);
  GtkTextIter start;
  GtkTextIter end;

  gtk_text_buffer_get_start_iter (source_buffer, &start);
  gtk_text_buffer_get_end_iter (source_buffer, &end);
  file_buffer = gtk_text_buffer_get_text (source_buffer, &start, &end, false);
  g_file_set_contents (file_path, file_buffer, -1, NULL);
  gtk_text_buffer_set_modified (source_buffer, FALSE);
}

static gchar *
get_file_name (FvfefvscPage *self)
{
  g_assert (FVFEFVSC_IS_PAGE (self));

  if (self->file)
    return g_file_get_basename (self->file);
  return "New Document";
}

static void fvfefvsc_page_set_properties (GObject *object,
                                          guint property_id,
                                          const GValue *value,
                                          GParamSpec *pspec)
{
  FvfefvscPage *self = FVFEFVSC_PAGE (object);
  g_assert (FVFEFVSC_IS_PAGE (self));

  switch ((FvfefvscPageProperty) property_id)
    {
    case PROP_FILE:
      self->file = g_value_get_object (value);
      break;

    default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void fvfefvsc_page_get_properties (GObject *object,
                                          guint property_id,
                                          GValue *value,
                                          GParamSpec *pspec)
{
  FvfefvscPage *self = FVFEFVSC_PAGE (object);
  g_assert (FVFEFVSC_IS_PAGE (self));

  switch ((FvfefvscPageProperty) property_id)
    {
    case PROP_FILE:
      g_value_set_object (value, self->file);
      break;

    case PROP_TITLE:
      g_value_set_string (value, get_file_name (self));
      break;

    default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
fvfefvsc_page_class_init (FvfefvscPageClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

  object_class->dispose = fvfefvsc_page_dispose;
  object_class->finalize = fvfefvsc_page_finalize;
  object_class->set_property = fvfefvsc_page_set_properties;
  object_class->get_property = fvfefvsc_page_get_properties;

  object_properties[PROP_FILE] =
    g_param_spec_object ("file",
                         "File",
                         "GFile representing the opened file",
                         G_TYPE_FILE,
                         G_PARAM_READWRITE);

  object_properties[PROP_TITLE] =
    g_param_spec_string ("file_name",
                         "File Name",
                         "Name of the File, used as title for the tab pages",
                         NULL,
                         G_PARAM_READABLE);

  g_object_class_install_properties (object_class,
                                     N_PROPERTIES,
                                     object_properties);

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
  FvfefvscPage *self = FVFEFVSC_PAGE (object);
  g_assert (FVFEFVSC_IS_PAGE (self));

  g_clear_pointer ((GtkWidget **)&self->box, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->scroller, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->source_view, gtk_widget_unparent);

  g_clear_object (&self->source_buffer);

  G_OBJECT_CLASS (fvfefvsc_page_parent_class)->dispose (object);
}

static void
fvfefvsc_page_finalize (GObject *object)
{
  FvfefvscPage *self = FVFEFVSC_PAGE (object);
  g_assert (FVFEFVSC_IS_PAGE (self));

  G_OBJECT_CLASS (fvfefvsc_page_parent_class)->finalize (object);
}
