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
//char ***cell_values;

GtkBuilder      *initial_table_builder;
GtkWidget       *initial_table_window;
GtkWidget       *initial_table;
GtkWidget ***entrada;

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
  int i,j,k;

    //printf("Entro al primer FOR\n" );
  for(j=1;j<numbOfObj+1;j++){
    for(k =0; k< 4;k++){
      //printf("Entro al segundo FOR\n" );
      entrada = gtk_grid_get_child_at (data, k, j);
      printf("%s\n",gtk_entry_get_text(entrada));

  }
}

}
void kn_filechooserbutton_file_set_cb(){
  FILE *file;
  gchar *filename;
  int in_char;
	int fila, columna , k, i= 0;



  filename=gtk_file_chooser_get_filename (file_chooser);
  numbOfObj=getObjectsQuantity(filename);
  printf("El numero de filas es:%d\n",numbOfObj);

	//GUardar espacios de la matriz de los datos
	// cell_values = calloc(numbOfObj, sizeof(gchar**));
	// for(k = 0; k < numbOfObj; k++) {
	// 		cell_values[k] = calloc(numbOfObj, sizeof(gchar*));
	// 		for(i = 0; i < numbOfObj; i++) {
	// 				cell_values[k][i] = (gchar *)malloc(500);
	// 			}
	// }

	create_entry_window();

	//g_stpcpy(cell_values[0][0], "HOla aaaa");

  file = fopen( filename, "r" );
  if(file){
    printf("%s\n","Abri el archivo" );
    //clear_token_buffer();
    // while ((in_char = getc(file)) != EOF){
    //   if(in_char == '|'){
		// 		if(columna == 4){
		// 			fila ++;
		// 			columna =0;
		// 		}
		//
    //     printf("%s\n", string_buffer );
		// 		printf("fila %d columna %d\n", fila, columna );
		// 	//	g_stpcpy(cell_values[fila][columna], string_buffer);
		// 		columna ++;
		//
    //     clear_token_buffer();
    //   }
    //   else{
    //     buffer_char(in_char);
		//
    //   }
    // }
   }

else{
    printf("%s\n","Error al abrir el archivo" );
  }

	// printf("RESULTADO DE LA MATRIZ DE DATOS\n" );
	// for (fila = 0; fila<numbOfObj ; fila++){
	// 	for(columna = 0; columna< 4; columna++ ){
	// 		printf("entrada[%d][%d] = %s\n",fila, columna, cell_values[fila][columna]);
	// 	}
	// }
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

void create_entry_window(){
	int k,j;
	GtkWidget       *button;
	GtkWidget       *button_box;
	initial_table_builder= gtk_builder_new();
	gtk_builder_add_from_file (initial_table_builder, "glade/initial_table_window.glade", NULL);

	initial_table_window = GTK_WIDGET(gtk_builder_get_object(initial_table_builder, "initial_table_window"));
	gtk_builder_connect_signals(initial_table_builder, NULL);

	initial_table= gtk_grid_new();
	gtk_grid_set_row_spacing (GTK_GRID (initial_table), 2);
	gtk_container_add (GTK_CONTAINER (initial_table_window), initial_table);

	//dynamic grid
	entrada=calloc(numbOfObj+1,sizeof(GtkWidget**));
	for(j = 0; j < numbOfObj+1; j++){
		entrada[j]=calloc(numbOfObj+1,sizeof(GtkWidget*));
	}

	//loops to create grid
	for(j=0;j<numbOfObj+1;j++){ //filas
		for(k =0; k< 4;k++){ //columnas
			entrada[j][k]= gtk_entry_new();
			gtk_grid_attach (GTK_GRID (initial_table),entrada[j][k] , k, j, 1, 1);
			//printf("Puse la pos[%d][%d]\n",j,k );

		}
	}

	gtk_entry_set_text (entrada[0][0],"Objeto");
	gtk_widget_set_sensitive (entrada[0][0], FALSE);
	gtk_widget_set_name(entrada[0][0], "column_name");

	gtk_entry_set_text (entrada[0][1],"Cantidad");
	gtk_widget_set_sensitive (entrada[0][1], FALSE);
	gtk_widget_set_name(entrada[0][1], "column_name");

	gtk_entry_set_text (entrada[0][2],"Costo");
	gtk_widget_set_sensitive (entrada[0][2], FALSE);
	gtk_widget_set_name(entrada[0][2], "column_name");

	gtk_entry_set_text (entrada[0][3],"Valor");
	gtk_widget_set_sensitive (entrada[0][3], FALSE);
	gtk_widget_set_name(entrada[0][3], "column_name");

	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach (GTK_GRID (initial_table),button_box ,1, j, 2, 2);

	button = gtk_button_new_with_label ("Procesar");
	g_signal_connect (button, "clicked", G_CALLBACK (calculate_knapsack_problem), (gpointer) initial_table);
	gtk_container_add (GTK_CONTAINER (button_box), button);

	g_object_unref(initial_table_builder);

	gtk_widget_show_all(initial_table_window);
}

void btn_aceptar_clicked_cb(){
  gchar* message;
  message = calloc(1, 500*sizeof(gchar));


  object_number = gtk_entry_get_text (entry_object_number);
  knapsack_capacity=gtk_entry_get_text (entry_knapsack_capacity);
  if(!is_number(object_number) || !is_number(knapsack_capacity)){

    create_warning_window("Tipo de dato inválido");

  }
  else{
		numbOfObj = atoi(gtk_entry_get_text (entry_object_number));
		create_entry_window();
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
