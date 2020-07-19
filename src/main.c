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
        "Run", GTK_RESPONSE_OK,
        "Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(popup));
    GtkWidget *text_input = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), text_input);

    gtk_widget_show_all(popup);

    int result = gtk_dialog_run(GTK_DIALOG(popup));
    int command_length = gtk_entry_get_text_length(GTK_ENTRY(text_input));
    const char *gtk_text_buffer = gtk_entry_buffer_get_text(
        GTK_ENTRY_BUFFER(gtk_entry_get_buffer(
            GTK_ENTRY(text_input)
        ))
    );
    char command_line[command_length + 1];
    strcpy(command_line, gtk_text_buffer);
    gtk_widget_destroy(popup);

    TaskConfig task_config;
    switch (result)
    {
        case GTK_RESPONSE_OK:            
            {
                char* tokens[sizeof(char*) * command_length];
                int n_tokens;
                {
                    char *token = strtok(&command_line, " ");
                    int index = 0;
                    while (token)
                    {
                        tokens[index++] = token;
                        token = strtok(NULL, " ");
                    }
                    n_tokens = index;
                }
                task_config.command = tokens[0];
                task_config.arguments = &tokens[1];
                task_config.n_arguments = n_tokens - 1;
                task_config.cwd = "/home/jack";

                start("/home/jack/Code/Tasks/Codebase/dist/taskshome", &task_config);
            }
        break;
          
        case GTK_RESPONSE_CANCEL:
        case GTK_RESPONSE_CLOSE:
        default:
        break;
    }
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