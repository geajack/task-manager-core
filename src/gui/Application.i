#include <gtk/gtk.h>

#include "../manager/api.h"

typedef struct Application
{
    const char *home;
    GtkWidget *main_window;

} Application;

void application_show_new_task_popup(Application *self);
void application_refresh_view(Application *self);

static void on_click_run_new_task(GtkWidget *widget, Application *app)
{
    application_show_new_task_popup(app);
}

void application_initialize(Application *self, GtkWidget *main_window)
{
    gtk_window_set_title(GTK_WINDOW(main_window), "Tasks");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 400);

    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *task_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *scrollable_area = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrollable_area, TRUE);
    gtk_widget_set_vexpand(scrollable_area, TRUE);
    
    gtk_container_add(GTK_CONTAINER(main_window), layout);
    gtk_widget_set_margin_start(layout, 5);
    gtk_widget_set_margin_top(layout, 5);
    gtk_widget_set_margin_end(layout, 5);
    gtk_widget_set_margin_bottom(layout, 5);

    GtkWidget *title = gtk_label_new("Running tasks");

    gtk_container_add(GTK_CONTAINER(layout), title);
    gtk_container_add(GTK_CONTAINER(layout), scrollable_area);
    gtk_container_add (GTK_CONTAINER(scrollable_area), task_box);
    gtk_container_add(GTK_CONTAINER(layout), button_box);
    gtk_widget_set_vexpand(task_box, TRUE);
    gtk_widget_set_vexpand(button_box, FALSE);

    StartedTasksList running_tasks;
    get_running_tasks(self->home, &running_tasks);

    for (int i = 0; i < running_tasks.count; i++)
    {
        GtkWidget *button;
        button = gtk_button_new_with_label(running_tasks.tasks[i].label);
        gtk_box_pack_start(GTK_BOX(task_box), button, FALSE, FALSE, 2);
    }

    {
        GtkWidget *button;
        button = gtk_button_new_with_label("Run new task");
        gtk_widget_set_hexpand(button, TRUE);
        gtk_widget_set_vexpand(button, TRUE);
        g_signal_connect(button, "clicked", G_CALLBACK(on_click_run_new_task), self);
        gtk_container_add(GTK_CONTAINER(button_box), button);
    }

    gtk_widget_show_all(main_window);
}

void application_refresh_view(Application *self)
{
    
}

void application_show_new_task_popup(Application *self)
{
    GtkWidget *popup;

    popup = gtk_dialog_new_with_buttons(
        "New task",
        GTK_WINDOW(self->main_window),
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

    LaunchConfiguration launch_config;
    switch (result)
    {
        case GTK_RESPONSE_OK:
            launch_config.command = command_line;
            launch_config.cwd = "/home/jack";
            start(self->home, &launch_config);
        break;
          
        case GTK_RESPONSE_CANCEL:
        case GTK_RESPONSE_CLOSE:
        default:
        break;
    }

    application_refresh_view(self);
}

void application_destroy(Application *self)
{

}