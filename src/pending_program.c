#include <gtk/gtk.h>


int main(int argc, char *argv[])
{
  GtkBuilder      *pending_builder;
  GtkWidget       *pending_window;

  /*--- CSS -----------------*/
GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;
/*-------------------------*/

  gtk_init(&argc, &argv);

  pending_builder = gtk_builder_new();
  gtk_builder_add_from_file (pending_builder, "glade/pending_window.glade", NULL);

  pending_window = GTK_WIDGET(gtk_builder_get_object(pending_builder, "pending_window"));
  gtk_builder_connect_signals(pending_builder, NULL);

  
  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

  GError *error = 0;

  gtk_css_provider_load_from_path (provider,
                                    g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
                                    NULL);

  g_object_unref(pending_builder);



  gtk_widget_show(pending_window);
  gtk_main();

  return 0;
}
void on_btn_terminar_clicked(){
  gtk_main_quit();
}
