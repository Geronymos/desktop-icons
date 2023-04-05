#include <gtk/gtk.h>
#include <gio/gio.h>

static void setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
    GtkWidget *box;
    GtkWidget *image;
    GtkWidget *label;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    image = gtk_image_new();
    gtk_image_set_icon_size(GTK_IMAGE(image), GTK_ICON_SIZE_LARGE);
    gtk_box_append(GTK_BOX(box), image);
    label = gtk_label_new("");
    gtk_box_append(GTK_BOX(box), label);
    gtk_list_item_set_child(list_item, box);
}

static void bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
    GtkWidget *image;
    GtkWidget *label;
    GFileInfo *file_info;

    image = gtk_widget_get_first_child(gtk_list_item_get_child(list_item));
    label = gtk_widget_get_next_sibling(image);
    file_info = gtk_list_item_get_item(list_item);

    gtk_image_set_from_gicon(GTK_IMAGE(image), g_file_info_get_icon(file_info));
    gtk_label_set_label(GTK_LABEL(label), g_file_info_get_display_name(file_info));
}

static void open_app_done(GObject *source, GAsyncResult *result, gpointer data)
{
    GtkFileLauncher *launcher = GTK_FILE_LAUNCHER (source);
    GError *error = NULL;

    if (!gtk_file_launcher_launch_finish (launcher, result, &error))
    {
        g_print ("%s\n", error->message);
        g_error_free (error);
    }
}

// static void activate_cb(GtkListView  *list, guint position, gpointer unused)
// {
//     GFileInfo *file_info;
//     GtkFileLauncher *launcher;
//     GFile *file;
// 
//     file_info = g_list_model_get_item(G_LIST_MODEL(gtk_list_view_get_model(list)), position);
//     GtkWindow *parent = GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(list)));
// 
//     launcher = gtk_file_launcher_new(file_info);
//     g_file_info_
//     g_app_info_launch_default_for_uri
// 
//     gtk_file_launcher_launch(launcher, parent, NULL, open_app_done, NULL);
// 
//     g_object_unref(launcher);
//     g_object_unref(file_info);
// }

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window, *gridview;
    GListModel *model;
    GtkListItemFactory *factory;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);

    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_listitem_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_listitem_cb), NULL);

    const gchar* desktop_path = g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP);
    // GFile *path = g_file_parse_name("~/desktop");
    GFile *path = g_file_parse_name(desktop_path);

    model = G_LIST_MODEL(gtk_directory_list_new("standard::*,ownser::user", path));

    gridview = gtk_grid_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(model)), factory);

    gtk_window_set_child(GTK_WINDOW (window), gridview);

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

