#include <gtk/gtk.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

#define MAXINPUT 100
#define INF 99999

int object_number=0;
int knapsack_capacity=0;
GtkWidget *entry_object_number;
GtkWidget *entry_knapsack_capacity;
GtkWidget       *warning_window;
GtkWidget *knapsack_solution_window;
GtkFileChooser *file_chooser;
char string_buffer[25];
int charPos,numbOfObj,nCapacity;
char **column_names;
int **matriz_datos;
bool f_manual = FALSE;

int knapsack_type;


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
void writeFile(){
	//Archivo en el que se graba información
	FILE * output;
	int j, i;
	char file_value[5];
	output= fopen( "output.txt", "w+");

	// printf("Prueba de nombre de objetos\n");
	// for (i = 0; i<numbOfObj ; i++) printf("Objeto %d = %s\n",i, column_names[i] );

		for(j=0;j<numbOfObj;j++){

			fprintf(output, "%s|",column_names[j]);
			snprintf(file_value,5,"%d",global_quantity[j]);
			fprintf(output, "%s|",file_value);
			clear_file_buffer(file_value);
			snprintf(file_value,5,"%d",global_weights[j]);
			fprintf(output, "%s|",file_value);
			clear_file_buffer(file_value);
			snprintf(file_value,5,"%d",global_values[j]);
			fprintf(output, "%s",file_value);
			fprintf(output, "\n");
		}



	fclose(output);
	printf("%s\n","Termine el write" );


}

void create_solution_table(int pK[numbOfObj+1][nCapacity+1]){

	GtkWidget *window;
	GtkWidget *scrolledwindow;
	GtkAdjustment *adjustmentValue;
  GtkWidget *table;
  GtkWidget *button;
  GtkWidget *button_box;

	/* create a new window */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window),"Knapsack Solution");

	 /*Scrolled window*/

	 scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
	 gtk_widget_set_size_request(scrolledwindow, 600, 300);


	 gtk_container_add (GTK_CONTAINER (window), scrolledwindow);

	 // create new table
   table= gtk_grid_new();
   gtk_grid_set_row_spacing (GTK_GRID (table), 2);
   gtk_container_add (GTK_CONTAINER (scrolledwindow), table);

	 GtkWidget ***entrada;

	 int j,k,i,t;
	 char pt_cell_value[5];
	 char cell_value[5];
	 char cell_prev_value[5];
	 t=0;
   entrada=calloc(nCapacity,2 + sizeof(GtkWidget**));
   for(j = 0; j < nCapacity+1; j++){
     entrada[j]=calloc(nCapacity,2+sizeof(GtkWidget*));
   }

	 for(k =0; k< nCapacity+2;k++){
     for(j=0;j<numbOfObj+1;j++){
			 entrada[j][k]= gtk_entry_new ();
       gtk_widget_set_sensitive (entrada[j][k], FALSE);
       gtk_grid_attach (GTK_GRID (table),entrada[j][k] , j, k, 1, 1);

			 if (k == 0 && j != 0){
				  gtk_widget_set_name(entrada[j][k], "column_name");
          gtk_entry_set_text (entrada[j][k],column_names[j-1]);
       }
			 if (j ==0 && k!=0){
         gtk_widget_set_name(entrada[j][k], "column_name");
				 snprintf(pt_cell_value,5,"%d",t);
         gtk_entry_set_text(entrada[j][k],pt_cell_value);
				 t++;
       }

			 if(k!=0&& j!=0){
				 gtk_widget_set_name(entrada[j][k], "new_val");
        	gtk_entry_set_text (entrada[j][k], "0");

       }

      }

    }

// Llena con datso del resultado
printf("voy por el segundo for\n" );
t=0;
	 for(k =0; k< nCapacity+2;k++){
     for(j=0;j<numbOfObj+1;j++){

			 if (k == 0 && j != 0){
				  gtk_widget_set_name(entrada[j][k], "column_name");
          gtk_entry_set_text (entrada[j][k],column_names[j-1]);
       }
			 if (j ==0 && k!=0){
         gtk_widget_set_name(entrada[j][k], "column_name");
				 snprintf(pt_cell_value,5,"%d",t);
         gtk_entry_set_text(entrada[j][k],pt_cell_value);
				 t++;
       }
			 if(j == 1 && k !=0){
				 snprintf(cell_value,5,"%d",pK[j][k-1]);
				 if(strcmp(cell_value, gtk_entry_get_text(entrada[j][k]))!= 0){
					 gtk_widget_set_name(entrada[j][k], "changed_val");
					 gtk_entry_set_text (entrada[j][k], cell_value);
				 }

			 }
			 if(k!=0&& j!=0){
				 if(j!=1){
					 snprintf(cell_value,5,"%d",pK[j][k-1]);
					 snprintf(cell_prev_value,5,"%d", pK[j-1][k-1]);
					 if(strcmp(cell_value, cell_prev_value)!= 0){
						 gtk_widget_set_name(entrada[j][k], "changed_val");
						 gtk_entry_set_text (entrada[j][k], cell_value);
					 }
					 else{
						 gtk_entry_set_text (entrada[j][k], cell_value);
					 }
				 }
       }

      }
		}
			gtk_widget_show_all(window);


}

void create_solution_table_bounded(int pK[numbOfObj+1][nCapacity+1], int pnewCopies[numbOfObj+1][nCapacity+1]){

	GtkWidget *window;
	GtkWidget *scrolledwindow;
	GtkAdjustment *adjustmentValue;
  GtkWidget *table;
  GtkWidget *button;
  GtkWidget *button_box;

	/* create a new window */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window),"Knapsack Solution");

	 /*Scrolled window*/

	 scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
	 gtk_widget_set_size_request(scrolledwindow, 600, 300);


	 gtk_container_add (GTK_CONTAINER (window), scrolledwindow);

	 // create new table
   table= gtk_grid_new();
   gtk_grid_set_row_spacing (GTK_GRID (table), 2);
   gtk_container_add (GTK_CONTAINER (scrolledwindow), table);

	 GtkWidget ***entrada;

	 int j,k,i,t;
	 char pt_cell_value[5];
	 char cell_value[5];
	 char cell_prev_value[5];
	 char actual_cell_value[10];
	 char quantity_cell[5];
	 char xn[5];
	 t=0;
   entrada=calloc(nCapacity,2 + sizeof(GtkWidget**));
   for(j = 0; j < nCapacity+1; j++){
     entrada[j]=calloc(nCapacity,2+sizeof(GtkWidget*));
   }

	 for(k =0; k< nCapacity+2;k++){
     for(j=0;j<numbOfObj+1;j++){
			 entrada[j][k]= gtk_entry_new ();
       gtk_widget_set_sensitive (entrada[j][k], FALSE);
       gtk_grid_attach (GTK_GRID (table),entrada[j][k] , j, k, 1, 1);

			 if (k == 0 && j != 0){
				  gtk_widget_set_name(entrada[j][k], "column_name");
          gtk_entry_set_text (entrada[j][k],column_names[j-1]);
       }
			 if (j ==0 && k!=0){
         gtk_widget_set_name(entrada[j][k], "column_name");
				 snprintf(pt_cell_value,5,"%d",t);
         gtk_entry_set_text(entrada[j][k],pt_cell_value);
				 t++;
       }

			 if(k!=0&& j!=0){
				 gtk_widget_set_name(entrada[j][k], "new_val");
        	gtk_entry_set_text (entrada[j][k], "0");

       }

      }

    }

// Llena con datso del resultado
printf("voy por el segundo for\n" );
t=0;
	 for(k =0; k< nCapacity+2;k++){
     for(j=0;j<numbOfObj+1;j++){

			 if (k == 0 && j != 0){
				  gtk_widget_set_name(entrada[j][k], "column_name");
          gtk_entry_set_text (entrada[j][k],column_names[j-1]);
       }
			 if (j ==0 && k!=0){
         gtk_widget_set_name(entrada[j][k], "column_name");
				 snprintf(pt_cell_value,5,"%d",t);
         gtk_entry_set_text(entrada[j][k],pt_cell_value);
				 t++;
       }
			 if(j == 1 && k !=0){
				 snprintf(cell_value,5,"%d",pK[j][k-1]);
				 snprintf(quantity_cell,5,"%d",pnewCopies[j][k-1]);
				 if(strcmp(cell_value, gtk_entry_get_text(entrada[j][k]))!= 0){
					 strcpy(actual_cell_value, cell_value);
					 strcat(actual_cell_value, " X1 = ");
					 strcat(actual_cell_value, quantity_cell);

					 gtk_widget_set_name(entrada[j][k], "changed_val");
					 gtk_entry_set_text (entrada[j][k], actual_cell_value);
				 }

			 }
			 if(k!=0&& j!=0){
				 if(j!=1){
					 snprintf(cell_value,5,"%d",pK[j][k-1]);
					 snprintf(cell_prev_value,5,"%d", pK[j-1][k-1]);
					 snprintf(quantity_cell,5,"%d",pnewCopies[j][k-1]);
					 snprintf(xn,5,"%d",j);
					 strcpy(actual_cell_value, cell_value);
					 strcat(actual_cell_value, " X");
					 strcat(actual_cell_value, xn);
					 strcat(actual_cell_value, " = ");
					 strcat(actual_cell_value, quantity_cell);
					 if(strcmp(cell_value, cell_prev_value)!= 0){
						 gtk_widget_set_name(entrada[j][k], "changed_val");
						 gtk_entry_set_text (entrada[j][k], actual_cell_value);
					 }
					 else{
						 if(atoi(cell_prev_value)!= 0){
							 gtk_entry_set_text (entrada[j][k], actual_cell_value);
						 }

					 }
				 }
       }

      }
		}
			gtk_widget_show_all(window);


}

int max(int a, int b) { return (a > b)? a : b; }
int min(int a, int b) { return (a < b)? a : b; }

void getResults(int pnewCopies[numbOfObj+1][nCapacity+1], int pK[numbOfObj+1][nCapacity+1]){
	int k, j,i;
	char Maximizar[1024];
	char Sujeto[1024];
	char Solucion[1024];
	char intermedio[506];

	int resultado[numbOfObj];
	for(i = 0; i<numbOfObj; i++) resultado[i] = 0;

	printf("------imprime el resultado de newCopies--------\n");
	for (k = 0; k <= nCapacity; k++)
	{
			for ( j = 0; j < numbOfObj+1; j++)
			{
				if (pnewCopies[j][k] == INF)
							printf("%7s", "INF");
					else
							printf ("%7d", pnewCopies[j][k]);
			}
			printf("\n");
	}

//	printf("%s\n","NEW COPIES al reves" );
		for (k = nCapacity; k>=0; k-- ){
			for(j = numbOfObj; j>=0 ; j --){
				//printf("punto[%d][%d] = %d \n",j, k ,pnewCopies[j][k]);
				if(pnewCopies[j][k] != 0){
					//printf("cantidad = %d, peso = %d, peso calculado = %d\n", pnewCopies[j][k], global_weights[j-1], pnewCopies[j][k]*global_weights[j-1]);
					//printf("tiene que llevar X%d = %d\n",j, pnewCopies[j][k] );
					resultado[j-1] = pnewCopies[j][k];
					k = k - pnewCopies[j][k]*global_weights[j-1];
				}
			}
			printf("\n");
		}

	//	printf("Problema en forma matematica\n");

		//strcpy(Maximizar, "Maximizar Z = " );
		//strcat(Maximizar, " X1 = ");
		//strcat(actual_cell_value, quantity_cell);

	  strcpy(Maximizar, "Maximizar Z = " );

		for(i =0 ; i <  numbOfObj-1; i++) {
			snprintf(intermedio, 506, "%dx%d +", global_values[i], i+1);
			strcat(Maximizar, intermedio);
		}
		snprintf(intermedio,506,"%dx%d\n",global_values[numbOfObj-1], numbOfObj);
		strcat(Maximizar, intermedio);



		//printf("%dX%d\n",global_values[numbOfObj-1], numbOfObj);

		strcpy(Sujeto, "Sujeto a: ");
		for(i =0 ; i <  numbOfObj-1; i++){
			snprintf(intermedio, 506, "%dx%d +",global_weights[i], i+1);
			strcat(Sujeto, intermedio);
		}
		snprintf(intermedio,506,"%dx%d <= %d\n",global_weights[numbOfObj-1], numbOfObj, nCapacity);
		strcat(Sujeto, intermedio);



		snprintf(intermedio, 506,"Z = %d\n",pK[numbOfObj][nCapacity]);
		strcpy(Solucion,intermedio);
		for(i = 0; i<numbOfObj; i++){
			snprintf(intermedio,506,"x%d = %d\n",i+1,resultado[i] );
			strcat(Solucion, intermedio);
		}

		GtkBuilder      *knapsack_solution_builder;

		GtkWidget       *knapsack_solution_label1;
		GtkWidget       *knapsack_solution_label2;
		GtkWidget       *knapsack_solution_label3;



		knapsack_solution_builder = gtk_builder_new();
		gtk_builder_add_from_file (knapsack_solution_builder, "glade/knapsack_solution_window.glade", NULL);

		knapsack_solution_window = GTK_WIDGET(gtk_builder_get_object(knapsack_solution_builder, "knapsack_solution_window"));
		gtk_builder_connect_signals(knapsack_solution_builder,NULL);

		knapsack_solution_label1 = GTK_WIDGET(gtk_builder_get_object(knapsack_solution_builder, "label1"));
		knapsack_solution_label2 = GTK_WIDGET(gtk_builder_get_object(knapsack_solution_builder, "label2"));
		knapsack_solution_label3 = GTK_WIDGET(gtk_builder_get_object(knapsack_solution_builder, "label3"));

		gtk_label_set_text (knapsack_solution_label1,Maximizar);
		gtk_label_set_text (knapsack_solution_label2,Sujeto);
		gtk_label_set_text (knapsack_solution_label3,Solucion);

		g_object_unref(knapsack_solution_builder);

		gtk_widget_show_all(knapsack_solution_window);
		gtk_main();


}


void knapSack_UnBounded(int W, int *wt, int *val, int n){
	nCapacity = atoi(W);
	int K[n+1][nCapacity+1];
	int newCopies[n+1][nCapacity+1];
	int i, w, Q, j, x, k;

	int c;
	c = 0;
	for (i = 0; i <= n; i++)
		{
			for (w = 0; w <= nCapacity; w++)
				{
						if (i==0 || w==0) {
								K[i][w] = 0;
								newCopies[i][w] = 0;
						} else if (wt[i-1] <= w) {
							c = K[i-1][w];
							Q = floor(w/wt[i-1]);
							j = 1;
							x = 0;
							while(j <= Q) {
								if(c < j*val[i-1] + K[i-1][w-(j*wt[i-1])]) {
									x = j;
								}
								c = max(c, j*val[i-1] + K[i-1][w-(j*wt[i-1])]);
								j++;
							}
							newCopies[i][w] = x;
							//printf("->%d (%d, %d)<-", newCopies[i][w], i , w);
							K[i][w] = c;
						} else {
								K[i][w] = K[i-1][w];
								newCopies[i][w] = 0;
						}
				}
			}
			// printf("------imprime el resultado de knapsack unbounded--------\n");
			// for (k = 0; k <= nCapacity; k++)
			// {
			// 		for ( j = 0; j < n+1; j++)
			// 		{
			// 			if (K[j][k] == INF)
			// 						printf("%7s", "INF");
			// 				else
			// 						printf ("%7d", K[j][k]);
			// 		}
			// 		printf("\n");
			// }
			// printf("------imprime el resultado de newCopies--------\n");
			// for (k = 0; k <= nCapacity; k++)
			// {
			// 		for ( j = 0; j < n+1; j++)
			// 		{
			// 			if (newCopies[j][k] == INF)
			// 						printf("%7s", "INF");
			// 				else
			// 						printf ("%7d", newCopies[j][k]);
			// 		}
			// 		printf("\n");
			// }
			create_solution_table_bounded(K,newCopies);
			getResults(newCopies, K);
}


void knapSack_Bounded(int W, int *wt, int *val, int *qua, int n){
	nCapacity = atoi(W);
	int K[n+1][nCapacity+1];
	int newCopies[n+1][nCapacity+1];
	int TnewCopies[nCapacity+1][n+1];
	int i, w, Q, j, x, k,d,c;

	c = 0;
	for (i = 0; i <= n; i++)
		{
			for (w = 0; w <= nCapacity; w++)
				{
						if (i==0 || w==0) {
								K[i][w] = 0;
								newCopies[i][w] = 0;
						} else if (wt[i-1] <= w) {
							c = K[i-1][w];
							Q = min(floor(w/wt[i-1]), qua[i-1]);
							j = 1;
							x = 0;
							while(j <= Q) {
								if(c < j*val[i-1] + K[i-1][w-(j*wt[i-1])]) {
									x = j;
								}
								c = max(c, j*val[i-1] + K[i-1][w-(j*wt[i-1])]);
								j++;
							}
							newCopies[i][w] = x;
							printf("->%d (%d, %d)<-", newCopies[i][w], i , w);
							K[i][w] = c;
						} else {
								K[i][w] = K[i-1][w];
								newCopies[i][w] = 0;
						}
				}
		}


		printf("------imprime el resultado de newCopies--------\n");
		for (k = 0; k <= nCapacity; k++)
		{
				for ( j = 0; j < n+1; j++)
				{
					if (newCopies[j][k] == INF)
								printf("%7s", "INF");
						else
								printf ("%7d", newCopies[j][k]);
				}
				printf("\n");
		}
		create_solution_table_bounded(K,newCopies);
		getResults(newCopies, K);
}


void knapSack(int W, int *wt, int *val, int n)
{
   int i, w,j,k;
	 printf("N:%d\n",n );
	 printf("%d\n", atoi(W));
	 nCapacity = atoi(W);
	 //for(i = 0; i < numbOfObj; i++) printf("%d\n",global_values[i]);

   int K[n+1][nCapacity+1];
	 int newCopies[n+1][nCapacity+1];

   // Build table K[][] in bottom up manner
   for (i = 0; i <= n; i++)
	 {

		 	//printf("%s\n","Entre al for" );
       for (w = 0; w <= nCapacity; w++)
       {
           if (i==0 || w==0){
						 K[i][w] = 0;
						 newCopies[i][w] = 0;
					 }
           else if (wt[i-1] <= w){
						 if(val[i-1] + K[i-1][w-wt[i-1]] > K[i-1][w] ){
							 K[i][w] = val[i-1] + K[i-1][w-wt[i-1]];
							 newCopies[i][w] = 1;
						 }
						 else{
							 K[i][w] = K[i-1][w];
							 newCopies[i][w] = 0;
						 }
					 }
           else{
						 K[i][w] = K[i-1][w];
						 newCopies[i][w] = 0;
					 }

       }
   }

	 //int i,j;
	 for (k = 0; k <= nCapacity; k++)
	 {
			 for ( j = 0; j < n+1; j++)
			 {
				 if (K[j][k] == INF)
							 printf("%7s", "INF");
					 else
							 printf ("%7d", K[j][k]);
			 }
			 printf("\n");
	 }

	 create_solution_table(K);
	 getResults(newCopies, K);

   //return K[n][W];
}

bool manual(){
	return f_manual == TRUE;
}

void defineKnapsackType(){
	int i;
	for(i = 0; i < numbOfObj; i++) {
		if(global_quantity[i] == INF){
			knapsack_type = 1; //unbounded knapSack
			break;
		}
		else if(global_quantity[i] != 1){
			knapsack_type = 2; // bounded knapSack
			break;
		}
		else{
			knapsack_type = 3; //knapSack 01
		}
	}
}



void solve_knapsack_problem(GtkWidget *widget, gpointer   data){
	GtkWidget  *entrada;
	entrada = gtk_entry_new();
	gchar* entrance;
	entrance = calloc(1, 500*sizeof(gchar));
	int i = 0;
	if(manual()){
		//Necesitamos dos matrices, una de datos y otra de columnas
		 column_names =  calloc(numbOfObj, 500*sizeof(gchar));
		//alojamos la memoria para cada espacio del char
		for (i = 0; i < numbOfObj; ++i) {
					column_names[i] = (char *)malloc(500);
		}
	}



	global_values = (int) calloc(numbOfObj, sizeof(int));
	global_weights = (int) calloc(numbOfObj, sizeof(int));
	global_quantity = (int) calloc(numbOfObj, sizeof(int));

	int fila, columna,value =0;
	i = 0;
	int j = 0;
	int k = 0;
	int x = 0;
	for(columna = 0; columna <4; columna++){
		for(fila = 1; fila <= numbOfObj; fila++){
			entrada = gtk_grid_get_child_at (data, columna, fila);

			if(columna == 0){
				g_stpcpy(column_names[x],gtk_entry_get_text(entrada));
				x++;
			}
		//	printf("Entrada[%d][%d] = %s\n",columna, fila, gtk_entry_get_text(entrada));
			else if(columna == 1){
				printf("Entrada[%d][%d] = %s\n",columna, fila, gtk_entry_get_text(entrada));
				// value = atoi(gtk_entry_get_text(entrada));
				// printf("Value = %d\n",value);
				g_stpcpy(entrance,gtk_entry_get_text(entrada));
				printf("ENTRANCE = %s\n", entrance);
				if(strcmp(entrance, "INF")==0){
					global_quantity[i] = 99999;
				}
				else{
					global_quantity[i]= atoi(entrance);
				}

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
	// printf("GLOBAL QUANTITIES\n");
	// for(i = 0; i < numbOfObj; i++) printf("%d\n",global_quantity[i]);
	// printf("GLOBAL WEIGHTS\n");
	// for(i = 0; i < numbOfObj; i++) printf("%d\n",global_weights[i]);
	// printf("GLOBAL VALUES\n");
	// for(i = 0; i < numbOfObj; i++) printf("%d\n",global_values[i]);
	//
	// printf("Prueba de nombre de objetos\n");
	// for (i = 0; i<numbOfObj ; i++) printf("Objeto %d = %s\n",i, column_names[i] );
	writeFile();
	defineKnapsackType();
	printf("El tipo de knapSack es : %d\n", knapsack_type );
	if(knapsack_type == 1){
		knapSack_UnBounded(knapsack_capacity, global_weights, global_values,numbOfObj);
	}
	else if(knapsack_type == 2){
		knapSack_Bounded(knapsack_capacity, global_weights, global_values,global_quantity ,numbOfObj);
	}
	else{
		knapSack(knapsack_capacity, global_weights, global_values, numbOfObj);
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
			if(matriz_datos[i][j] == INF){
				gtk_entry_set_text (pt_entrada, "INF");
			}
			else{
				gtk_entry_set_text (pt_entrada, pt_cell_value );
			}
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
	printf("Numero de lineas%d\n", numbOfObj);
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
					//printf("matriz_datos[%d][%d] = %d\n", fila, columna, atoi(matriz_datos_iniciales[k]) );
					if(strcmp(matriz_datos_iniciales[k], "INF")==0){
						matriz_datos[fila][columna] = 99999;
					}
					else{
						matriz_datos[fila][columna] = atoi(matriz_datos_iniciales[k]);
					}
					columna++;
				}
			}

			// printf("Prueba de nombre de objetos\n");
			// for (i = 0; i<numbOfObj ; i++) printf("Objeto %d = %s\n",i, column_names[i] );
			//
			printf("Prueba datos de la matriz\n" );
			printSolution(matriz_datos, numbOfObj);

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
		f_manual = TRUE;
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
