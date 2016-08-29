#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *label_test;
int pid;

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

      /*--- CSS -----------------*/
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
  /*-------------------------*/

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/try.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "dynamicProgramming_win"));
    gtk_builder_connect_signals(builder, NULL);


    label_test = GTK_WIDGET(gtk_builder_get_object(builder, "label_test"));

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

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    if(pid == 0){
       kill(pid, SIGTERM);

    }
    gtk_main_quit();

}

void on_btn_algoritmo1_clicked()
{
    //gtk_label_set_text(GTK_LABEL(label_test), "Esto es una pruebaa!");
    /*system("cd /home/compiladores/GUI/src");*/
    //system("gcc pending_program.c -o pending_program -pthread pkg-config -cflags -libs gtk+-3.0 -export-dynamic");
    //system("gcc -o pending_program pending_program.o");
    system("./floyd &");

}

void btn_algoritmo2_clicked_cb(){
    system("./knapsack &");
}
void btn_algoritmo3_clicked_cb(){
    system("./pending_program &");
}
void btn_algoritmo4_clicked_cb(){
    system("./pending_program &");
}
void btn_algoritmo5_clicked_cb(){
    system("./pending_program &");
}
void btn_algoritmo6_clicked_cb(){
    system("./pending_program &");
}
void btn_algoritmo7_clicked_cb(){
    on_window_main_destroy();
}
