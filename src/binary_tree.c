#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//Global variables
GtkWidget *number_keys_entry, *warning_window;
GtkBuilder      *file_saver_builder;
GtkWidget       *file_saver_window;
GtkFileChooser *file_chooser;
char string_buffer[25];
char file_name_buffer[25];
int number_keys,charPos, winnerK= 0;
bool f_manual = FALSE;

//For keys we will need to arrays / keys can be strings or number_keys
char **key_as_string;
int  **key_as_int;
//Array for weights
float *weights;
double **matriz_solution;
int **r_table_mtx;
float *probabilities;

GtkBuilder      *initial_BTREE_table_builder;
GtkWidget       *initial_BTREE_table_window;
GtkWidget       *initial_BTREE_table;
GtkWidget ***entrada;


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

		for(j=0;j<number_keys;j++){

			fprintf(output, "%s|",key_as_string[j]);
			snprintf(file_value,20,"%.4f",weights[j]);
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
//it fills initial table with values in the file
void update_initial_table(int pKey_number){
	printf("%s\n","1. Estoy llenando la tabla inicial" );
  GtkWidget  *pt_entrada;
  pt_entrada = gtk_entry_new();
  char pt_cell_value[25];
  // modificar entrada de la P table
  int i, j;
	int w = 0;
  for(i = 1; i < pKey_number+1 ; i ++){
    for (j = 0; j<2; j++){
			pt_entrada = gtk_grid_get_child_at (initial_BTREE_table, j, i);
      if (j == 0){
				//printf("Escribire: %s\n",key_as_string[i-1]);
				gtk_entry_set_text (pt_entrada, key_as_string[i-1]);
			}
			else{
				snprintf(pt_cell_value,10,"%.4f",weights[i-1]);
				printf("%s\n",pt_cell_value );
				gtk_entry_set_text (pt_entrada, pt_cell_value );

			}

    }// end j FOR
  }//end i FOR

}

void printSolution(double **dist)
{
    int i,j;
    for (i = 0; i < number_keys+1; i++)
    {
        for ( j = 0; j < number_keys+1; j++)
        {
            printf ("%10.4f", dist[i][j]);
        }
        printf("\n");
    }
}

void printSolution_int(int **dist)
{
    int i,j;
    for (i = 0; i < number_keys+1; i++)
    {
        for ( j = 0; j < number_keys+1; j++)
        {
            printf ("%10d", dist[i][j]);
        }
        printf("\n");
    }
}

void sort_keys()
{
    int i, j;
    char temp[70];
  float temp_weight;

    for(i=0; i<number_keys-1; ++i){
        for(j=i+1; j<number_keys; ++j)
        {

            if(strcmp(key_as_string[i], key_as_string[j])>0)
            {
                strcpy(temp, key_as_string[i]);
        temp_weight = weights[i];
                strcpy(key_as_string[i], key_as_string[j]);
        weights[i] = weights[j];
                strcpy(key_as_string[j], temp);
        weights[j] = temp_weight;
            }
        }
   }
   for(i=0;i<number_keys;i++){
      printf("Sorted Key:%s\n",key_as_string[i]);
    }

   for(i=0;i<number_keys;i++){
      printf("Sorted Peso:%lf\n",weights[i]);
    }






}
//the function gets the probabilities and stores them in the array
void getProbabilities(){
 float sum_weights= 0;
 int ind;
 for(ind = 0;ind < number_keys ; ind++){
  sum_weights+=weights[ind];
 }
 printf("The sum of the weights is:%lf\n",sum_weights);

 probabilities = calloc(number_keys, 500*sizeof(float));

 for(ind = 0;ind < number_keys ; ind++){
  probabilities[ind] = weights[ind]/sum_weights;

 }

}

double  array_min(double *mid_matrix){
  double minimum = mid_matrix[0];
  int c;
  int location;
    for ( c = 1 ; c < number_keys+1 ; c++ )
    {
        if ( mid_matrix[c] < minimum )
        {

           minimum = mid_matrix[c];


        }
    }
	//	printf("Minimo:%.2f\n", minimum);
    return minimum;
}
double find_min(double **dist, int i, int j){
	//printf("Vamos a encontrar el minimo, pero primero una prueba\n");
	int a,k;
	double sum_probabilidades =0.0000;
	double *mid_matrix;// Almacenaremos todos los resultados para escoger el menor
	double min;
	mid_matrix = calloc(number_keys+1, 1+sizeof(double));
	for(k = 0; k <number_keys+1; k++) mid_matrix[k] = 99999.9999; // lo llenamos con un numero lo suficientemente grande;



	for(a = i+1; a <= j; a++) {
		sum_probabilidades += probabilities[a-1];
		//printf("probabilities[%d]= %.4f\n",a,probabilities[a-1] );
	}
	//printf("Suma de probabilidades desde key %d hasta key %d = %.4f\n",i+1, j , sum_probabilidades );

	for(k= i+1; k <= j; k++){
	//printf("matriz_solution[%d][%d] = %.4f, matriz_solution[%d][%d] = %.4f \n", i, k-1, matriz_solution[i][k-1], k, j, matriz_solution[k][j]  );
		mid_matrix[k] = matriz_solution[i][k-1] + matriz_solution[k][j] + sum_probabilidades;
	}
	// printf("Mid Matrix\n");
	// for(k = 0; k <number_keys+1; k++) printf("[%.4f] , ",mid_matrix[k] );
	// printf("\n");
	//printSolution(dist);


	min = array_min(mid_matrix);

	for(k = 0; k <number_keys+1; k++) if(min == mid_matrix[k] ) winnerK = k;
	//printf("K GANADOR = %d\n", winnerK);
	//actualizamos tabla R con winner k
	r_table_mtx[i][j] = winnerK;
//	printf("-------------------------------------------------------------\n");
	return min;
}

void 	create_table_A(){
	printf("TRATO DE ABRIR LA VENTANA\n" );
	GtkBuilder      *btree_solution_window_builder;
	GtkWidget       *btree_solution_table_window;
	GtkWidget       *btree_solution_table_scrolledwindow;
	GtkWidget				*btree_solution_grid;

	char buff[25];
	btree_solution_window_builder =  gtk_builder_new();
	gtk_builder_add_from_file (btree_solution_window_builder, "glade/btree_solution_window.glade", NULL);

	btree_solution_table_window = GTK_WIDGET(gtk_builder_get_object(btree_solution_window_builder, "btree_solution_window"));
  gtk_builder_connect_signals(btree_solution_window_builder, NULL);
	gtk_window_set_title (GTK_WINDOW (btree_solution_table_window), "Tabla A");

	btree_solution_table_scrolledwindow = GTK_WIDGET(gtk_builder_get_object(btree_solution_window_builder, "btree_solution_scrolledwindow"));
	gtk_builder_connect_signals(btree_solution_window_builder, NULL);

	btree_solution_grid = gtk_grid_new();
  gtk_grid_set_row_spacing (GTK_GRID (btree_solution_grid), 2);
  gtk_container_add (GTK_CONTAINER (btree_solution_table_scrolledwindow), btree_solution_grid);

	GtkWidget ***entrada;
	int j,k,i;
  entrada=calloc(number_keys+2,sizeof(GtkWidget**));
  for(j = 0; j < number_keys+2; j++){
    entrada[j]=calloc(number_keys+2,sizeof(GtkWidget*));
  }

	for(k =0; k< number_keys+2;k++){
    for(j=0;j<number_keys+2;j++){

      entrada[k][j]= gtk_entry_new();
      gtk_grid_attach (GTK_GRID (btree_solution_grid),entrada[k][j] , k, j, 1, 1);
			gtk_widget_set_sensitive (entrada[k][j], FALSE);
			if(k == 0 && j == 0) {
				gtk_widget_set_name(entrada[k][j], "column_name_A");
			}
			if(k== 0 && j !=0){
			snprintf(buff,25,"%d",j);
			gtk_entry_set_text (entrada[k][j],buff);
			gtk_widget_set_name(entrada[k][j], "column_name_A");
			}
			else if(k!= 0 && j ==0){
			snprintf(buff,25,"%d",k-1);
			gtk_entry_set_text (entrada[k][j],buff);
			gtk_widget_set_name(entrada[k][j], "column_name_A");
			}
			else if(k !=0 && j!= 0){
				gtk_widget_set_name(entrada[k][j], "field_A");
				if ( k > j || k == j){

					if((j-1== 0)&& (k-1 == number_keys)){
						gtk_widget_set_name(entrada[k][j],"new_val_A");
					}
					snprintf(buff,25,"%.4f",matriz_solution[j-1][k-1]);
					gtk_entry_set_text (entrada[k][j],buff);

				}

			}

    }
  }


	g_object_unref(btree_solution_window_builder);

	gtk_widget_show_all(btree_solution_table_window);
}

void 	create_table_R(){
	printf("Table RRRRR\n" );
	GtkBuilder      *btree_solution_window_builder;
	GtkWidget       *btree_solution_table_window;
	GtkWidget       *btree_solution_table_scrolledwindow;
	GtkWidget				*btree_solution_grid;

	char buff[25];
	btree_solution_window_builder =  gtk_builder_new();
	gtk_builder_add_from_file (btree_solution_window_builder, "glade/btree_rSolution_window.glade", NULL);

	btree_solution_table_window = GTK_WIDGET(gtk_builder_get_object(btree_solution_window_builder, "btree_rSolution_window"));
  gtk_builder_connect_signals(btree_solution_window_builder, NULL);
	gtk_window_set_title (GTK_WINDOW (btree_solution_table_window), "Tabla R");

	btree_solution_table_scrolledwindow = GTK_WIDGET(gtk_builder_get_object(btree_solution_window_builder, "btree_rSolution_scrolledwindow"));
	gtk_builder_connect_signals(btree_solution_window_builder, NULL);

	btree_solution_grid = gtk_grid_new();
  gtk_grid_set_row_spacing (GTK_GRID (btree_solution_grid), 2);
  gtk_container_add (GTK_CONTAINER (btree_solution_table_scrolledwindow), btree_solution_grid);

	GtkWidget ***entrada;
	int j,k,i;
  entrada=calloc(number_keys+2,sizeof(GtkWidget**));
  for(j = 0; j < number_keys+2; j++){
    entrada[j]=calloc(number_keys+2,sizeof(GtkWidget*));
  }

	for(k =0; k< number_keys+2;k++){
    for(j=0;j<number_keys+2;j++){

      entrada[k][j]= gtk_entry_new();
      gtk_grid_attach (GTK_GRID (btree_solution_grid),entrada[k][j] , k, j, 1, 1);
			gtk_widget_set_sensitive (entrada[k][j], FALSE);
			if(k == 0 && j == 0) {
				gtk_widget_set_name(entrada[k][j], "column_name_R");
			}
			if(k== 0 && j !=0){
			snprintf(buff,25,"%d",j);
			gtk_entry_set_text (entrada[k][j],buff);
			gtk_widget_set_name(entrada[k][j], "column_name_R");
			}
			else if(k!= 0 && j ==0){
			snprintf(buff,25,"%d",k-1);
			gtk_entry_set_text (entrada[k][j],buff);
			gtk_widget_set_name(entrada[k][j], "column_name_R");
			}
			else if(k !=0 && j!= 0){
				if ( k > j || k == j){
					snprintf(buff,25,"%d",r_table_mtx[j-1][k-1]);
					gtk_entry_set_text (entrada[k][j],buff);
				}
				gtk_widget_set_name(entrada[k][j], "field_R");
			}
    }
  }
	g_object_unref(btree_solution_window_builder);

	gtk_widget_show_all(btree_solution_table_window);
}

void create_solution_tables(){

 	create_table_A();
	create_table_R();


}

void create_solution_matrix(){
		//printf("%s\n", "HOLA");
		int i, j = 0;

		for(i = 0; i< number_keys; i ++) printf("probabilities[%d] = %.4f\n", i, probabilities[i] );

		matriz_solution = calloc(number_keys+1, 1+sizeof(double*));
		r_table_mtx = calloc(number_keys+1, 1+sizeof(int*));
		for (i = 0; i < number_keys+1; ++i) {
			 matriz_solution[i] = calloc(number_keys+1,sizeof(double));
			 r_table_mtx[i] = calloc(number_keys+1,sizeof(int));
		}
		for (i = 0; i<number_keys+1 ; i++){
			for(j = 0; j<number_keys+1; j++ ){
				r_table_mtx[i][j] = 0;
			}
		}
		for (i = 0; i < number_keys+1; i++){
			for ( j = 0; j < number_keys+1; j++){
				if(i == j-1){
					printf("m[%d][%d] = %.4f\n", i, j, probabilities[i] );
					matriz_solution[i][i+1] = probabilities[i] ;
					r_table_mtx[i][i+1] = j;
				}
				else if(i ==  j ){
					matriz_solution[i][j] = 0;
				}
				else{
					matriz_solution[i][j] = 1;
				}
			}
		}

	i = 0;
	j = 2;
	int last_j = 2;
	bool siga = TRUE;
	while(siga){
		if( (i == 0)&&(j == number_keys)){
			printf("[%d][%d]\n", i, j );
			matriz_solution[i][j] = find_min(matriz_solution, i, j);
				siga = FALSE;
			}
			else{
				printf("[%d][%d]\n", i, j );
				matriz_solution[i][j] = find_min(matriz_solution, i, j);
					i ++;
					j ++;
				if (j == number_keys+1){ //reinicializo
					i = 0;
					last_j ++;
					j = last_j;
				}
			}
	}

}

void solve_BTREE_problem(GtkWidget *widget, gpointer   data){
	int fila, columna,value, i, x, w;
  float ret;
	GtkWidget  *entrada;
  entrada = gtk_entry_new();
  gchar* entrance;
  entrance = calloc(1, 500*sizeof(gchar));
	if(f_manual){
		//llenar las matrices con los datos de entrada
		key_as_string = calloc(number_keys, 500*sizeof(char));
		//alojamos la memoria para cada espacio del char
		for (i = 0; i < number_keys; ++i) {
			 key_as_string[i] = (char *)malloc(500);
		}
		weights = calloc(number_keys, 500*sizeof(float));


		//alojamos la memoria para cada espacio
		x, w = 0;
		for(columna = 0; columna <2; columna++){
	    for(fila = 1; fila <= number_keys; fila++){
	      entrada = gtk_grid_get_child_at (data, columna, fila);
					if(columna == 0){
						g_stpcpy(entrance,gtk_entry_get_text(entrada));
						g_stpcpy(key_as_string[x],entrance);
						x++;

					}
					else{

							g_stpcpy(entrance,gtk_entry_get_text(entrada));
							ret = strtof(entrance,NULL);
				      weights[w]=ret;
				      w++;
					}
			}
		}
		writeFile();
	}
	int k;
	for(k=0;k<number_keys;k++){
	 printf("Key:%s\n",key_as_string[k]);
	}

	for(w=0;w<number_keys;w++){
	printf("Peso:%lf\n",weights[w]);
	}

	sort_keys();
	getProbabilities();
	create_solution_matrix();
	printSolution(matriz_solution);
	printf("--------------R TABLE SOLUTION--------------\n");
	printSolution_int(r_table_mtx);
	create_solution_tables();
}


//Manual entry of weights and keys
void create_entry_window(){
	int k,j;
	GtkWidget       *button;
	GtkWidget       *button_box;
	initial_BTREE_table_builder= gtk_builder_new();
	gtk_builder_add_from_file (initial_BTREE_table_builder, "glade/initial_entry_win_BTREE.glade", NULL);

	initial_BTREE_table_window = GTK_WIDGET(gtk_builder_get_object(initial_BTREE_table_builder, "btree_entry_window"));
	gtk_builder_connect_signals(initial_BTREE_table_builder, NULL);

	initial_BTREE_table= gtk_grid_new();
	gtk_grid_set_row_spacing (GTK_GRID (initial_BTREE_table), 2);
	gtk_container_add (GTK_CONTAINER (initial_BTREE_table_window), initial_BTREE_table);

	//dynamic grid
	entrada=calloc(number_keys+1,sizeof(GtkWidget**));
	for(j = 0; j < number_keys+1; j++){
		entrada[j]=calloc(number_keys+1,sizeof(GtkWidget*));
	}

	//loops to create grid
	for(j=0;j<number_keys+1;j++){ //filas
		for(k =0; k< 2;k++){ //columnas
			entrada[j][k]= gtk_entry_new();
			gtk_grid_attach (GTK_GRID (initial_BTREE_table),entrada[j][k] , k, j, 1, 1);
			//printf("Puse la pos[%d][%d]\n",j,k );

		}
	}

	gtk_entry_set_text (entrada[0][0],"Llave");
	gtk_widget_set_sensitive (entrada[0][0], FALSE);
	gtk_widget_set_name(entrada[0][0], "column_name");

	gtk_entry_set_text (entrada[0][1],"Peso");
	gtk_widget_set_sensitive (entrada[0][1], FALSE);
	gtk_widget_set_name(entrada[0][1], "column_name");


	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach (GTK_GRID (initial_BTREE_table),button_box ,0.5, j, 2, 2);

	button = gtk_button_new_with_label ("Procesar");
	g_signal_connect (button, "clicked", G_CALLBACK (solve_BTREE_problem), (gpointer) initial_BTREE_table);
	gtk_container_add (GTK_CONTAINER (button_box), button);

	g_object_unref(initial_BTREE_table_builder);

	gtk_widget_show_all(initial_BTREE_table_window);
}

void filechooser_btree_file_set_cb(){
  FILE *file;
  gchar *filename;
  int ind,in_char,i=0;
  filename=gtk_file_chooser_get_filename (file_chooser);

  number_keys =getObjectsQuantity(filename);
//  printf("Numero de lineas:%d\n", number_keys);


  char **matriz_datos_iniciales = calloc(number_keys*2, 500*sizeof(char));
	//alojamos la memoria para cada espacio del char
	for (i = 0; i < number_keys*2; ++i) {
		 matriz_datos_iniciales[i] = (char *)malloc(500);
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
  //Separate values in two arrays / one for weights and other for keys

  //Change: just allocate when needed

  key_as_string = calloc(number_keys, 500*sizeof(char));
	//alojamos la memoria para cada espacio del char
	for (i = 0; i < number_keys; ++i) {
		 key_as_string[i] = (char *)malloc(500);
	}


  weights = calloc(number_keys, 500*sizeof(float));
	//alojamos la memoria para cada espacio

  int k,w = 0;
  for(i = 0;i<number_keys*2;i++){

    //printf("With %d the result is %d\n",i,reminder );
    if(i % 2  == 0){

      //function to see type of keys
      strcpy(key_as_string[k],matriz_datos_iniciales[i]);
      k++;
    }
    else{
    //  printf("Peso PRIMERO:%s\n",matriz_datos_iniciales[i]);
      //char *ptr;
      float ret;
			ret = strtof(matriz_datos_iniciales[i],NULL);
    //  printf("The number(float) is %.4f\n", ret);
      weights[w]=ret;
      w++;

    }

  }

  for(k=0;k<number_keys;k++){
   printf("Key:%s\n",key_as_string[k]);
  }

  for(w=0;w<number_keys;w++){
  printf("Peso:%lf\n",weights[w]);
  }

  create_entry_window();
	update_initial_table(number_keys);
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
		create_entry_window();
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
