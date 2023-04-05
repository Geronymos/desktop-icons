#include <gtk/gtk.h>
#include <gio/gio.h>


static GListModel *create_desktop_list(void)
{
    GListStore *store;

    store = g_list_store_new(G_TYPE_FILE_INFO);

    GFile *path = g_file_parse_name("~/desktop");
    GFileEnumerator *children = g_file_enumerate_children(path, "standard::*,ownser::user", 0, 0, 0);
    
    GFileInfo *info;
    while ( (info = g_file_enumerator_next_file(children, 0, 0)) ) {
        g_list_store_append(store, info);
    }

    return G_LIST_MODEL(store);

}

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

    model = create_desktop_list();

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

