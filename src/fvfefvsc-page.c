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
  PROP_BUFFER = 1,
  PROP_TITLE,
  N_PROPERTIES
} FvfefvscPageProperty;

static GParamSpec *object_properties[N_PROPERTIES] = {NULL, };

FvfefvscPage *
fvfefvsc_page_new_for_buffer (FvfefvscBuffer *buffer)
{
  g_assert (FVFEFVSC_IS_BUFFER (buffer));
  return g_object_new (FVFEFVSC_TYPE_PAGE,
                       "buffer", buffer,
                       NULL);
}

static void
fvfefvsc_page_set_buffer (FvfefvscPage *self,
                          FvfefvscBuffer *buffer)
{
  g_assert (FVFEFVSC_IS_PAGE (self));
  g_assert (FVFEFVSC_IS_BUFFER (buffer));
  g_assert (self->buffer == NULL);

  if (g_set_object (&self->buffer, buffer))
    {
      gtk_text_view_set_buffer (GTK_TEXT_VIEW (self->view), GTK_TEXT_BUFFER (self->buffer));

      g_object_bind_property (self->buffer, "title",
                              self, "title",
                              G_BINDING_SYNC_CREATE);
    }
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
    case PROP_BUFFER:
      fvfefvsc_page_set_buffer (self, g_value_get_object (value));
      break;

    case PROP_TITLE:
      self->title = g_value_dup_string (value);
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
    case PROP_BUFFER:
      g_value_set_object (value, self->buffer);
      break;

    case PROP_TITLE:
      g_value_set_string (value, self->title);
      break;

    default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
fvfefvsc_page_dispose (GObject *object)
{
  FvfefvscPage *self = FVFEFVSC_PAGE (object);
  g_assert (FVFEFVSC_IS_PAGE (self));

  g_clear_pointer ((GtkWidget **)&self->box, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->scroller, gtk_widget_unparent);
  g_clear_pointer ((GtkWidget **)&self->view, gtk_widget_unparent);

  G_OBJECT_CLASS (fvfefvsc_page_parent_class)->dispose (object);
}

static void
fvfefvsc_page_finalize (GObject *object)
{
  FvfefvscPage *self = FVFEFVSC_PAGE (object);
  g_assert (FVFEFVSC_IS_PAGE (self));

  g_free (self->title);

  G_OBJECT_CLASS (fvfefvsc_page_parent_class)->finalize (object);
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

  object_properties[PROP_BUFFER] =
    g_param_spec_object ("buffer",
                         "Buffer",
                         "Buffer that contains the files contents",
                         FVFEFVSC_TYPE_BUFFER,
                         G_PARAM_READWRITE);

  object_properties[PROP_TITLE] =
    g_param_spec_string ("title",
                         "Title",
                         "Name of the File, used as title for the tab pages",
                         "New Document",
                         G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     N_PROPERTIES,
                                     object_properties);

  gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BIN_LAYOUT);
  gtk_widget_class_set_template_from_resource (widget_class, "/xyz/schaetzle/Fvfefvsc/fvfefvsc-page.ui");
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, box);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, scroller);
  gtk_widget_class_bind_template_child (widget_class, FvfefvscPage, view);

  // See https://discourse.gnome.org/t/using-gtksourceview-in-glade-template/652
  g_type_ensure (GTK_SOURCE_TYPE_VIEW);
  g_type_ensure (FVFEFVSC_TYPE_BUFFER);
}

static void
fvfefvsc_page_init (FvfefvscPage *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->buffer = NULL;
}
