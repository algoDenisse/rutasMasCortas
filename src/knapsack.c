#include <gtk/gtk.h>
#include <stdbool.h>
#define MAXINPUT 100

int object_number=0;
int knapsack_capacity=0;
GtkWidget *entry_object_number;
GtkWidget *entry_knapsack_capacity;
GtkWidget       *warning_window;
GtkFileChooser *file_chooser;
char string_buffer[25];
int charPos,numbOfObj;

void buffer_char(char c){
	string_buffer[charPos++] = c;
}

void clear_token_buffer(){
	memset(string_buffer, 0, 25);
	charPos = 0;
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

int getObjectsQuantity( gchar *pFilename){
	int row_numb=0;
	int in_ch;
	FILE *pfile;
	pfile = fopen( pFilename, "r" );
	while ((in_ch = getc(pfile)) != EOF){
		if(in_ch == '\n'){
			row_numb++;
		}
	}
  //fclose(pFilename);
	return row_numb;

}

void calculate_knapsack_problem(GtkWidget *widget, gpointer   data){
  GtkWidget  *entrada;
  gchar* entrance;
  entrada = gtk_entry_new();
  int i;

  int j,k;
  i=0;

    //printf("Entro al primer FOR\n" );
  for(j=1;j<numbOfObj+1;j++){
    for(k =0; k< 4;k++){
      //printf("Entro al segundo FOR\n" );
      entrada = gtk_grid_get_child_at (data, k, j);
      //g_stpcpy(entrance,gtk_entry_get_text(entrada));
      printf("%s\n",gtk_entry_get_text(entrada));

  }
}

}
void kn_filechooserbutton_file_set_cb(){
  FILE *file;
  gchar *filename;
  int in_char,nobjects;

  filename=gtk_file_chooser_get_filename (file_chooser);
  nobjects=getObjectsQuantity(filename);
  printf("El numero de filas es:%d\n",nobjects);

  file = fopen( filename, "r" );
  if(file){
    printf("%s\n","Abri el archivo" );
    //clear_token_buffer();

    while ((in_char = getc(file)) != EOF){
      if(in_char == '|'){

        printf("%s\n", string_buffer );
        clear_token_buffer();
      }
      else{
        buffer_char(in_char);

      }
    }
  }

  else{
    printf("%s\n","Error al abrir el archivo" );
  }


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
void btn_aceptar_clicked_cb(){

  GtkBuilder      *initial_table_builder;
  GtkWidget       *initial_table_window;
  GtkWidget       *table;
  GtkWidget       *button;
  GtkWidget       *button_box;
  gchar* message;
  message = calloc(1, 500*sizeof(gchar));
  int k,j;

  object_number = gtk_entry_get_text (entry_object_number);
  knapsack_capacity=gtk_entry_get_text (entry_knapsack_capacity);
  if(!is_number(object_number) || !is_number(knapsack_capacity)){

    create_warning_window("Tipo de dato inválido");

  }
  else{


      initial_table_builder= gtk_builder_new();
      gtk_builder_add_from_file (initial_table_builder, "glade/initial_table_window.glade", NULL);

      initial_table_window = GTK_WIDGET(gtk_builder_get_object(initial_table_builder, "initial_table_window"));
      gtk_builder_connect_signals(initial_table_builder, NULL);

      table= gtk_grid_new();
      gtk_grid_set_row_spacing (GTK_GRID (table), 2);
      gtk_container_add (GTK_CONTAINER (initial_table_window), table);

      //dynamic grid
      GtkWidget ***entrada;
      numbOfObj = atoi(gtk_entry_get_text (entry_object_number));

      entrada=calloc(numbOfObj+1,sizeof(GtkWidget**));
      for(j = 0; j < numbOfObj+1; j++){
        entrada[j]=calloc(numbOfObj+1,sizeof(GtkWidget*));
      }

      //loops to create grid
      for(k =0; k< 4;k++){ //columnas
        for(j=0;j<numbOfObj+1;j++){ //filas

          entrada[k][j]= gtk_entry_new();
          gtk_grid_attach (GTK_GRID (table),entrada[k][j] , k, j, 1, 1);


        }
      }

      gtk_entry_set_text (entrada[0][0],"Objeto");
      gtk_widget_set_sensitive (entrada[0][0], FALSE);
      gtk_widget_set_name(entrada[0][0], "column_name");

      gtk_entry_set_text (entrada[1][0],"Cantidad");
      gtk_widget_set_sensitive (entrada[1][0], FALSE);
      gtk_widget_set_name(entrada[1][0], "column_name");

      gtk_entry_set_text (entrada[2][0],"Costo");
      gtk_widget_set_sensitive (entrada[2][0], FALSE);
      gtk_widget_set_name(entrada[2][0], "column_name");

      gtk_entry_set_text (entrada[3][0],"Valor");
      gtk_widget_set_sensitive (entrada[3][0], FALSE);
      gtk_widget_set_name(entrada[3][0], "column_name");

      button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
      gtk_grid_attach (GTK_GRID (table),button_box ,1, j, 2, 2);

      button = gtk_button_new_with_label ("Procesar");
      g_signal_connect (button, "clicked", G_CALLBACK (calculate_knapsack_problem), (gpointer) table);
      gtk_container_add (GTK_CONTAINER (button_box), button);

      g_object_unref(initial_table_builder);

      gtk_widget_show_all(initial_table_window);

  }


}

int main(int argc, char *argv[]){

  GtkBuilder      *initial_knapsack_builder;
  GtkWidget       *initial_knapsack_window;

  /*--- CSS -----------------*/
GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;
/*-------------------------*/

gtk_init(&argc, &argv);

initial_knapsack_builder = gtk_builder_new();
gtk_builder_add_from_file (initial_knapsack_builder, "glade/initial_knapsack_window.glade", NULL);

initial_knapsack_window = GTK_WIDGET(gtk_builder_get_object(initial_knapsack_builder, "initial_knapsack_window"));

entry_object_number= GTK_WIDGET(gtk_builder_get_object(initial_knapsack_builder, "entry_object_number"));
entry_knapsack_capacity=GTK_WIDGET(gtk_builder_get_object(initial_knapsack_builder, "entry_knapsack_capacity"));
file_chooser = GTK_WIDGET(gtk_builder_get_object(initial_knapsack_builder, "kn_filechooserbutton"));

gtk_builder_connect_signals(initial_knapsack_builder,NULL);

provider = gtk_css_provider_new ();
display = gdk_display_get_default ();
screen = gdk_display_get_default_screen (display);
gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

gsize bytes_written, bytes_read;

const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

GError *error = 0;

gtk_css_provider_load_from_path (provider,g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);

g_object_unref(initial_knapsack_builder);

gtk_widget_show_all(initial_knapsack_window);
gtk_main();

return 0;



}
