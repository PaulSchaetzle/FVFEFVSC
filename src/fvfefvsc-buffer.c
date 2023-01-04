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

#include "config.h"

#include "fvfefvsc-buffer.h"

G_DEFINE_FINAL_TYPE (FvfefvscBuffer, fvfefvsc_buffer, GTK_SOURCE_TYPE_BUFFER);

typedef enum
{
  PROP_FILE = 1,
  PROP_TITLE,
  N_PROPERTIES,
} FvfefvscBufferProperty;

static GParamSpec *object_properties[N_PROPERTIES] = {NULL, };

FvfefvscBuffer *
fvfefvsc_buffer_new_for_file (GFile *file)
{
  g_assert (G_IS_FILE (file));
  return g_object_new (FVFEFVSC_TYPE_BUFFER,
                       "file", file,
                       NULL);
}

FvfefvscBuffer *
fvfefvsc_buffer_new (void)
{
  return g_object_new (FVFEFVSC_TYPE_BUFFER,
                       NULL);
}

static void
fvfefvsc_buffer_load_cb (GObject *source_object,
                         GAsyncResult* res,
                         gpointer user_data)
{
  GtkSourceFileLoader *loader = GTK_SOURCE_FILE_LOADER (source_object);
  FvfefvscBuffer *self = FVFEFVSC_BUFFER (user_data);
  g_assert (GTK_SOURCE_IS_FILE_LOADER (loader));
  g_assert (FVFEFVSC_IS_BUFFER (self));

  if (gtk_source_file_loader_load_finish (loader, res, NULL))
    {
      GtkSourceLanguageManager *language_manager;
      GtkSourceLanguage *language;
      GFile *file;
      g_autofree gchar *filename;

      g_object_get (self,
                    "file",
                    &file, NULL);

      filename = g_file_get_parse_name (file);

      language_manager = gtk_source_language_manager_get_default ();
      language = gtk_source_language_manager_guess_language (language_manager,
                                                             filename,
                                                             NULL);
      gtk_source_buffer_set_language (GTK_SOURCE_BUFFER (self), language);
    }
}

void
fvfefvsc_buffer_load (FvfefvscBuffer *self)
{
  g_assert (GTK_SOURCE_IS_FILE (self->file));
  GtkSourceFileLoader *loader = gtk_source_file_loader_new (GTK_SOURCE_BUFFER (self), self->file);

  gtk_source_file_loader_load_async (loader,
                                     G_PRIORITY_DEFAULT,
                                     NULL,
                                     NULL,
                                     NULL,
                                     NULL,
                                     fvfefvsc_buffer_load_cb,
                                     self);
}

static void
fvfefvsc_buffer_save_cb (GObject *source_object,
                         GAsyncResult* res,
                         gpointer user_data)
{
  GtkSourceFileSaver *saver = GTK_SOURCE_FILE_SAVER (source_object);
  FvfefvscBuffer *self = FVFEFVSC_BUFFER (user_data);
  g_assert (GTK_SOURCE_IS_FILE_SAVER (saver));
  g_assert (FVFEFVSC_IS_BUFFER (self));

  g_object_notify_by_pspec (G_OBJECT (self), object_properties[PROP_TITLE]);
}

void
fvfefvsc_buffer_save (FvfefvscBuffer *self)
{
  g_assert (GTK_SOURCE_IS_FILE (self->file));
  GtkSourceFileSaver *saver = gtk_source_file_saver_new (GTK_SOURCE_BUFFER (self), self->file);

  gtk_source_file_saver_save_async (saver,
                                    G_PRIORITY_DEFAULT,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL,
                                    fvfefvsc_buffer_save_cb,
                                    self);
}

static gchar *
fvfefvsc_buffer_get_title (FvfefvscBuffer *self)
{
  g_assert (FVFEFVSC_IS_BUFFER (self));
  GFile *file = gtk_source_file_get_location (self->file);
  gchar *title;

  if (file != NULL)
    title = g_file_get_basename (file);
  else
    title = "New Document";   // Fixme: Works but could be better!

  return title;
}

static void
fvfefvsc_buffer_set_properties (GObject *object,
                                guint property_id,
                                const GValue *value,
                                GParamSpec *pspec)
{
  FvfefvscBuffer *self = FVFEFVSC_BUFFER (object);
  g_assert (FVFEFVSC_IS_BUFFER (self));

  switch (property_id)
    {
    case PROP_FILE:
      gtk_source_file_set_location (self->file, g_value_get_object (value));
      break;

    default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
fvfefvsc_buffer_get_properties (GObject *object,
                                guint property_id,
                                GValue *value,
                                GParamSpec *pspec)
{
  FvfefvscBuffer *self = FVFEFVSC_BUFFER (object);
  g_assert (FVFEFVSC_IS_BUFFER (self));

  switch (property_id)
    {
    case PROP_FILE:
      g_value_set_object (value, gtk_source_file_get_location (self->file));
      break;

    case PROP_TITLE:
      g_value_set_string (value, fvfefvsc_buffer_get_title (self));
      break;

    default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
fvfefvsc_buffer_class_init (FvfefvscBufferClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = fvfefvsc_buffer_set_properties;
  object_class->get_property = fvfefvsc_buffer_get_properties;

  object_properties[PROP_FILE] =
    g_param_spec_object ("file",
                         "File",
                         "GFile representing the opened file",
                         G_TYPE_FILE,
                         G_PARAM_READWRITE);

  object_properties[PROP_TITLE] =
    g_param_spec_string ("title",
                         "Title",
                         "Name of the File, used as title for the tab pages",
                         "New Document",
                         G_PARAM_READABLE);

  g_object_class_install_properties (object_class,
                                     N_PROPERTIES,
                                     object_properties);
}

static void
fvfefvsc_buffer_init (FvfefvscBuffer *self)
{
  self->file = gtk_source_file_new();
}
