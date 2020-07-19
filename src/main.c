#include <gtk/gtk.h>

#include "manager/api.h"

GtkWidget *main_window;

static void on_click_run_new_task(GtkWidget *widget, gpointer unused)
{
    GtkWidget *popup;

    popup = gtk_dialog_new_with_buttons(
        "New task",
        GTK_WINDOW(main_window),
        GTK_DIALOG_MODAL,
        "Run", 0,
        "Cancel", 1,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(popup));
    GtkWidget *text_input = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), text_input);

    gtk_widget_show_all(popup);

    int result = gtk_dialog_run(GTK_DIALOG(popup));
    g_print(
        "%s\n",
        gtk_entry_buffer_get_text(
            gtk_entry_get_buffer(
                text_input
            )
        )
    );
    gtk_widget_destroy(popup);
}

static void on_application_start(GtkApplication *app, gpointer user_data)
{
    GtkWidget *button_box;

    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 200, 200);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add(GTK_CONTAINER(main_window), button_box);

    {
        GtkWidget *button;
        button = gtk_button_new_with_label("Run new task");
        g_signal_connect(button, "clicked", G_CALLBACK(on_click_run_new_task), NULL);
        gtk_container_add(GTK_CONTAINER(button_box), button);
    }

    gtk_widget_show_all(main_window);
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