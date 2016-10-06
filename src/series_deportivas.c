#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

GtkWidget *number_games, *ph_entry, *pr_entry, *partida, *warning_window;
GtkBuilder      *series_deportivas_table_builder;
GtkWidget       *series_deportivas_table_window;
GtkWidget       *series_deportivas_table_scrolledwindow;
GtkWidget       *series_deportivas_table;

GtkBuilder      *file_saver_builder;
GtkWidget       *file_saver_window;
GtkFileChooser *file_chooser;

GtkWidget **entrada;

int ph,pr,charPos,number_of_games, defining_games_quantity = 0;
int *global_numberofgames;
int *matriz_datos;
double **matriz_solution;

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

void update_global_numberofgames(GtkWidget *widget, gpointer   data){
	int val = data;

	if(global_numberofgames[val] ==0){
			global_numberofgames[val] = 1;
			printf("HOLAAAA %d VAl = %d\n", data, global_numberofgames[val] );
	}
	else{
		global_numberofgames[val] = 0;
		printf("HOLAAAA %d VAl = %d\n", data, global_numberofgames[val] );
	}
}

void printSolution(double **dist)
{
    int i,j;
    for (i = 0; i < defining_games_quantity+1; i++)
    {
        for ( j = 0; j < defining_games_quantity+1; j++)
        {
            printf ("%10.4f", dist[i][j]);
        }
        printf("\n");
    }
}

void calculateProbabilities(){
	int i, j, nextgame, games2go = 0;

	for(i =0; i < number_of_games; i++) printf("global_numberofgames[%d] = %d\n",i, global_numberofgames[i]);

	matriz_solution = calloc(defining_games_quantity+1, 1+sizeof(double*));
	for (i = 0; i < defining_games_quantity+1; ++i) {
		 matriz_solution[i] = calloc(defining_games_quantity+1,sizeof(double));
	}

	for (i = 0; i < defining_games_quantity+1; i++){
		for ( j = 0; j < defining_games_quantity+1; j++){
			if(i == 0 && j !=0 ){
				matriz_solution[i][j] = 1.00;
			}
			else if (j == 0 && i != 0){
				matriz_solution[i][j] = 0.00;
			}
			else if (j == 0 && i ==0){
				matriz_solution[i][j] = -1.00;
			}
			else{
				games2go =((defining_games_quantity - i)+(defining_games_quantity -j));
				// printf("i = %d, j= %d\n", i, j );
				// printf("G2G = %d\n", games2go );
				if(global_numberofgames[games2go] == 1){
					matriz_solution[i][j] = (matriz_solution[i-1][j] * floorf(ph_double* 100) / 100 + matriz_solution[i][j-1] * floorf(qr_double* 100) / 100);
				}
				else{
					matriz_solution[i][j] = (matriz_solution[i-1][j] * floorf(pr_double* 100) / 100 + matriz_solution[i][j-1] * floorf(qh_double* 100) / 100);
				}
			}

		}
	}

	printSolution(matriz_solution);
}


void solve_series_deportivas_problem(){
	//Solve Matrix

	calculateProbabilities();

	char game_number[25];
	series_deportivas_table_builder= gtk_builder_new();
  gtk_builder_add_from_file (series_deportivas_table_builder, "glade/initial_series_deportivas_table_window.glade", NULL);

  series_deportivas_table_window = GTK_WIDGET(gtk_builder_get_object(series_deportivas_table_builder, "initial_series_deportivas_table_window"));
  gtk_builder_connect_signals(series_deportivas_table_builder, NULL);

  series_deportivas_table_scrolledwindow = GTK_WIDGET(gtk_builder_get_object(series_deportivas_table_builder, "initial_series_deportivas_table_scrolledwindow"));
	gtk_builder_connect_signals(series_deportivas_table_builder, NULL);

  series_deportivas_table= gtk_grid_new();
  gtk_grid_set_row_spacing (GTK_GRID (series_deportivas_table), 2);
  gtk_container_add (GTK_CONTAINER (series_deportivas_table_scrolledwindow), series_deportivas_table);

  GtkWidget ***entrada;
	int j,k,i;
  entrada=calloc(defining_games_quantity+2,sizeof(GtkWidget**));
  for(j = 0; j < defining_games_quantity+2; j++){
    entrada[j]=calloc(defining_games_quantity+2,sizeof(GtkWidget*));
  }
  for(k =0; k< defining_games_quantity+2;k++){
    for(j=0;j<defining_games_quantity+2;j++){

      entrada[k][j]= gtk_entry_new();
      gtk_grid_attach (GTK_GRID (series_deportivas_table),entrada[k][j] , k, j, 1, 1);
			gtk_widget_set_sensitive (entrada[k][j], FALSE);
      if(k== 0 && j !=0){
			snprintf(game_number,25,"%d",j-1);
      gtk_entry_set_text (entrada[k][j],game_number);
			gtk_widget_set_name(entrada[k][j], "column_name");
      }
      if (k != 0 && j == 0){
				snprintf(game_number,25,"%d",k-1);
	      gtk_entry_set_text (entrada[k][j],game_number);
				gtk_widget_set_name(entrada[k][j], "column_name");

      }
    }
  }

	g_object_unref(series_deportivas_table_builder);

	gtk_widget_show_all(series_deportivas_table_window);
}

void create_buttons_window(){
	int k,j;
	char game_number[25];
	GtkWidget *window;
	GtkWidget *scrolledwindow;
	GtkAdjustment *adjustmentValue;
	GtkWidget *table;
	GtkWidget *button;
	GtkWidget *button_box;

	/* create a new window */
	 window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	 gtk_window_set_title (GTK_WINDOW (window),"Series Deportivas");

	 /*Scrolled window*/

	 scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
	 gtk_widget_set_size_request(scrolledwindow, 432, 100);


	 gtk_container_add (GTK_CONTAINER (window), scrolledwindow);

	 // create new table
	 table= gtk_grid_new();
	 gtk_grid_set_row_spacing (GTK_GRID (table), 2);
	 gtk_container_add (GTK_CONTAINER (scrolledwindow), table);
	//dynamic grid
	entrada=calloc(number_of_games+1,sizeof(GtkWidget*));
	  int fila = 0;
  for(j = 0; j < number_of_games; j++){
		snprintf(game_number,25,"%d",j+1);
    entrada[j]= gtk_toggle_button_new_with_label (game_number);
		gtk_widget_set_name(entrada[j], "toggle_btn");
		g_signal_connect (entrada[j], "toggled", G_CALLBACK (update_global_numberofgames),(gpointer) j);

		gtk_grid_attach (GTK_GRID (table),entrada[j], j, 1, 1, 1);
  }

	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach (GTK_GRID (table),button_box ,(j/2)-1, j, 3, 2);

	button = gtk_button_new_with_label ("Procesar");
	g_signal_connect (button, "clicked", G_CALLBACK (solve_series_deportivas_problem), NULL);
	gtk_container_add (GTK_CONTAINER (button_box), button);

		gtk_widget_show_all(window);
}

void on_load_file_button_file_set(){
  FILE *file;
  gchar *filename;
  int i,k, in_char, ind = 0;
	bool passed = TRUE;

	char **matriz_datos_iniciales = calloc(3, 500*sizeof(char));
	//alojamos la memoria para cada espacio del char
	for (i = 0; i < 3; ++i) {
		 matriz_datos_iniciales[i] = (char *)malloc(500);
	}

	matriz_datos =  calloc(3, 1+sizeof(int));

  filename=gtk_file_chooser_get_filename (file_chooser);
  file = fopen( filename, "r" );
    if(file){
      clear_token_buffer();
      while ((in_char = getc(file)) != EOF){
          if((in_char == '|')|| (in_char == '\n')){
						 strcpy(matriz_datos_iniciales[ind], string_buffer);
						 if(!is_number(matriz_datos_iniciales[ind])){
							 create_warning_window("El tipo de dato debe ser entero");
							 passed = FALSE;
						 }
						 else if(strcmp(matriz_datos_iniciales[ind], "")== 0){
							 create_warning_window("Los campos no pueden ser vacíos");
							 passed = FALSE;
						 }
						 else{
							 matriz_datos[ind] = atoi(matriz_datos_iniciales[ind]);
								ind ++;
								clear_token_buffer();
								k++;
						 }

          }
          else{
            buffer_char(in_char);
          }
      }
     }
    else{
        printf("%s\n","Error al abrir el archivo" );
      }

			for (i = 0; i < 3; ++i) {
				 printf("MAtriz[%d] = %d\n",i,  matriz_datos[i]);
			}

		if (passed){
			ph = matriz_datos[1];
			pr =  matriz_datos[2];
			number_of_games =  matriz_datos[0];
			 printf("ALL GOOD :) \n");
			 ph_double = (double) ph / 100;
			 qr_double = 1 - ph_double;
			 pr_double = (double) pr / 100;
			 qh_double = 1 - pr_double;
			 printf("Mi probabilidad en casa %f\n", ph_double);
			 printf("Probabilidad de equipo contrario on the road %f\n", qr_double);
			 printf("Mi probabilidad on the road %f\n", pr_double);
			 printf("Probabilidad de equipo contrario en casa %f\n", qh_double);
			 printf("JUEGOS TOTALES %d\n", number_of_games);
			 global_numberofgames = (int) calloc(number_of_games+1, sizeof(int));
			defining_games_quantity = (number_of_games + 1) / 2;
			printf("DEFIING GAMES QUANTITY = %d\n", defining_games_quantity );
			for(k = 0; k < number_of_games; k++) global_numberofgames[k] = 0;
			create_buttons_window();


		}
}

void on_create_button_clicked(){
	int k;
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
    global_numberofgames = (int) calloc(number_of_games+1, sizeof(int));
		defining_games_quantity = (number_of_games + 1) / 2;
		printf("DEFIING GAMES QUANTITY = %d\n", defining_games_quantity );
		for(k = 0; k < number_of_games; k++) global_numberofgames[k] = 0;
    create_buttons_window();
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
