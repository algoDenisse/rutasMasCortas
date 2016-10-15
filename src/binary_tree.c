#include <gtk/gtk.h>
#include <stdbool.h>

//Global variables
GtkWidget *number_keys_entry, *warning_window;
GtkFileChooser *file_chooser;
char string_buffer[25];

int number_keys,charPos;
bool f_manual = FALSE;

void buffer_char(char c){
	string_buffer[charPos++] = c;
}

void clear_token_buffer(){
	memset(string_buffer, 0, 25);
	charPos = 0;
}

void filechooser_btree_file_set_cb(){
  FILE *file;
  gchar *filename;
  int ind,in_char,i=0;

  char **matriz_datos_iniciales = calloc(2, 500*sizeof(char));
  //allocate memory
	for (i = 0; i < 2; ++i) {
		 matriz_datos_iniciales[i] = (char *)malloc(500);
	}


  filename=gtk_file_chooser_get_filename (file_chooser);
  file = fopen( filename, "r" );

  if(file){
    clear_token_buffer();
    while ((in_char = getc(file)) != EOF){

      if((in_char == '|')|| (in_char == '\n')){
        strcpy(matriz_datos_iniciales[ind], string_buffer);
        clear_token_buffer();
        ind++;
      }
      else{
        buffer_char(in_char);
      }

    }//end While
  }//end if file

  else{
      printf("%s\n","Error al abrir el archivo" );
    }


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
}

void on_ok_key_button_clicked(){


  if(strcmp(gtk_entry_get_text (number_keys_entry), "") ==0 ){
    create_warning_window("Los campos no pueden ser vacíos");
  }
  else if(!is_number(gtk_entry_get_text (number_keys_entry)) ){

    create_warning_window("El tipo de dato debe ser entero");

  } else{
    number_keys=atoi(gtk_entry_get_text (number_keys_entry));
    printf("El valor digitado es:%d\n",number_keys);
    f_manual = TRUE;
  }


}

int main(int argc, char *argv[]){

  //It creates the window that requests the number of keys

  GtkBuilder      *binary_tree_builder;
  GtkWidget       *binary_tree_window;

    /*--- CSS -----------------*/
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;
  /*-------------------------*/

  gtk_init(&argc, &argv);

  binary_tree_builder = gtk_builder_new();
  gtk_builder_add_from_file (binary_tree_builder, "glade/binary_tree_initial_window.glade", NULL);

  binary_tree_window = GTK_WIDGET(gtk_builder_get_object(binary_tree_builder, "binary_tree_initial_window"));

  number_keys_entry = GTK_WIDGET(gtk_builder_get_object(binary_tree_builder, "key_number_entry"));

  file_chooser = GTK_WIDGET(gtk_builder_get_object(binary_tree_builder, "filechooser_btree"));

  gtk_builder_connect_signals(binary_tree_builder,NULL);

  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

  GError *error = 0;

  gtk_css_provider_load_from_path (provider,g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);

  g_object_unref(binary_tree_builder);

  gtk_widget_show_all(binary_tree_window);
  gtk_main();

  return 0;

}
