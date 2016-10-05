#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>

GtkWidget *number_games, *ph_entry, *pr_entry, *partida, *warning_window;
GtkBuilder      *initial_series_deportivas_table_builder;
GtkWidget       *initial_series_deportivas_table_window;
GtkWidget       *initial_series_deportivas_table;

GtkBuilder      *file_saver_builder;
GtkWidget       *file_saver_window;
GtkFileChooser *file_chooser;

GtkWidget ***entrada;

int ph,pr,charPos,number_of_games,games_quantity, button_quantity=0;

double ph_double;
double qr_double;
double pr_double;
double qh_double;


char string_buffer[25];
bool f_manual = FALSE;
char file_name_buffer[25];

void buffer_char(char c){
	string_buffer[charPos++] = c;
}

void clear_token_buffer(){
	memset(string_buffer, 0, 25);
	charPos = 0;
}
void clear_file_buffer(char pfile_value[5]){
	memset(pfile_value, 0, 5);

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
void create_entry_window(){
	int k,j;
	GtkWidget       *button;
	GtkWidget       *button_box;
	initial_series_deportivas_table_builder= gtk_builder_new();
	gtk_builder_add_from_file (initial_series_deportivas_table_builder, "glade/initial_series_deportivas_table_window.glade", NULL);

	initial_series_deportivas_table_window = GTK_WIDGET(gtk_builder_get_object(initial_series_deportivas_table_builder, "initial_series_deportivas_table_window"));
	gtk_builder_connect_signals(initial_series_deportivas_table_builder, NULL);

	initial_series_deportivas_table= gtk_grid_new();
	gtk_grid_set_row_spacing (GTK_GRID (initial_series_deportivas_table), 2);
	gtk_container_add (GTK_CONTAINER (initial_series_deportivas_table_window), initial_series_deportivas_table);

	//dynamic grid
	entrada=calloc(button_quantity+1,sizeof(GtkWidget**));
	for(j = 0; j < button_quantity+1; j++  ){
		entrada[j]=calloc(button_quantity+1,sizeof(GtkWidget*));
	}

	//loops to create grid
	for(j=0;j<button_quantity+1;j++){ //filas
		for(k =0; k< 3;k++){ //columnas
			entrada[j][k]= gtk_toggle_button_new ();
			gtk_grid_attach (GTK_GRID (initial_series_deportivas_table),entrada[j][k] , k, j, 1, 1);
			//printf("Puse la pos[%d][%d]\n",j,k );
      // if((k==0) && (j!=0)){
      //   snprintf(intermediate_buffer,5,"%d",j);
      //   gtk_entry_set_text (entrada[j][k],intermediate_buffer);
      //   gtk_widget_set_sensitive (entrada[j][k], FALSE);
      // }

		}
	}



	// button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	// gtk_grid_attach (GTK_GRID (initial_equipment_table),button_box ,1, j, 2, 2);
  //
	// button = gtk_button_new_with_label ("Procesar");
	// g_signal_connect (button, "clicked", G_CALLBACK (solve_requipmentEquipment_problem), (gpointer) initial_equipment_table);
	// gtk_container_add (GTK_CONTAINER (button_box), button);

	g_object_unref(initial_series_deportivas_table_builder);

	gtk_widget_show_all(initial_series_deportivas_table_window);
//}

}

void on_load_file_button_file_set(){
  FILE *file;
  gchar *filename;
  int k, in_char;
  filename=gtk_file_chooser_get_filename (file_chooser);
  file = fopen( filename, "r" );
    if(file){
      clear_token_buffer();
      while ((in_char = getc(file)) != EOF){
          if((in_char == '|')|| (in_char == '\n')){
             printf("String buffer %s\n", string_buffer );
              clear_token_buffer();
              k++;
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

void on_create_button_clicked(){
  ph = atoi(gtk_entry_get_text (ph_entry));
  pr = atoi(gtk_entry_get_text (pr_entry));
  number_of_games = atoi(gtk_entry_get_text (number_games));

 if(strcmp(gtk_entry_get_text (ph_entry), "") ==0 || strcmp(gtk_entry_get_text (pr_entry), "") ==0 || strcmp(gtk_entry_get_text (number_games), "") ==0 ){
   create_warning_window("Los campos no pueden ser vacíos");
 }
 else if(!is_number(gtk_entry_get_text (ph_entry)) || !is_number(gtk_entry_get_text (pr_entry)) || !is_number(gtk_entry_get_text (number_games))){

   create_warning_window("El tipo de dato debe ser entero");

 }
  else{
    f_manual = TRUE;
    ph_double = (double) ph / 100;
    qr_double = 1 - ph_double;
    pr_double = (double) pr / 100;
    qh_double = 1 - pr_double;
    printf("Mi probabilidad en casa %f\n", ph_double);
    printf("Probabilidad de equipo contrario on the road %f\n", qr_double);
    printf("Mi probabilidad on the road %f\n", pr_double);
    printf("Probabilidad de equipo contrario en casa %f\n", qh_double);
    printf("JUEGOS TOTALES %d\n", number_of_games);
    games_quantity = (number_of_games + 1) /2;
    button_quantity = games_quantity +2;
    create_entry_window();
 }
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

file_chooser = GTK_WIDGET(gtk_builder_get_object(initial_series_builder, "load_file_button"));

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
