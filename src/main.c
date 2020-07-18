#include <gtk/gtk.h>

static void print_message(GtkWidget *widget, char const* message)
{
    g_print(message);
}

static void on_application_start(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *button_box;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(window), button_box);

    {
        GtkWidget *button;
        button = gtk_button_new_with_label("Run new task");
        g_signal_connect(button, "clicked", G_CALLBACK(print_message), "Button 1 clicked\n");
        gtk_container_add(GTK_CONTAINER(button_box), button);
    }

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.github.geajack.tasks", G_APPLICATION_FLAGS_NONE); 
    g_signal_connect(app, "activate", G_CALLBACK(on_application_start), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}