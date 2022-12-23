#include "config.h"

#include "fvfefvsc-window.h"

static void
action_save (GtkWidget *widget,
             const char *action_name,
             GVariant   *param)
{
  FvfefvscWindow *self = (FvfefvscWindow*) widget;
  save_file(self->visible_page);
}

static void
action_open_response(FvfefvscWindow *self, int response, GtkFileChooserNative *native)
{
 if (response == GTK_RESPONSE_ACCEPT)
    {
      AdwTabPage *tab_page;
      self->visible_page = fvfefvsc_page_new ();
      GFile *file =  gtk_file_chooser_get_file (GTK_FILE_CHOOSER(native));
      load_file(self->visible_page, file);
      g_object_unref (file);
      tab_page = adw_tab_view_add_page(self->tab_view, GTK_WIDGET(self->visible_page), NULL);
      adw_tab_page_set_title(tab_page, self->visible_page->file_path);
    }

    gtk_native_dialog_destroy (GTK_NATIVE_DIALOG (native));
}

static void
action_save_as_response(FvfefvscWindow *self, int response, GtkFileChooserNative *native)
{
 if (response == GTK_RESPONSE_ACCEPT)
    {
      GFile *file =  gtk_file_chooser_get_file (GTK_FILE_CHOOSER(native));
      set_filepath (self->visible_page, g_file_get_path(file));
      g_object_unref (file);
    }
    gtk_native_dialog_destroy (GTK_NATIVE_DIALOG (native));
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
action_save_as (GtkWidget *widget,
             const char *action_name,
             GVariant   *param)
{
  FvfefvscWindow *self = (FvfefvscWindow*) widget;
  GtkFileChooserNative *native;
  native = gtk_file_chooser_native_new ("Open File",
                                        GTK_WINDOW (self),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        "_Save",
                                        "_Cancel");
  g_signal_connect_object (native, "response", G_CALLBACK (action_save_as_response), self, G_CONNECT_SWAPPED);
  gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

static void
action_new (GtkWidget *widget,
            const char *action_name,
            GVariant *param)
{
  FvfefvscWindow *self = FVFEFVSC_WINDOW (widget);
  AdwTabPage *tab_page;
  self->visible_page = fvfefvsc_page_new ();
  tab_page = adw_tab_view_add_page(self->tab_view, GTK_WIDGET(self->visible_page), NULL);
  adw_tab_page_set_title(tab_page, "New Document");
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
  gtk_widget_class_install_action (widget_class,
                                  "win.save_as",
                                  NULL,
                                  action_save_as);

  gtk_widget_class_install_action (widget_class,
                                  "win.new",
                                  NULL,
                                  action_new);
}
