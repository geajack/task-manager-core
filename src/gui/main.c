#include <gtk/gtk.h>

#include "Application.i"

static void on_application_start(GtkApplication *gtk_app, Application *app)
{
    GtkWidget *main_window = gtk_application_window_new(gtk_app);
    application_initialize(app, main_window);
}

int main(int argc, char **argv)
{
    Application app;
    app.home = "/home/jack/.local/share/tasks/home";

    GtkApplication *gtk_app;
    gtk_app = gtk_application_new("com.github.geajack.tasks", G_APPLICATION_FLAGS_NONE); 
    g_signal_connect(gtk_app, "activate", G_CALLBACK(on_application_start), &app);
    int status = g_application_run(G_APPLICATION(gtk_app), argc, argv);
    application_destroy(&app);
    g_object_unref(gtk_app);

    return status;
}