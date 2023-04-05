#include <gtk/gtk.h>
#include <gio/gio.h>

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window, *iconview;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);

    // iconview = gtk_grid_view_new(gtk_single_selection_new(0), );
    // gtk_window_set_child(GTK_WINDOW (window), iconview);

    GFile *path = g_file_parse_name("~/desktop");
    GFileEnumerator *children = g_file_enumerate_children(path, "standard::*,ownser::user", 0, 0, 0);
    
    GtkIconTheme *theme = gtk_icon_theme_get_for_display(gtk_widget_get_display(window));

    GFileInfo *info;
    while ( (info = g_file_enumerator_next_file(children, 0, 0)) ) {
        GIcon *icon = g_file_info_get_icon(info);
        const char *name = g_file_info_get_name(info);
        GtkIconPaintable *pixbuf = gtk_icon_theme_lookup_by_gicon(theme, icon, 48, 1, 0, 0);

        printf("%s\n", name);
    }

    gtk_window_present(GTK_WINDOW (window));
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

