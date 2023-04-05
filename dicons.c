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

static GtkListStore *create_desktop_list(void)
{
    GtkListStore *store;
    GtkTreeIter iter;
    GDir *dir;
    GFile *file;
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

    return GTK_LIST_STORE(store);
}

// static void setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
// {
//     GtkWidget *box;
//     GtkWidget *image;
//     GtkWidget *label;
// 
//     box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
//     image = gtk_image_new();
//     gtk_image_set_icon_size(GTK_IMAGE(image), GTK_ICON_SIZE_LARGE);
//     gtk_box_append(GTK_BOX(box), image);
//     label = gtk_label_new("");
//     gtk_box_append(GTK_BOX(box), label);
//     gtk_list_item_set_child(list_item, box);
// }
// 
// static void bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
// {
//     GtkWidget *image;
//     GtkWidget *label;
//     GFile *file;
//     GFileInfo *file_info;
// 
//     image = gtk_widget_get_first_child(gtk_list_item_get_child(list_item));
//     label = gtk_widget_get_next_sibling(image);
//     file = gtk_list_item_get_item(list_item);
//     file_info = g_file_query_info(file, "standard::*,ownser::user", 0, 0, 0);
// 
//     gtk_image_set_from_gicon(GTK_IMAGE(image), g_file_info_get_icon(file_info));
//     gtk_label_set_label(GTK_LABEL(label), g_file_info_get_display_name(file_info));
// }
// 
// static void activate_cb(GtkGridView  *list, guint position, gpointer unused)
// {
//     GdkAppLaunchContext *context;
//     GFile *file;
//     char* file_uri = g_file_get_uri(file);
// 
//     file = g_list_model_get_item(G_LIST_MODEL(gtk_grid_view_get_model(list)), position);
//     printf("uri %s\n", file_uri);
// 
//     g_app_info_launch_default_for_uri(file_uri, 0, 0);
// 
//     g_object_unref(file);
// }

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window, *icon_view;
       // , *gridview;
    GtkListStore *model;
    // GtkListItemFactory *factory;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);

    gtk_layer_init_for_window(GTK_WINDOW(window));
    gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_BOTTOM);

    // factory = gtk_signal_list_item_factory_new();
    // g_signal_connect(factory, "setup", G_CALLBACK(setup_listitem_cb), NULL);
    // g_signal_connect(factory, "bind", G_CALLBACK(bind_listitem_cb), NULL);

    model = create_desktop_list();
    icon_view = gtk_icon_view_new_with_model(GTK_TREE_MODEL(model));

    gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (icon_view),
            GTK_SELECTION_MULTIPLE);
    gtk_icon_view_set_text_column(GTK_ICON_VIEW (icon_view), COL_DISPLAY_NAME);
    gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW (icon_view), COL_PIXBUF);
    // gridview = gtk_grid_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(model)), factory);

    // g_signal_connect(gridview, "activate", G_CALLBACK(activate_cb), NULL);

    gtk_container_add(GTK_CONTAINER(window), icon_view);
    gtk_widget_grab_focus (icon_view);

    // gtk_window_present(GTK_WINDOW (window));

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

