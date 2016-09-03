#include <gtk/gtk.h>
#include <stdbool.h>
#include <ctype.h>
#define MAXINPUT 100
#define INF 99999

int object_number=0;
int knapsack_capacity=0;
GtkWidget *entry_object_number;
GtkWidget *entry_knapsack_capacity;
GtkWidget       *warning_window;
GtkFileChooser *file_chooser;
char string_buffer[25];
int charPos,numbOfObj;
char **column_names;
int **matriz_datos;


int *global_values;
int *global_weights;
int *global_quantity;

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

void create_solution_table(){

}

void solve_knapsack_problem(GtkWidget *widget, gpointer   data){
	GtkWidget  *entrada;
	entrada = gtk_entry_new();
	gchar* entrance;
	entrance = calloc(1, 500*sizeof(gchar));
	int val[] = {60, 100, 120, 45};
	int wt[] = {10, 20, 30,78};
	int  W = 50;
	int n = sizeof(val)/sizeof(val[0]);
	printf("el misterioso %d\n", n);

	global_values = (int) calloc(numbOfObj, sizeof(int));
	global_weights = (int) calloc(numbOfObj, sizeof(int));
	global_quantity = (int) calloc(numbOfObj, sizeof(int));

	int fila, columna,value =0;
	int i = 0;
	int j = 0;
	int k = 0;
	for(columna = 1; columna <4; columna++){
		for(fila = 1; fila <= numbOfObj; fila++){
			entrada = gtk_grid_get_child_at (data, columna, fila);
		//	printf("Entrada[%d][%d] = %s\n",columna, fila, gtk_entry_get_text(entrada));
			if(columna == 1){
				printf("Entrada[%d][%d] = %s\n",columna, fila, gtk_entry_get_text(entrada));
				// value = atoi(gtk_entry_get_text(entrada));
				// printf("Value = %d\n",value);
				g_stpcpy(entrance,gtk_entry_get_text(entrada));
				global_quantity[i]= atoi(entrance);
				i++;
			}
			else if(columna == 2){
				printf("Entrada[%d][%d] = %s\n",columna, fila, gtk_entry_get_text(entrada));
				g_stpcpy(entrance,gtk_entry_get_text(entrada));
				global_weights[j] = atoi(entrance);
				j++;
			}
			else if(columna == 3){
				printf("Entrada[%d][%d] = %s\n",columna, fila, gtk_entry_get_text(entrada));
				g_stpcpy(entrance,gtk_entry_get_text(entrada));
				global_values[k] = atoi(entrance);
				k++;
			}
		}
	}
	//
	// Prueba
	printf("GLOBAL QUANTITIES\n");
	for(i = 0; i < numbOfObj; i++) printf("%d\n",global_quantity[i]);
	printf("GLOBAL WEIGHTS\n");
	for(i = 0; i < numbOfObj; i++) printf("%d\n",global_weights[i]);
	printf("GLOBAL VALUES\n");
	for(i = 0; i < numbOfObj; i++) printf("%d\n",global_values[i]);
}


void calculate_knapsack_problem(GtkWidget *widget, gpointer   data){
  // GtkWidget  *entrada;
  // gchar* entrance;
  // entrada = gtk_entry_new();
  // int i,j,k, fila, columna;

	// //Necesitamos dos matrices, una de datos y otra de columnas
	// global_column_names =  calloc(numbOfObj, 500*sizeof(char));
	// //alojamos la memoria para cada espacio del char
	// for (i = 0; i < numbOfObj; ++i) {
	// 		global_column_names[i] = (char *)malloc(500);
	// }
	//
	// global_values = calloc(numbOfObj,sizeof(int));
	// global_weights = calloc(numbOfObj,sizeof(int));
	// global_quantity =calloc(numbOfObj,sizeof(int));

	int val[] = {60, 100, 120, 45};
	int wt[] = {10, 20, 30,78};
	int  W = 50;
	int n = sizeof(val)/sizeof(val[0]);
	printf("el misterioso %d", n);


	// //alojamos memoria de la matriz de datos
	// global_matriz_datos = calloc(numbOfObj, 1+sizeof(int*));
	// for (i = 0; i < 3; ++i) {
	// 		global_matriz_datos[i] = calloc(3,sizeof(int));
	// }

  //printf("Entro al primer FOR\n" );
	// i=0;
  // for(j=1;j<numbOfObj+1;j++){
  //   for(k =0; k< 4;k++){
	//
	// 		//lleno la col 0
	// 		 if(k == 0){
	// 		  entrada = gtk_grid_get_child_at (data, k, j);
	// 		 	printf("Nombre del objeto : %s\n",gtk_entry_get_text(entrada));
	// 		 	strcpy(global_column_names[i],gtk_entry_get_text(entrada));
	// 		 	i++;
	// 		 }
	// 		else{
	// 			  entrada = gtk_grid_get_child_at (data, k, j);
	// 				printf("Dato[%d][%d] = %s\n",k, j, gtk_entry_get_text(entrada));
	// 				columna = k-1;
	// 				fila= j-1;
	// 				printf("fila %d , columna %d \n", fila, columna );
	// 				global_matriz_datos[fila][columna] = atoi(gtk_entry_get_text(entrada));
	// 		}
	// 	  }
	// 	}
	//
	// 	//llamar funcion que hace la prueba
	// 	//create_solution_table();
	// 	printf("Prueba de nombre de objetos\n");
	// 	for (i = 0; i<numbOfObj ; i++) printf("Objeto %d = %s\n",i, global_column_names[i] );
	//
	// 	printf("Prueba datos de la matriz\n" );
	// 	printSolution(global_matriz_datos, numbOfObj);
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
	g_signal_connect (button, "clicked", G_CALLBACK (solve_knapsack_problem), (gpointer) initial_table);
	gtk_container_add (GTK_CONTAINER (button_box), button);

	g_object_unref(initial_table_builder);

	gtk_widget_show_all(initial_table_window);
}

void printSolution(int  **dist, int numbOfObj)
{
    int i,j;
    for (i = 0; i < numbOfObj; i++)
    {
        for ( j = 0; j < 3; j++)
        {
          if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf ("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

void update_initial_table(int numbOfObj){
	printf("%s\n","1. Modifico la tabla inicial" );
  GtkWidget  *pt_entrada;
  pt_entrada = gtk_entry_new();
  char pt_cell_value[25];
  // modificar entrada de la P table
  int i, j;
  for(i = 0; i < numbOfObj ; i ++){
    for (j = 0; j<4; j++){
			//lleno la col 0
			if(j == 0){
				pt_entrada = gtk_grid_get_child_at (initial_table, j, i+1);
				gtk_entry_set_text (pt_entrada, column_names[i]);
			}
      pt_entrada = gtk_grid_get_child_at (initial_table, j+1, i+1);
      snprintf(pt_cell_value,5,"%d",matriz_datos[i][j]);
      gtk_entry_set_text (pt_entrada, pt_cell_value );
      //if (optimal_routes_mtx[i][j] != 0) gtk_widget_set_name(pt_entrada, "new_val");
      //printf("Entrada %d, %d = optimal_routes_mtx[%d][%d] = %d \n",j+1, i+1, i, j,optimal_routes_mtx[i][j] );
    }
  }
}

void kn_filechooserbutton_file_set_cb(){
  FILE *file;
  gchar *filename;
  int in_char;
	int fila, columna , k, i, j= 0;
  filename=gtk_file_chooser_get_filename (file_chooser);
  numbOfObj=getObjectsQuantity(filename);
	knapsack_capacity= gtk_entry_get_text (entry_knapsack_capacity);
	if((!is_number(knapsack_capacity)) || ( strcmp(gtk_entry_get_text (entry_knapsack_capacity), "") ==0)){
		create_warning_window("Debe ingresar una capacidad de la mochila valida.");
	}
	else{
		printf("la capacidad del knapscak es de %d\n", knapsack_capacity);
		if(numbOfObj< 3){
			create_warning_window("La cantidad de objetos debe ser mayor a 3.");
		}
		else{
			printf("El numero de filas es:%d\n",numbOfObj);

			//Necesitamos dos matrices, una de datos y otra de columnas
			column_names =  calloc(numbOfObj, 500*sizeof(gchar));
			//alojamos la memoria para cada espacio del char
			for (i = 0; i < numbOfObj; ++i) {
					column_names[i] = (char *)malloc(500);
			}

			//alojamos memoria de la matriz de datos
			matriz_datos = calloc(numbOfObj, 1+sizeof(int*));
			for (i = 0; i < numbOfObj; ++i) {
					matriz_datos[i] = calloc(numbOfObj,sizeof(int));
			}

			char **matriz_datos_iniciales = calloc(numbOfObj*4, 500*sizeof(char));
			//alojamos la memoria para cada espacio del char
			for (i = 0; i < numbOfObj*4; ++i) {
					matriz_datos_iniciales[i] = (char *)malloc(500);
			}
			create_entry_window();

			file = fopen( filename, "r" );
			if(file){
				clear_token_buffer();
				while ((in_char = getc(file)) != EOF){
						if((in_char == '|')|| (in_char == '\n')){
								strcpy(matriz_datos_iniciales[k], string_buffer);
								clear_token_buffer();
								k++;
						}
						else{
							buffer_char(in_char);
						}
				}
				printf("ya voy por AQUI\n" );
			 }
			else{
					printf("%s\n","Error al abrir el archivo" );
				}
			//VAmos a llenar las columnas


			fila, columna,i = 0;
			for (k = 0; k< numbOfObj*4 ; k++){
				if(k % 4  == 0){
					strcpy(column_names[i],matriz_datos_iniciales[k]);
					columna = 0;
					i++;
					if(k !=0){
						fila++;
					}
				}
				else{
					//llenamos la matriz
					printf("matriz_datos[%d][%d] = %d\n", fila, columna, atoi(matriz_datos_iniciales[k]) );
					matriz_datos[fila][columna] = atoi(matriz_datos_iniciales[k]);
					columna++;
				}
			}

			// printf("Prueba de nombre de objetos\n");
			// for (i = 0; i<numbOfObj ; i++) printf("Objeto %d = %s\n",i, column_names[i] );
			//
			// printf("Prueba datos de la matriz\n" );
			// printSolution(matriz_datos, numbOfObj);

			update_initial_table(numbOfObj);
		}

	}

}



void btn_aceptar_clicked_cb(){
  gchar* message;
  message = calloc(1, 500*sizeof(gchar));


  object_number = gtk_entry_get_text (entry_object_number);
  knapsack_capacity=gtk_entry_get_text (entry_knapsack_capacity);
  if(!is_number(object_number) || !is_number(knapsack_capacity)){

    create_warning_window("Tipo de dato inválido");

  }
	else if(atoi(object_number)<3){
		create_warning_window("La cantidad de objetos debe ser mayor a 3.");
	}
	else if(strcmp(gtk_entry_get_text (entry_knapsack_capacity), "") ==0){
		create_warning_window("Debe ingresar una capacidad de la mochila valida.");
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
