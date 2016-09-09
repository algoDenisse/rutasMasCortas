#include <gtk/gtk.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>


//Global variables
GtkWidget *entry_initial_price;
GtkWidget *entry_useful_life;
GtkWidget *entry_project_term;
GtkFileChooser *file_chooser;
int initial_price,useful_life,project_term = 0;
GtkWidget       *warning_window;

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

void btn_ok_clicked_cb(){
  gchar* message;
  message = calloc(1, 500*sizeof(gchar));


  initial_price = gtk_entry_get_text (entry_initial_price);
  useful_life = gtk_entry_get_text (entry_useful_life);
  project_term = gtk_entry_get_text (entry_project_term);

  if(!is_number(initial_price) || !is_number(useful_life) || !is_number(project_term)){

    create_warning_window("El tipo de dato debe ser entero");

  }
}


int main(int argc, char *argv[]){

  //Main function executes the initial window

  GtkBuilder      *initial_replacement_builder;
  GtkWidget       *initial_replacement_window;

  /*--- CSS -----------------*/
GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;
/*-------------------------*/

gtk_init(&argc, &argv);

initial_replacement_builder = gtk_builder_new();
gtk_builder_add_from_file (initial_replacement_builder, "glade/initial_replacement_window.glade", NULL);

initial_replacement_window = GTK_WIDGET(gtk_builder_get_object(initial_replacement_builder, "initial_replacement_window"));

entry_initial_price= GTK_WIDGET(gtk_builder_get_object(initial_replacement_builder, "initial_price_entry"));
entry_useful_life=GTK_WIDGET(gtk_builder_get_object(initial_replacement_builder, "useful_life_entry"));
entry_project_term=GTK_WIDGET(gtk_builder_get_object(initial_replacement_builder, "project_term_entry"));

file_chooser = GTK_WIDGET(gtk_builder_get_object(initial_replacement_builder, "load_file_button"));

gtk_builder_connect_signals(initial_replacement_builder,NULL);

provider = gtk_css_provider_new ();
display = gdk_display_get_default ();
screen = gdk_display_get_default_screen (display);
gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

gsize bytes_written, bytes_read;

const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

GError *error = 0;

gtk_css_provider_load_from_path (provider,g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);

g_object_unref(initial_replacement_builder);

gtk_widget_show_all(initial_replacement_window);
gtk_main();

return 0;



}
