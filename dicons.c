#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gtk-layer-shell/gtk-layer-shell.h>

enum
{
    COL_PIXBUF,
    COL_DISPLAY_NAME,
    COL_FILE,
    COL_FILE_INFO,
    NUM_COLS
};

GtkListStore *store;

// Callback function for file created signal
static void file_changed_cb(GFileMonitor *monitor, GFile *file, GFile *other_file, GFileMonitorEvent evtype, gpointer user_data)
{
    char *fpath = g_file_get_path(file);
    char *opath = NULL;
    if (other_file) {
        opath = g_file_get_path(other_file);
    }

    switch(evtype) {
        case G_FILE_MONITOR_EVENT_DELETED:
            g_print("%s deleted\n", fpath);
            break;
        case G_FILE_MONITOR_EVENT_CREATED:
            g_print("%s created\n", fpath);
            break;
    }
    if (opath) {
        g_free(opath);
    }
    g_free(fpath);
}

static GtkListStore *create_desktop_list(void)
{
    GtkTreeIter iter;
    GDir *dir;
    GFile *file, *dir_file;
    GFileMonitor *monitor;
    GFileInfo *file_info;
    const gchar *file_name, *display_name;
    GtkIconTheme *theme;
    GdkPixbuf *pixbuf;

    const gchar* desktop_path = g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP);
    printf("n %s\n", desktop_path);

    theme = gtk_icon_theme_get_default();

    store = gtk_list_store_new(NUM_COLS,
            GDK_TYPE_PIXBUF,
            G_TYPE_STRING,
            G_TYPE_FILE,
            G_TYPE_FILE_INFO);

    dir = g_dir_open(desktop_path, 0, 0);
    
    dir_file = g_file_new_for_path(desktop_path);
    // monitor = g_file_monitor_directory(dir_file, G_FILE_MONITOR_WATCH_MOVES, NULL, NULL);
    monitor = g_file_monitor_directory(dir_file, G_FILE_MONITOR_NONE, NULL, NULL);

    while ( (file_name = g_dir_read_name(dir)) ) {
        printf("g %s\n", file_name);

        file = g_file_new_for_path(g_build_filename(desktop_path, file_name, NULL));
        display_name = g_filename_to_utf8(file_name, -1, 0, 0, 0);
        file_info = g_file_query_info(file, "standard::*,ownser::user", 0, 0, 0);
        pixbuf = gtk_icon_info_load_icon(
            gtk_icon_theme_lookup_by_gicon(
                    theme,
                    g_file_info_get_icon(file_info),
                    48, 0), 0);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                COL_PIXBUF, pixbuf,
                COL_DISPLAY_NAME, display_name,
                COL_FILE, file, 
                COL_FILE_INFO, file_info,
                -1
                );
    }

    g_signal_connect(monitor, "changed", G_CALLBACK(file_changed_cb), NULL);

    return GTK_LIST_STORE(store);
}

static void activate_cb(GtkIconView *icon_view, GtkTreePath *tree_path, gpointer user_data)
{
    GtkListStore *store;
    GtkTreeIter iter;
    GFile *file;
    char* file_uri; 
    store = GTK_LIST_STORE (user_data);

    gtk_tree_model_get_iter (GTK_TREE_MODEL (store), &iter, tree_path);

    gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, COL_FILE, &file, -1);

    file_uri = g_file_get_uri(file);
    printf("uri %s\n", file_uri);

    g_app_info_launch_default_for_uri(file_uri, 0, 0);
}

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window, *icon_view;
    GtkListStore *model;
    GdkScreen *screen;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Window");

    gtk_layer_init_for_window(GTK_WINDOW(window));
    gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_BOTTOM);

    for (int anchor = 0; anchor < 4; anchor++) 
        gtk_layer_set_anchor(GTK_WINDOW(window), anchor, 1); 

    gtk_layer_set_margin(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, 20);

    model = create_desktop_list();
    icon_view = gtk_icon_view_new_with_model(GTK_TREE_MODEL(model));

    gtk_widget_override_background_color(icon_view, 0, &(GdkRGBA){0,0,0,0});
    gtk_widget_override_background_color(window, 0, &(GdkRGBA){0,0,0,0});

    gtk_icon_view_set_item_width(GTK_ICON_VIEW(icon_view), 96);
    gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (icon_view),
            GTK_SELECTION_MULTIPLE);
    gtk_icon_view_set_text_column(GTK_ICON_VIEW (icon_view), COL_DISPLAY_NAME);
    gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW (icon_view), COL_PIXBUF);

    g_signal_connect(icon_view, "item-activated", G_CALLBACK(activate_cb), model);

    gtk_container_add(GTK_CONTAINER(window), icon_view);
    gtk_widget_grab_focus (icon_view);

    gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}

