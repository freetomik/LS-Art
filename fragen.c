#include <gtk/gtk.h>

static gboolean do_draw(GtkWidget *draw, cairo_t *cc, gpointer data)
{
	GtkAllocation alloc;

	gtk_widget_get_allocation(draw, &alloc);
	// g_print("width=%d height=%d\n", alloc.width, alloc.height);
	cairo_set_antialias(cc, CAIRO_ANTIALIAS_DEFAULT);

	cairo_new_path(cc);	/* nova kresba */
	cairo_line_to(cc, 5, 5);
	cairo_line_to(cc, 100, 20);
	cairo_line_to(cc, 20, 100);
	cairo_close_path(cc);	/* ukoncit cestu */

	/* cairo_fill/stroke ukoncuje kresbu */
	cairo_stroke(cc);

	return TRUE; /* neprobublavat udalost vyse */
}

static gboolean resize(GtkWidget *draw, GtkAllocation *alloc, gpointer data)
{
	// g_print("width=%d height=%d\n", alloc->width, alloc->height);
	return TRUE;
}

int main(int argc, char **argv)
{
	GtkWidget *window, *box, *draw;

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Fractal Generator");
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
	gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_STATIC);

	box = gtk_box_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

	draw = gtk_drawing_area_new();
	gtk_box_pack_start(GTK_BOX(box), draw, TRUE, TRUE, 0);

	g_signal_connect(G_OBJECT(draw), "draw",
		G_CALLBACK(do_draw), NULL);
	g_signal_connect(G_OBJECT(draw), "size-allocate",
		G_CALLBACK(resize), NULL);
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}
