#include <gtk/gtk.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

class Turtle {
private:
	double x;
	double y;
	double rotation;

public:
	Turtle ();
	virtual ~Turtle ();
};

std::string filename;
std::stack<Turtle> turtleStack;

static gboolean do_draw(GtkWidget *draw, cairo_t *cc, gpointer data)
{
	// GtkAllocation alloc;

	ifstream inFile(filename, fstream::in);

	// gtk_widget_get_allocation(draw, &alloc);
	// g_print("width=%d height=%d\n", alloc.width, alloc.height);
	cairo_set_antialias(cc, CAIRO_ANTIALIAS_DEFAULT);
	// TODO: set some nice LINE_JOIN

	cairo_new_path(cc);	/* nova kresba */

	char c;
	while (inFile.get(c)) {
		cout << c;
		// TODO:
		// switch(c) {
			// case 'F':
			// turtle.forward();
			// break;
			// ...
		// }
	}

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
	if (argc < 2) {
		printf("Program reads L-System string from file and renders it.\n");
		printf("Usage: %s file\n", argv[0]);
		return 1;
	}
	filename.assign(argv[1]);

	GtkWidget *window, *box, *draw;

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Fractal Generator");
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
	gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_STATIC);

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
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