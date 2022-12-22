#include "config.h"

#include "fvfefvsc-window.h"

void
load_file(FvfefvscWindow *self, GFile *file)
{
  gchar* path_name;
  gchar* file_buffer;
  path_name = g_file_get_path(file);
  g_file_get_contents (path_name, &file_buffer, NULL, NULL);
  self->text_buffer = gtk_text_view_get_buffer(self->text_view);
  gtk_text_buffer_set_text(self->text_buffer, file_buffer, -1);
}

static void
action_open_response(FvfefvscWindow *self, int response, GtkFileChooserNative *native)
{
 if (response == GTK_RESPONSE_ACCEPT)
    {
      GFile *file =  gtk_file_chooser_get_file (GTK_FILE_CHOOSER(native));
      load_file(self, file);
      g_object_unref (file);
    }
}

static void
action_open (GtkWidget *widget, const char *action_name, GVariant *param)
{
  FvfefvscWindow *self = (FvfefvscWindow*) widget;
  GtkFileChooserNative *native;
  native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        "_Open",
                                        "_Cancel");
  g_signal_connect_object (native, "response", G_CALLBACK (action_open_response), self, G_CONNECT_SWAPPED);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

static void
action_save (GtkWidget *widget,
             const char *action_name,
             GVariant   *param)
{
  FvfefvscWindow *self = (FvfefvscWindow*) widget;

}

void
_fvfefvsc_window_class_actions_init(FvfefvscWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_install_action (widget_class,
                                  "win.open",
                                  NULL,
                                  action_open);

  gtk_widget_class_install_action (widget_class,
                                  "win.save",
                                  NULL,
                                  action_save);
}
