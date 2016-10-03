#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

GtkWidget *number_games;
GtkWidget *ph_entry;
GtkWidget *pr_entry;
GtkWidget       *warning_window;

int ph;
int pr;
int number_of_games=0;

double ph_double;
double pr_double;

bool is_number(gchar* pvalue){

  int length,i;

  length = strlen (pvalue);
   for (i=0;i<length; i++)
       if (!isdigit(pvalue[i]))
       {
           return FALSE;
       }
   return TRUE;

}

void btn_warning_clicked_cb(){
  gtk_widget_destroy (warning_window);
}

void create_warning_window(gchar* pMessage){
  GtkBuilder      *warning_builder;

  GtkWidget       *message_label;

  warning_builder = gtk_builder_new();
  gtk_builder_add_from_file (warning_builder, "glade/warning_window.glade", NULL);

  warning_window = GTK_WIDGET(gtk_builder_get_object(warning_builder, "warning_window"));
  gtk_builder_connect_signals(warning_builder,NULL);

  message_label = GTK_WIDGET(gtk_builder_get_object(warning_builder, "lbl_warning"));

  gtk_label_set_text (message_label,pMessage);

  g_object_unref(warning_builder);

  gtk_widget_show_all(warning_window);
  gtk_main();


  printf("%s\n", pMessage);

}

void on_create_button_clicked(){


  ph = atoi(gtk_entry_get_text (ph_entry));
  ph_double = (double) ph / 100;

  pr = atoi(gtk_entry_get_text (pr_entry));
  pr_double = (double) pr / 100;

  number_of_games = atoi(gtk_entry_get_text (number_games));



  printf("%f\n", ph_double);
  printf("%f\n", pr_double);
  printf("%d\n", number_of_games);

}

int main(int argc, char *argv[]){

  //Main function executes the initial window

  GtkBuilder      *initial_series_builder;
  GtkWidget       *initial_series_window;

  /*--- CSS -----------------*/
GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;
/*-------------------------*/

gtk_init(&argc, &argv);

initial_series_builder = gtk_builder_new();
gtk_builder_add_from_file (initial_series_builder, "glade/initial_series_deportivas_window.glade", NULL);

initial_series_window = GTK_WIDGET(gtk_builder_get_object(initial_series_builder, "initial_series_deportivas_window"));

number_games= GTK_WIDGET(gtk_builder_get_object(initial_series_builder, "number_games_entry"));
ph_entry=GTK_WIDGET(gtk_builder_get_object(initial_series_builder, "ph_entry"));
pr_entry=GTK_WIDGET(gtk_builder_get_object(initial_series_builder, "pr_entry"));


gtk_builder_connect_signals(initial_series_builder,NULL);

provider = gtk_css_provider_new ();
display = gdk_display_get_default ();
screen = gdk_display_get_default_screen (display);
gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

gsize bytes_written, bytes_read;

const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

GError *error = 0;

gtk_css_provider_load_from_path (provider,g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);

g_object_unref(initial_series_builder);

gtk_widget_show_all(initial_series_window);
gtk_main();

return 0;
}
