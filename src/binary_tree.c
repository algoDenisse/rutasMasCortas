#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Global variables
GtkWidget *number_keys_entry, *warning_window;
GtkFileChooser *file_chooser;
char string_buffer[25];

int number_keys,charPos;
bool f_manual = FALSE;

//For keys we will need to arrays / keys can be strings or number_keys
char **key_as_string;
char **sorted_key_as_string;

//Array for weights
float *weights;
float *probabilities;

GtkBuilder      *initial_BTREE_table_builder;
GtkWidget       *initial_BTREE_table_window;
GtkWidget       *initial_BTREE_table;
GtkWidget ***entrada;

void buffer_char(char c){
	string_buffer[charPos++] = c;
}

void clear_token_buffer(){
	memset(string_buffer, 0, 25);
	charPos = 0;
}
//Changes added by Treicy
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

void solve_BTREE_problem(){
	printf("%s\n","Estoy en Procesar");
	sort_keys();
	getProbabilities();
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
	//printf("%s\n","1. Estoy llenando la tabla inicial" );
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
				snprintf(pt_cell_value,10,"%lf",weights[i-1]);
				printf("%s\n",pt_cell_value );
				gtk_entry_set_text (pt_entrada, pt_cell_value );

			}

    }// end j FOR
  }//end i FOR

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
  //printf("Numero de lineas:%d\n", number_keys);


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
      printf("Peso:%s\n",matriz_datos_iniciales[i]);
      //char *ptr;
      float ret;
			ret = strtof(matriz_datos_iniciales[i],NULL);
      //printf("The number(float) is %lf\n", ret);
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
