#include <gtk/gtk.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>


//Global variables
GtkWidget *entry_initial_price;
GtkWidget *entry_useful_life;
GtkWidget *entry_project_term;
GtkBuilder      *file_saver_builder;
GtkWidget       *file_saver_window;
GtkFileChooser *file_chooser;
int verify_entry,charPos,initial_price,useful_life,project_term = 0;
bool f_manual = FALSE;
char intermediate_buffer[5];
char string_buffer[25];
char file_name_buffer[25];
int **matriz_datos;
GtkWidget       *warning_window;
GtkBuilder      *initial_equipment_table_builder;
GtkWidget       *initial_equipment_table_window;
GtkWidget       *initial_equipment_table;
GtkWidget ***entrada;

int *global_mantenimiento;
int *global_ventas;

void printSolution(int  **dist, int numbOfObj)
{
    int i,j;
    for (i = 0; i < useful_life; i++)
    {
        for ( j = 0; j < 2; j++)
        {
            printf ("%7d", dist[i][j]);
        }
        printf("\n");
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
    char file_value[5];
    output= fopen( file_name_buffer, "w+");


      for(j=0;j<useful_life;j++){
      	snprintf(file_value,5,"%d",global_mantenimiento[j]);
      	fprintf(output, "%s|",file_value);
      	clear_file_buffer(file_value);
      	snprintf(file_value,5,"%d",global_ventas[j]);
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
  gtk_main();
}

int array_min(int array[project_term+1]){
  int minimum = array[0];
  int c;
  int location;
    for ( c = 1 ; c < project_term+1 ; c++ )
    {
        if ( array[c] < minimum )
        {
           minimum = array[c];
           location = c+1;
        }
    }
    return minimum;
}

void getEquipmentReplaceSolution(){
  int C[useful_life+1];
  int G[project_term+1];// G[0] a G[n]
  int k, i, j, x;
  int R[project_term+1];


  C[0] = 0;
  for(k = 0; k <= project_term; k++) R[k] = 99999;

  for (k = 1; k <= useful_life;k++){
    C[k] =  initial_price;
    for (i = 1; i <= k;i++){
      C[k] +=global_mantenimiento[i];
    }
    C[k]= C[k]-global_ventas[k];
  }

  printf("----------Costos totales------------\n");
  for (i = 0; i <= useful_life; i ++) printf("%d\n", C[i] );
  G[project_term] =0;
  for(i = project_term-1; i>=0; i --){
    int valor_actual = i;
    int plazo = project_term;
    int v = plazo - valor_actual;
    printf("Valor Actual = %d \n", valor_actual );
    if(v <= useful_life){
      int indice = 1;
      for(j = v; j >=1 ; j--){
          //printf("C[%d] + G[%d] = %d + %d  = %d\n",j, valor_actual+indice,C[j], G[valor_actual + indice], C[j]+G[valor_actual+indice]);
          R[valor_actual+indice] = C[j]+G[valor_actual+indice];
          //printf("R[%d] = %d\n", valor_actual, R[valor_actual] );
          indice ++;
          G[valor_actual] = array_min(R);
          printf("G[%d] = %d\n", valor_actual, G[valor_actual] );

      }
    }
      for(k = 0; k <= project_term; k++) R[k] = 99999;
  }
}

void solve_requipmentEquipment_problem(GtkWidget *widget, gpointer   data){
  printf("Let's do this!\n" );
  GtkWidget  *entrada;
  entrada = gtk_entry_new();
  gchar* entrance;
  entrance = calloc(1, 500*sizeof(gchar));

  global_mantenimiento = (int) calloc(useful_life+1, sizeof(int));
  global_ventas = (int) calloc(useful_life+1, sizeof(int));

  global_mantenimiento[0] = 0;
  global_ventas[0] = 0;
  int i = 1;

  int fila, columna,value =0;
  int j = 1;
  int k = 0;
  int x = 0;
  for(columna = 0; columna <3; columna++){
    for(fila = 1; fila <= useful_life; fila++){
      entrada = gtk_grid_get_child_at (data, columna, fila);

      if(columna == 1){
        g_stpcpy(entrance,gtk_entry_get_text(entrada));
        verify_entry = gtk_entry_get_text(entrada);
        if (!is_number(verify_entry)){
          create_warning_window("Las entradas deben ser numericas.");
        }
        else{
          global_mantenimiento[i] = atoi(entrance);
          i++;
        }
      }
      else if(columna == 2){
        g_stpcpy(entrance,gtk_entry_get_text(entrada));
        verify_entry = gtk_entry_get_text(entrada);
        if (!is_number(verify_entry)){
          create_warning_window("Las entradas deben ser numericas.");
        }
        else{
          global_ventas[j] = atoi(entrance);
          j++;
        }

      }
    }
  }

  printf("GLOBAL Mantenimiento\n");
  for(i = 0; i < useful_life; i++){
    printf("Mantenimiento[%d] = %d\n", i, global_mantenimiento[i]);
  }
  printf("GLOBAL Ventas\n");
  for(i = 0; i < useful_life; i++){
    printf("Ventas[%d] = %d\n", i, global_ventas[i]);
  }
  if(f_manual) writeFile();
  printf("VOY POR ACA\n");
  getEquipmentReplaceSolution();
}


void create_entry_window(){
	int k,j;
	GtkWidget       *button;
	GtkWidget       *button_box;
	initial_equipment_table_builder= gtk_builder_new();
	gtk_builder_add_from_file (initial_equipment_table_builder, "glade/initial_equipment_table_window.glade", NULL);

	initial_equipment_table_window = GTK_WIDGET(gtk_builder_get_object(initial_equipment_table_builder, "initial_equipment_table_window"));
	gtk_builder_connect_signals(initial_equipment_table_builder, NULL);

	initial_equipment_table= gtk_grid_new();
	gtk_grid_set_row_spacing (GTK_GRID (initial_equipment_table), 2);
	gtk_container_add (GTK_CONTAINER (initial_equipment_table_window), initial_equipment_table);

	//dynamic grid
	entrada=calloc(useful_life+1,sizeof(GtkWidget**));
	for(j = 0; j < useful_life+1; j++){
		entrada[j]=calloc(useful_life+1,sizeof(GtkWidget*));
	}

	//loops to create grid
	for(j=0;j<useful_life+1;j++){ //filas
		for(k =0; k< 3;k++){ //columnas
			entrada[j][k]= gtk_entry_new();
			gtk_grid_attach (GTK_GRID (initial_equipment_table),entrada[j][k] , k, j, 1, 1);
			//printf("Puse la pos[%d][%d]\n",j,k );
      if((k==0) && (j!=0)){
        snprintf(intermediate_buffer,5,"%d",j);
        gtk_entry_set_text (entrada[j][k],intermediate_buffer);
        gtk_widget_set_sensitive (entrada[j][k], FALSE);
      }

		}
	}

	gtk_entry_set_text (entrada[0][0],"Año");
	gtk_widget_set_sensitive (entrada[0][0], FALSE);
	gtk_widget_set_name(entrada[0][0], "column_name");

	gtk_entry_set_text (entrada[0][1],"Mantenimiento");
	gtk_widget_set_sensitive (entrada[0][1], FALSE);
	gtk_widget_set_name(entrada[0][1], "column_name");

	gtk_entry_set_text (entrada[0][2],"Venta");
	gtk_widget_set_sensitive (entrada[0][2], FALSE);
	gtk_widget_set_name(entrada[0][2], "column_name");


	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach (GTK_GRID (initial_equipment_table),button_box ,1, j, 1, 2);

	button = gtk_button_new_with_label ("Procesar");
	g_signal_connect (button, "clicked", G_CALLBACK (solve_requipmentEquipment_problem), (gpointer) initial_equipment_table);
	gtk_container_add (GTK_CONTAINER (button_box), button);

	g_object_unref(initial_equipment_table_builder);

	gtk_widget_show_all(initial_equipment_table_window);
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

void update_initial_table(int useful_life){
	printf("%s\n","1. Modifico la tabla inicial" );
  GtkWidget  *pt_entrada;
  pt_entrada = gtk_entry_new();
  char pt_cell_value[25];

  int i, j;
  for(i = 0; i < useful_life ; i ++){
    for (j = 0; j<4; j++){
			if(j == 0){
				pt_entrada = gtk_grid_get_child_at (initial_equipment_table, j, i+1);
        snprintf(intermediate_buffer,5,"%d",i);
        gtk_entry_set_text (pt_entrada, intermediate_buffer);
        gtk_widget_set_sensitive (pt_entrada, FALSE);
			}
      pt_entrada = gtk_grid_get_child_at (initial_equipment_table, j+1, i+1);
      snprintf(pt_cell_value,5,"%d",matriz_datos[i][j]);
				gtk_entry_set_text (pt_entrada, pt_cell_value );
			}
    }
}


void kn_filechooserbutton_file_set_cb(){
  FILE *file;
  gchar *filename;
  int in_char;
	int fila, columna , k, i, j= 0;
  filename=gtk_file_chooser_get_filename (file_chooser);
   int i_price = gtk_entry_get_text (entry_initial_price);
   int u_life =  gtk_entry_get_text (entry_useful_life);
   int p_term = gtk_entry_get_text (entry_project_term);

   if(!is_number(i_price) || !is_number(u_life) || !is_number(p_term)){

     create_warning_window("El tipo de dato debe ser entero");

   }
   else{
     printf("El numero de filas es:%d\n",useful_life);
     initial_price = atoi(i_price);
     project_term = atoi(p_term);
     useful_life =getObjectsQuantity(filename);
   	 printf("Numero de lineas%d\n", useful_life);

     		//alojamos memoria de la matriz de datos
     matriz_datos = calloc(useful_life, 1+sizeof(int*));
     for (i = 0; i < useful_life; ++i) {
     		matriz_datos[i] = calloc(useful_life,sizeof(int));
     }
     char **matriz_datos_iniciales = calloc(useful_life*2, 500*sizeof(char));
     //alojamos la memoria para cada espacio del char
     for (i = 0; i < useful_life*2; ++i) {
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
		 }
		else{
				printf("%s\n","Error al abrir el archivo" );
			}
      printf("VAmos a imprimir los datos iniciales\n");
      for (i = 0; i < useful_life*2; ++i) {
         printf("%s\n", matriz_datos_iniciales[i]);
      }

      fila, columna,i = 0;
			for (k = 0; k< useful_life*2 ; k++){
  			if((k % 2 == 0)&&(k !=0)){
					fila++;
          columna = 0;
  			}
        verify_entry = matriz_datos_iniciales[k];
        if(!is_number(verify_entry)){
          create_warning_window("Las entradas deben ser numericas");
        }
        else{
          printf("matriz_datos[%d][%d] =  %d\n",fila, columna,atoi(matriz_datos_iniciales[k]));
          matriz_datos[fila][columna] = atoi(matriz_datos_iniciales[k]);
          columna++;
        }
  		}

      printf("Prueba datos de la matriz\n" );
      printSolution(matriz_datos, useful_life);
      update_initial_table(useful_life);


   }
}


void btn_ok_clicked_cb(){
  gchar* message;
  message = calloc(1, 500*sizeof(gchar));


  int i_price = gtk_entry_get_text (entry_initial_price);
  int u_life =  gtk_entry_get_text (entry_useful_life);
  int p_term = gtk_entry_get_text (entry_project_term);

  if(!is_number(i_price) || !is_number(u_life) || !is_number(p_term)){

    create_warning_window("El tipo de dato debe ser entero");

  }
  else{
    f_manual = TRUE;
    initial_price = atoi(i_price);
    useful_life = atoi(u_life);
    project_term = atoi(p_term);
    create_entry_window();
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
