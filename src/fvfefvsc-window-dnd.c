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
#include "fvfefvsc-window.h"

gboolean
action_on_drop_cb (FvfefvscWindow *self,
                const GValue *value,
                double x,
                double y,
                GtkDropTarget *target)
{
  g_assert (FVFEFVSC_IS_WINDOW (self));
  g_assert (GTK_IS_DROP_TARGET (target));

  if (G_VALUE_HOLDS (value, GDK_TYPE_FILE_LIST))
    {
      GSList *list = g_value_get_boxed (value);

      for (const GSList *iter = list; iter; iter = iter->next)
        {
          GFile *file = iter->data;
          g_assert (G_IS_FILE (file));
          action_open_file (self, file);
        }
      return TRUE;
    }
  return FALSE;
}

void
fvfefvsc_window_dnd_init (FvfefvscWindow *self)
{
  g_assert (FVFEFVSC_IS_WINDOW (self));

  self->drop_target = gtk_drop_target_new (GDK_TYPE_FILE_LIST, GDK_ACTION_COPY);
  g_signal_connect_swapped (self->drop_target, "drop", G_CALLBACK (action_on_drop_cb), self);
  gtk_widget_add_controller (GTK_WIDGET (self), GTK_EVENT_CONTROLLER (self->drop_target));
}
