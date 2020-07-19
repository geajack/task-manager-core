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
    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Tasks");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 400);

    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *scrollable_area = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrollable_area, TRUE);
    gtk_widget_set_vexpand(scrollable_area, TRUE);
    
    gtk_container_add(GTK_CONTAINER(main_window), layout);
    gtk_widget_set_margin_left(layout, 5);
    gtk_widget_set_margin_top(layout, 5);
    gtk_widget_set_margin_right(layout, 5);
    gtk_widget_set_margin_bottom(layout, 5);

    GtkWidget *title = gtk_label_new("Running tasks");

    gtk_container_add(GTK_CONTAINER(layout), title);
    gtk_container_add(GTK_CONTAINER(layout), scrollable_area);
    gtk_container_add (GTK_CONTAINER(scrollable_area), task_box);
    gtk_container_add(GTK_CONTAINER(layout), button_box);
    gtk_widget_set_vexpand(task_box, TRUE);
    gtk_widget_set_vexpand(button_box, FALSE);

    for (int i = 0; i < 20; i++)
    {
        GtkWidget *button;
        button = gtk_button_new_with_label("Task");
        gtk_box_pack_start(GTK_BOX(task_box), button, FALSE, FALSE, 2);
    }

    {
        GtkWidget *button;
        button = gtk_button_new_with_label("Run new task");
        gtk_widget_set_hexpand(button, TRUE);
        gtk_widget_set_vexpand(button, TRUE);
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