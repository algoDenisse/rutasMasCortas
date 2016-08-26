#include <gtk/gtk.h>
#include <stdbool.h>
#define INF 99999
GtkWidget       *entry_grid_size;
GtkWidget *PTable_window, *combobox1, *combobox2;
GtkWidget *table_pt;
int n;
int numeroDeTabla= 1;
int **optimal_routes_mtx;
int **global_distance_mtx;
char **column_names;
const char *alphabet[27]={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","Ñ","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

gchar pointA[20];
gchar pointB[20];

static void generate_D1 (GtkWidget *widget, gpointer   data){

    printf("1. Click en D1\n" );

    GtkWidget  *entrada;
    gchar* entrance;
    entrada = gtk_entry_new();
    int i;
    //alojar memoria matriz global
    global_distance_mtx = calloc(n, 1+sizeof(int*));
    optimal_routes_mtx = calloc(n, 1+sizeof(int*));
    for (i = 0; i < n; ++i) {
        global_distance_mtx[i] = calloc(n,sizeof(int));
        optimal_routes_mtx[i] = calloc(n,sizeof(int));
    }
    //alojamos la menmoria para el array
    column_names =  calloc(n, 500*sizeof(gchar));
    //alojamos la memoria para cada espacio del char

    for (i = 0; i < n; ++i) {
        column_names[i] = (char *)malloc(500);
    }
    entrance = calloc(1, 500*sizeof(gchar));

    //alojamos memoria de lamatriz de distancias
    int **matriz_distancias = calloc(n, 1+sizeof(int*));

    for (i = 0; i < n; ++i) {
        matriz_distancias[i] = calloc(n,sizeof(int));
    }
    int j,k;
    i=0;
    for(k =0; k< n+1;k++){
      //printf("Entro al primer FOR\n" );
      for(j=0;j<n+1;j++){
        //printf("Entro al segundo FOR\n" );
        entrada = gtk_grid_get_child_at (data, k, j);
        g_stpcpy(entrance,gtk_entry_get_text(entrada));
        //Guardamos valor de las column_names
        if (k !=0 && j == 0){

          strcpy(column_names[i],entrance);
        //  printf("Nombre de la columna posicion %d, %d = %s\n",j, k ,entrance);
          i++;
        }
        //GUardamos el valor de los numeros en la matriz
        if (k!=0 && j!=0){
          if (strcmp(entrance, "INF")==0){
            //printf("%s\n","Encontre un infinito!" );
            matriz_distancias[j-1][k-1]  = 99999;
          }
          else{
            matriz_distancias[j-1][k-1] = atoi(entrance);
            //  printf("Valor de la entrada %d, %d = %d\n",j, k, atoi(entrance));
          }

        }
    }
}
  for (i = 0; i<n ; i++){
    for(j = 0; j< n; j++ ){
      optimal_routes_mtx[i][j] = 0;
    }
  }
  create_PTable();
  generate_Dn(matriz_distancias, numeroDeTabla);
  create_new_grid(global_distance_mtx, numeroDeTabla);
  gtk_widget_set_sensitive (widget, FALSE);

  free(entrance);
 //free_memory(matriz_distancias, column_names);
}

void guardarMatrizDistancia (GtkWidget *widget, gpointer   data){
  GtkWidget  *entrada;
  gchar* entrance;
  entrada = gtk_entry_new();
  int i;
  entrance = calloc(1, 500*sizeof(gchar));

  column_names =  calloc(n, 500*sizeof(gchar));
  for (i = 0; i < n; ++i) {
      column_names[i] = (char *)malloc(500);
  }

  //alojamos memoria de lamatriz de distancias
  int **matriz_distancias = calloc(n, 1+sizeof(int*));

  for (i = 0; i < n; ++i) {
      matriz_distancias[i] = calloc(n,sizeof(int));
  }
  int j,k;
  i=0;
  for(k =0; k< n+1;k++){
    //printf("Entro al primer FOR\n" );
    for(j=0;j<n+1;j++){
      //printf("Entro al segundo FOR\n" );
      entrada = gtk_grid_get_child_at (data, k, j);
      g_stpcpy(entrance,gtk_entry_get_text(entrada));
      printf("D2 _%s\n",entrance);

      if (k !=0 && j == 0){
        strcpy(column_names[i],entrance);
      //  printf("Nombre de la columna posicion %d, %d = %s\n",j, k ,entrance);
        i++;
      }

      // //GUardamos el valor de los numeros en la matriz
      if (k!=0 && j!=0){
        if (strcmp(entrance, "INF")==0){
          //printf("%s\n","Encontre un infinito!" );
          matriz_distancias[j-1][k-1]  = 99999;
        }
        else{
          matriz_distancias[j-1][k-1] = atoi(entrance);
          //  printf("Valor de la entrada %d, %d = %d\n",j, k, atoi(entrance));
        }

      }
  }

}
printf("numeroDeTabla %d\n",numeroDeTabla );
//Llevar un contador global! de las iteraciones y parar cunado se llegue a n

 if (numeroDeTabla > n){
   printf("No hay mas tablas por generar\n" );

 }
 else{
   generate_Dn(matriz_distancias, numeroDeTabla);
   create_new_grid(global_distance_mtx, numeroDeTabla);
 }
gtk_widget_set_sensitive (widget, FALSE);
 // free(entrance);
 //free_memory(matriz_distancias, column_names);
}

void create_PTable(){
  printf("2. Creacion del grid de la matriz P\n");
  PTable_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (PTable_window), "Tabla P");
  // create new table
  table_pt= gtk_grid_new();
  gtk_grid_set_row_spacing (GTK_GRID (table_pt), 2);
  gtk_container_add (GTK_CONTAINER (PTable_window), table_pt);

  //alojamos espacio para cada entrada de la matriz
  GtkWidget ***entrada;
  int j,k,i;
  entrada=calloc(n+1,sizeof(GtkWidget**));
  for(j = 0; j < n+1; j++){
    entrada[j]=calloc(n+1,sizeof(GtkWidget*));
  }

  for(k =0; k< n+1;k++){
    for(j=0;j<n+1;j++){

      entrada[k][j]= gtk_entry_new ();
      gtk_widget_set_sensitive (entrada[k][j], FALSE);
      gtk_widget_set_name(entrada[k][j], "tp_cell");
      gtk_grid_attach (GTK_GRID (table_pt),entrada[k][j] , k, j, 1, 1);

      if(k == 0 && j == 0){
        gtk_widget_set_name(entrada[k][j], "column_label");
      }
      if (k == 0 && j != 0){
         gtk_widget_set_name(entrada[k][j], "column_name");
         gtk_entry_set_text (entrada[k][j],column_names[j-1]);
      }
      if (j ==0 && k!=0){
        gtk_widget_set_name(entrada[k][j], "column_name");
        gtk_entry_set_text(entrada[k][j],column_names[k-1]);
      }

      if (k != 0 && j != 0){
                 gtk_entry_set_text (entrada[k][j], "0");
                 //actualizo matriz de PTable

      }
    }
  }
  gtk_widget_show_all(PTable_window);
}

void getBestRoute(int puntoA, int puntoB){
   int stop;
   stop = optimal_routes_mtx[puntoA-1][puntoB-1];
   if (stop==0){
     printf("Vaya de Punto %d  a Punto %d\n",puntoA, puntoB);
   }
   else{
     printf("Vaya de Punto %d a Punto %d\n",puntoA,stop);
      getBestRoute(puntoA,stop);
      getBestRoute(stop,puntoB);
   }
}




void getOptimalRoutes(GtkWidget *widget, gpointer   data){
  printf("4. Dar Rutas Optimas\n");
  printSolution(data);
  int i, j;
  for(i = 1; i < n ; i++){
    for (j= 1; j<n; j++){
      printf("\n\nRuta Optima de Punto %d ----> Punto %d: \n", i, j);
      getBestRoute(i, j);
    }
  }
    // GtkBuilder      *dialog_builder;
    // GtkWidget       *dialog_window, *combobox1, *combobox2, *button;
    // gchar columna[20];
    //
    // dialog_builder = gtk_builder_new();
    // gtk_builder_add_from_file (dialog_builder, "glade/Dialog.glade", NULL);
    //
    // dialog_window = GTK_WIDGET(gtk_builder_get_object(dialog_builder, "dialog_window"));
    // gtk_builder_connect_signals(dialog_builder,NULL);
    //
    // //llenar los combobox
    // combobox1 =  GTK_WIDGET(gtk_builder_get_object(dialog_builder, "comboboxtext1"));
    // combobox2 =  GTK_WIDGET(gtk_builder_get_object(dialog_builder, "comboboxtext2"));
    //
    // for (i = 0; i < n; i ++){
    //   strcpy(columna, column_names[i]);
    //   gtk_combo_box_text_append (combobox1,NULL, columna);
    //   gtk_combo_box_text_append (combobox2,NULL, columna);
    // }
    //
    // g_object_unref(dialog_builder);
    //
    // gtk_widget_show_all(dialog_window);

}

void on_search_routes_clicked(){

  // printf("VAlor de pointA %d\n",gtk_combo_box_get_active(combobox1) );
  // printf("VAlor de pointb %s\n",gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox2)) );
  //
  // int i,j, pos1, pos2;
  // for(i= 0; i<n; i++){
  //   if(strcmp(column_names[i], gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox1)))==0)
  //       pos1 = i+1;
  //   if(strcmp(column_names[i], gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox2)))==0)
  //           pos2 = i+1;
  // }

}

void create_new_grid(int **global_distance_mtx, int table_number){
  GtkWidget *window;
  GtkWidget *table;
  GtkWidget *button;
  GtkWidget *button_box;

  char cell_value[5];


  /* create a new window */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   char title[20];
   char str[5];
   strcpy(title, "D(");
   snprintf(str,5,"%d",table_number);
   strcat(title, str);
   strcat(title, ")");
   gtk_window_set_title (GTK_WINDOW (window), title);

   // create new table
   table= gtk_grid_new();
   gtk_grid_set_row_spacing (GTK_GRID (table), 2);
   gtk_container_add (GTK_CONTAINER (window), table);

   //alojamos espacio para cada entrada de la matriz
   GtkWidget ***entrada;

   int j,k,i;
   entrada=calloc(n+1,sizeof(GtkWidget**));
   for(j = 0; j < n+1; j++){
     entrada[j]=calloc(n+1,sizeof(GtkWidget*));
   }

   for(k =0; k< n+1;k++){
     for(j=0;j<n+1;j++){

       entrada[k][j]= gtk_entry_new ();
       gtk_widget_set_sensitive (entrada[k][j], FALSE);
       gtk_grid_attach (GTK_GRID (table),entrada[k][j] , k, j, 1, 1);

       if(k==j && k != 0 && j != 0){
         gtk_entry_set_text(entrada[k][j],"0");
       }
       if (k == 0 && j != 0){
          gtk_widget_set_name(entrada[k][j], "column_name");
          gtk_entry_set_text (entrada[k][j],column_names[j-1]);
       }
       if (j ==0 && k!=0){
         gtk_widget_set_name(entrada[k][j], "column_name");
         gtk_entry_set_text(entrada[k][j],column_names[k-1]);
       }

       if(k!=0 && j!=0){
          if (global_distance_mtx[k-1][j-1] == INF)
                  strcpy(cell_value, "INF");
          else{
                 snprintf(cell_value,5,"%d",global_distance_mtx[j-1][k-1]);
          }
          gtk_entry_set_text (entrada[k][j], cell_value);

       }

     }
   }

   button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
   gtk_grid_attach (GTK_GRID (table),button_box , n/2, j, 1, 1);

   if (table_number<n){
     strcpy(title, "Generar D(");
     snprintf(str,5,"%d",table_number+1);
     strcat(title, str);
     strcat(title, ")");

     button = gtk_button_new_with_label (title);
     g_signal_connect (button, "clicked", G_CALLBACK (guardarMatrizDistancia), (gpointer) table);
     gtk_container_add (GTK_CONTAINER (button_box), button);

     //funcion guardarMatrizDistancia(button, table);
     numeroDeTabla ++;
   }
   else{
     button = gtk_button_new_with_label ("Obtener Rutas");
     g_signal_connect (button, "clicked", G_CALLBACK (getOptimalRoutes), (gpointer) optimal_routes_mtx);
     gtk_container_add (GTK_CONTAINER (button_box), button);
   }


   gtk_widget_show_all(window);

}



void update_PTable(){
  printf("%s\n","3. Modifico el P table" );
  GtkWidget  *pt_entrada;
  pt_entrada = gtk_entry_new();
  char pt_cell_value[5];
  // modificar entrada de la P table
  int i, j;
  for(i = 0; i < n ; i ++){
    for (j = 0; j<n; j++){
      pt_entrada = gtk_grid_get_child_at (table_pt, j+1, i+1);
      snprintf(pt_cell_value,5,"%d",optimal_routes_mtx[i][j]);
      gtk_entry_set_text (pt_entrada, pt_cell_value );
      if (optimal_routes_mtx[i][j] != 0) gtk_widget_set_name(pt_entrada, "new_val");
      printf("Entrada %d, %d = optimal_routes_mtx[%d][%d] = %d \n",j+1, i+1, i, j,optimal_routes_mtx[i][j] );
    }
  }
}


void free_memory(int **graph, char **nodes){
  //Libera los datos de la matriz_distancias
  free(graph);
  free(nodes);

}

void generate_Dn(int  **graph, int table_number){
  //update_previous_global_mtx(graph);
  printf("3. Calculo FLoyd y hago la nueva matriz\n" );
  int dist[n][n], i, j, k;

   for (i = 0; i < n; i++)
       for (j = 0; j < n; j++)
           dist[i][j] = graph[i][j];

       for (i = 0; i < n; i++)
       {
           for (j = 0; j < n; j++)
           {
               if(i != table_number -1 && j != table_number -1){
                 if (dist[i][table_number-1] + dist[table_number-1][j] < dist[i][j]){
                      dist[i][j] = dist[i][table_number-1] + dist[table_number-1][j];
                      optimal_routes_mtx[i][j] = table_number;
                 }
               }
           }
   }

   printf("Ruta OPtima So far\n");
   printSolution(optimal_routes_mtx);

   //copio la solucion en el global distance matrix
   update_global_mtx(dist);
   update_PTable();

}


void update_global_mtx(int dist[][n]){
  int i, j;
  for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
          global_distance_mtx[i][j] = dist[i][j];
}

void printSolution(int  **dist)
{
    int i,j;
    for (i = 0; i < n; i++)
    {
        for ( j = 0; j < n; j++)
        {
          if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf ("%7d", dist[i][j]);
        }
        printf("\n");
    }
}


void button_crear_grid_clicked_cb(){

  GtkBuilder      *floyd_builder;
  GtkWidget       *floyd_window;
  GtkWidget       *table;
  GtkWidget       *button;
  GtkWidget       *button_box;


  /*--- CSS -----------------*/
GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;
/*-------------------------*/

  floyd_builder = gtk_builder_new();
  gtk_builder_add_from_file (floyd_builder, "glade/floyd_window.glade", NULL);

  floyd_window = GTK_WIDGET(gtk_builder_get_object(floyd_builder, "floyd_window"));
  gtk_builder_connect_signals(floyd_builder, NULL);

  table= gtk_grid_new();
  gtk_grid_set_row_spacing (GTK_GRID (table), 2);
  gtk_container_add (GTK_CONTAINER (floyd_window), table);


  GtkWidget ***entrada;

  printf("%s\n",gtk_entry_get_text (entry_grid_size) );

  n = atoi(gtk_entry_get_text (entry_grid_size));

  int j,k,i;
  entrada=calloc(n+1,sizeof(GtkWidget**));
  for(j = 0; j < n+1; j++){
    entrada[j]=calloc(n+1,sizeof(GtkWidget*));
  }
  for(k =0; k< n+1;k++){
    for(j=0;j<n+1;j++){

      entrada[k][j]= gtk_entry_new();
      gtk_grid_attach (GTK_GRID (table),entrada[k][j] , k, j, 1, 1);
      if(k==j && k != 0 && j != 0){
        gtk_entry_set_text (entrada[k][j],"0");
        gtk_widget_set_sensitive (entrada[k][j], FALSE);
      }
      if (k == 0 && j != 0){
        gtk_widget_set_name(entrada[k][j], "column_name");
        gtk_entry_set_text (entrada[k][j],alphabet[j-1]);
      }
      if (j ==0 && k!=0){
        gtk_widget_set_name(entrada[k][j], "column_name");
        gtk_entry_set_text (entrada[k][j],alphabet[k-1]);
      }
    }
  }

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_grid_attach (GTK_GRID (table),button_box , n/2, j, 1, 1);

  button = gtk_button_new_with_label ("Generar D(1)");
  g_signal_connect (button, "clicked", G_CALLBACK (generate_D1), (gpointer) table);
  gtk_container_add (GTK_CONTAINER (button_box), button);


  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

  GError *error = 0;

  gtk_css_provider_load_from_path (provider,
                                    g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
                                  NULL);

  g_object_unref(floyd_builder);

  gtk_widget_show_all(floyd_window);

}



int main(int argc, char *argv[])
{

  GtkBuilder      *initial_floyd_builder;
  GtkWidget       *initial_floyd_window;

  /*--- CSS -----------------*/
GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;
/*-------------------------*/

  gtk_init(&argc, &argv);

  initial_floyd_builder = gtk_builder_new();
  gtk_builder_add_from_file (initial_floyd_builder, "glade/initial_floyd.glade", NULL);

  initial_floyd_window = GTK_WIDGET(gtk_builder_get_object(initial_floyd_builder, "initial_floyd_window"));
  entry_grid_size = GTK_WIDGET(gtk_builder_get_object(initial_floyd_builder, "entry_grid_size"));


  gtk_builder_connect_signals(initial_floyd_builder,NULL);

  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "css/style.css";  // your path, for instance: /home/zerohour/Documents/programming/cssexternal.css

  GError *error = 0;

  gtk_css_provider_load_from_path (provider,
                                    g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
                                    NULL);

  g_object_unref(initial_floyd_builder);



  gtk_widget_show_all(initial_floyd_window);
  gtk_main();

  return 0;

}
