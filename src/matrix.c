#include <gtk/gtk.h>
#include <stdbool.h>

GtkWidget *number_matrix_entry, *warning_window;
int matrix_number,charPos;
GtkFileChooser *file_chooser;
bool f_manual = FALSE;
GtkBuilder      *file_saver_builder;
GtkWidget       *file_saver_window;
char file_name_buffer[25];

GtkBuilder      *initial_MATRIX_table_builder;
GtkWidget       *initial_MATRIX_table_window;
GtkWidget       *initial_MATRIX_table;
GtkWidget ***entrada;
char string_buffer[25];

char **matrix_names;
char **column_names;
int *global_n_values;
int *global_m_values;
//Matriz intermedia de datos leidos del archivo
int **matriz_datos;


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

void clear_file_buffer(char pfile_value[5]){
	memset(pfile_value, 0, 5);

}

void on_btn_save_filename_clicked(GtkWidget *widget, gpointer   data){
  printf("voya a guaradr\n" );
  strcpy(file_name_buffer, gtk_entry_get_text (data));
  strcat(file_name_buffer, ".txt");
  if (strcmp(gtk_entry_get_text (data), "") == 0){
    create_warning_window("Debe ingresar un nombre valido.");
  }
  else{
    printf("El nombre del archivo es: %s\n", file_name_buffer );
    //Archivo en el que se graba información
    FILE * output;
    int j, i;
    char file_value[20];
    output= fopen( file_name_buffer, "w+");

		printf(" HEY\n");

		for(j=0;j<matrix_number;j++){

			fprintf(output, "%s|",column_names[j]);
			snprintf(file_value,20,"%d|",global_n_values[j]);
      fprintf(output, "%s",file_value);
      snprintf(file_value,20,"%d",global_m_values[j]);
			fprintf(output, "%s",file_value);
			clear_file_buffer(file_value);
			fprintf(output, "\n");
		}



    fclose(output);
    gtk_widget_destroy (file_saver_window);

  }
}

void writeFile(){

  GtkWidget *button;
  GtkWidget *entry;


  file_saver_builder = gtk_builder_new();
  gtk_builder_add_from_file (file_saver_builder, "glade/file_saver_window.glade", NULL);

  file_saver_window = GTK_WIDGET(gtk_builder_get_object(file_saver_builder, "file_saver_window"));
  gtk_builder_connect_signals(file_saver_builder,NULL);

  entry = GTK_WIDGET(gtk_builder_get_object(file_saver_builder, "file_name"));

  button =  GTK_WIDGET(gtk_builder_get_object(file_saver_builder, "btn_save_filename"));
  g_signal_connect (button, "clicked", G_CALLBACK (on_btn_save_filename_clicked), (gpointer) entry);


  g_object_unref(file_saver_builder);

  gtk_widget_show_all(file_saver_window);


}

void buffer_char(char c){
	string_buffer[charPos++] = c;
}

void clear_token_buffer(){
	memset(string_buffer, 0, 25);
	charPos = 0;
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

int getObjectsQuantity( gchar *pFilename){

	int row_numb=0;
	int in_ch;
	FILE *pfile;
	pfile = fopen(pFilename, "r");

	while ((in_ch = getc(pfile)) != EOF){
		if(in_ch == '\n'){
			row_numb++;
		}
	}
  //fclose(pFilename);

	return row_numb;
}

void update_initial_table(int pMatrix_number){
	printf("%s\n","1. Estoy llenando la tabla inicial" );
  GtkWidget  *pt_entrada;
  pt_entrada = gtk_entry_new();
  char pt_cell_value[25];
  // modificar entrada de la P table
  int i, j;
	int w = 0;
  for(i = 1; i < pMatrix_number+1 ; i ++){ //fila
    for (j = 0; j<3; j++){ //columna
			pt_entrada = gtk_grid_get_child_at (initial_MATRIX_table, j, i);
      if (j == 0){ //matrix names

				gtk_entry_set_text (pt_entrada, matrix_names[i-1]);
			}
			else{
        //snprintf(pt_cell_value,5,"%d",matriz_datos[i-1]);

        gtk_entry_set_text (pt_entrada, matriz_datos[w]);
        w++;
				//snprintf(pt_cell_value,10,"%.4f",weights[i-1]);
				//printf("%s\n",pt_cell_value );
				//gtk_entry_set_text (pt_entrada, pt_cell_value );

			}

    }// end j FOR
  }//end i FOR

}

void solve_MATRIX_problem(GtkWidget *widget, gpointer   data){
  printf("%s\n","Click en Procesar" );
  int fila, columna,value, i;
  GtkWidget  *entrada;
  entrada = gtk_entry_new();
  gchar* entrance;
  entrance = calloc(1, 500*sizeof(gchar));

  global_n_values= (int) calloc(matrix_number, sizeof(int));
  global_m_values = (int) calloc(matrix_number, sizeof(int));

  column_names = calloc(matrix_number, 500*sizeof(char));
  //alojamos la memoria para cada espacio del char
  for (i = 0; i < matrix_number; ++i) {
     column_names[i] = (char *)malloc(500);
  }

  //alojamos la memoria para cada espacio

  int k = 0;
  int w = 0;
  int d = 0;
  for(columna = 0; columna <3; columna++){
    for(fila = 1; fila <= matrix_number; fila++){
      entrada = gtk_grid_get_child_at (data, columna, fila);
        if(columna == 0){
          g_stpcpy(entrance,gtk_entry_get_text(entrada));
          //printf("k = %d\n",k );
          strcpy(column_names[k],entrance);
          k++;

        }
        else if(columna == 1){
          g_stpcpy(entrance,gtk_entry_get_text(entrada));
          global_n_values[d]=atoi(entrance);
          d++;
        }
        else{
          g_stpcpy(entrance,gtk_entry_get_text(entrada));
          global_m_values[w]=atoi(entrance);
          w++;


        }
    }
  }
  writeFile();


}

//Manual entry of weights and keys
void create_entry_window(){
	int k,j;
	GtkWidget       *button;
	GtkWidget       *button_box;
  GtkWidget *matrix_entry_scrolledwindow;

	initial_MATRIX_table_builder= gtk_builder_new();
	gtk_builder_add_from_file (initial_MATRIX_table_builder, "glade/initial_entry_win_MATRIX .glade", NULL);

	initial_MATRIX_table_window = GTK_WIDGET(gtk_builder_get_object(initial_MATRIX_table_builder, "matrix_entry_window"));
	gtk_builder_connect_signals(initial_MATRIX_table_builder, NULL);

  matrix_entry_scrolledwindow = GTK_WIDGET(gtk_builder_get_object(initial_MATRIX_table_builder, "matrix_entry_scrolledwindow"));
	gtk_builder_connect_signals(initial_MATRIX_table_builder, NULL);

	initial_MATRIX_table= gtk_grid_new();
	gtk_grid_set_row_spacing (GTK_GRID (initial_MATRIX_table), 2);
	gtk_container_add (GTK_CONTAINER (matrix_entry_scrolledwindow), initial_MATRIX_table);

	//dynamic grid
	entrada=calloc(matrix_number+1,sizeof(GtkWidget**));
	for(j = 0; j < matrix_number+1; j++){
		entrada[j]=calloc(matrix_number+1,sizeof(GtkWidget*));
	}

	//loops to create grid
	for(j=0;j<matrix_number+1;j++){ //filas
		for(k =0; k< 3;k++){ //columnas
			entrada[j][k]= gtk_entry_new();
			gtk_grid_attach (GTK_GRID (initial_MATRIX_table),entrada[j][k] , k, j, 1, 1);
			//printf("Puse la pos[%d][%d]\n",j,k );

		}
	}

	gtk_entry_set_text (entrada[0][0],"Matriz");
	gtk_widget_set_sensitive (entrada[0][0], FALSE);
	gtk_widget_set_name(entrada[0][0], "column_name");

	gtk_entry_set_text (entrada[0][1],"N");
	gtk_widget_set_sensitive (entrada[0][1], FALSE);
	gtk_widget_set_name(entrada[0][1], "column_name");

  gtk_entry_set_text (entrada[0][2],"M");
	gtk_widget_set_sensitive (entrada[0][2], FALSE);
	gtk_widget_set_name(entrada[0][2], "column_name");


	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach (GTK_GRID (initial_MATRIX_table),button_box ,0.5, j, 2, 2);

	button = gtk_button_new_with_label ("Procesar");
	g_signal_connect (button, "clicked", G_CALLBACK (solve_MATRIX_problem), (gpointer) initial_MATRIX_table);
	gtk_container_add (GTK_CONTAINER (button_box), button);

	g_object_unref(initial_MATRIX_table_builder);

	gtk_widget_show_all(initial_MATRIX_table_window);
}


void on_ok_key_button_clicked(){


  if(strcmp(gtk_entry_get_text (number_matrix_entry), "") ==0 ){
    create_warning_window("Los campos no pueden ser vacíos");
  }
  else if(!is_number(gtk_entry_get_text (number_matrix_entry)) ){

    create_warning_window("El tipo de dato debe ser entero");

  } else{
    matrix_number=atoi(gtk_entry_get_text (number_matrix_entry));
    printf("El valor digitado es:%d\n",matrix_number);
    f_manual = TRUE;
	  create_entry_window();
  }


}

void filechooser_matrix_file_set_cb(){
  FILE *file;
  gchar *filename;
  int ind,in_char,i=0;
  filename=gtk_file_chooser_get_filename (file_chooser);

  matrix_number =getObjectsQuantity(filename);
  printf("Numero de lineas:%d\n", matrix_number);

  matrix_names =  calloc(matrix_number, 500*sizeof(gchar));
  //alojamos la memoria para cada espacio del char
  for (i = 0; i < matrix_number; ++i) {
      matrix_names[i] = (char *)malloc(500);
  }
  //global_n_values= (int) calloc(matrix_number, sizeof(int));
  //global_m_values = (int) calloc(matrix_number, sizeof(int));

  char **matriz_datos_iniciales = calloc(matrix_number*3, 500*sizeof(char));
  //alojamos la memoria para cada espacio del char
  for (i = 0; i < matrix_number*3; ++i) {
    matriz_datos_iniciales[i] = (char *)malloc(500);
  }

  matriz_datos = calloc(matrix_number*2, 1+sizeof(int*));
  for (i = 0; i < matrix_number*2; ++i) {
      matriz_datos[i] = calloc(matrix_number*2,sizeof(int));
  }


  file = fopen( filename, "r" );
  if(file){
  clear_token_buffer();
  while ((in_char = getc(file)) != EOF){

      if((in_char == '|')|| (in_char == '\n')){
        //printf("Elemento:%s\n",string_buffer );
        strcpy(matriz_datos_iniciales[ind],string_buffer);
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

  int w,n = 0;
  for(i = 0;i<matrix_number*3;i++){

    //printf("With %d the result is %d\n",i,reminder );

    if(i%3 == 0){
      //printf("with %d Nombre:%s\n",i, matriz_datos_iniciales[i]);
      strcpy(matrix_names[n],matriz_datos_iniciales[i]);
      n++;
    }
    else{

      //printf("with %d Otro:%s\n",i, matriz_datos_iniciales[i]);
      strcpy(matriz_datos[w],matriz_datos_iniciales[i]);
      w++;

    }

  }
  //printf("%s\n","Voy a imprimir lo que tiene matriz de datos" );
  //for(i=0;i<matrix_number*2;i++){
    //printf("Elemento:%s\n",matriz_datos[i] );
  //}
  create_entry_window();
	update_initial_table(matrix_number);
}


int main(int argc, char *argv[]){

  //It creates the window that requests the number of keys

  GtkBuilder      *matrix_builder;
  GtkWidget       *matrix_window;

    /*--- CSS -----------------*/
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;
  /*-------------------------*/

  gtk_init(&argc, &argv);

  matrix_builder = gtk_builder_new();
  gtk_builder_add_from_file (matrix_builder, "glade/matrix_initial_window.glade", NULL);

  matrix_window = GTK_WIDGET(gtk_builder_get_object(matrix_builder, "matrix_initial_window"));

  number_matrix_entry = GTK_WIDGET(gtk_builder_get_object(matrix_builder, "matrix_number_entry"));

  file_chooser = GTK_WIDGET(gtk_builder_get_object(matrix_builder, "filechooser_matrix"));

  gtk_builder_connect_signals(matrix_builder,NULL);

  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

  GError *error = 0;

  gtk_css_provider_load_from_path (provider,g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),NULL);

  g_object_unref(matrix_builder);

  gtk_widget_show_all(matrix_window);
  gtk_main();

  return 0;

}
