#include "gui.hpp"
#include "util.cpp"

#include <fstream>

GtkWidget *window = NULL;
GtkWidget *text_view = NULL;
GtkTextBuffer *text_buffer;

vector<string> LSfiles = vector<string>();

static gboolean do_draw(GtkWidget *draw, cairo_t *cr, gpointer data)
{
	// GtkAllocation alloc;

	// ifstream inFile(filename, fstream::in);

	// gtk_widget_get_allocation(draw, &alloc);
	// g_print("width=%d height=%d\n", alloc.width, alloc.height);
	// cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
	// TODO: set some nice LINE_JOIN

	// cairo_new_path(cr);	/* nova kresba */
	// cairo_move_to(cr, 50, 50);
	// cairo_line_to(cr, 30, 20);
	// cairo_line_to(cr, 40, 10);
	// cairo_line_to(cr, 70, 30);

	// double startx = 100, starty = 400, startr = 0;
	// Turtle turtle = Turtle(startx, starty, startr, cr);
	// double dist = 30.0;
	// double angle = 60.0;

	// char c;
	// while (inFile.get(c)) {
	// 	// cout << c;
	// 	if(c >= 'A' && c <= 'Z')
	// 		turtle.forwardLine(dist);
	// 	else if(c == '+')
	// 		turtle.turnLeft(angle);
	// 	else if(c == '-')
	// 		turtle.turnRight(angle);
	// }

	// cairo_close_path(cr);	/* ukoncit cestu */

	/* cairo_fill/stroke ukoncuje kresbu */
	// cairo_stroke(cr);

	guint width, height;
	GdkRGBA color;
	GtkStyleContext *context;

	context = gtk_widget_get_style_context (draw);

	width = gtk_widget_get_allocated_width (draw);
	height = gtk_widget_get_allocated_height (draw);

	gtk_render_background (context, cr, 0, 0, width, height);

	cairo_arc (cr,
						 width / 2.0, height / 2.0,
						 MIN (width, height) / 2.0,
						 0, 2 * G_PI);

	gtk_style_context_get_color (context,
															 gtk_style_context_get_state (context),
															 &color);
	gdk_cairo_set_source_rgba (cr, &color);

	cairo_fill (cr);

	return TRUE; /* neprobublavat udalost vyse */
}

static gboolean resize(GtkWidget *draw, GtkAllocation *alloc, gpointer data)
{
	// g_print("width=%d height=%d\n", alloc->width, alloc->height);
	return TRUE;
}

static void open_message_dialog(const char *message, const char *secondary)
{
  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (GTK_WINDOW (window),
                                   GtkDialogFlags(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
                                   GTK_MESSAGE_ERROR,
                                   GTK_BUTTONS_OK,
                                   message
                                   );
  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                            secondary);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

void combo_changed(GtkComboBox *widget, gpointer user_data)
{
		gint index = gtk_combo_box_get_active(widget);
		cout << "selected file: " << LSfiles[index] << '\n';

		string file_path = string("fractals/") + LSfiles[index];
		ifstream LSfile(file_path.data(), ios::in);
		string file_str;
		if (LSfile.is_open()) {
			string line;
			while (getline(LSfile, line)) {
				file_str += line + '\n';
			}
			LSfile.close();
		}
		else {
			string s_msg = string("Unable to open file ") + file_path;
			const char *msg = s_msg.data();
			const char *sec = "Do you have access right to the file?";
			open_message_dialog(msg, sec);
		}
		// change text in text view
		gtk_text_buffer_set_text (text_buffer, file_str.data(), -1);

		// TODO
		// draw fractal based on L-System specification from file
		// ...
}

void runGUI(int argc, char **argv)
{
		gtk_init(&argc, &argv);

		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
	  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	  gtk_window_set_title(GTK_WINDOW(window), "Fractal Generator");
		gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
		gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_STATIC);

		// top-level grid
		GtkWidget *grid;
		grid = gtk_grid_new();

		// combo box with input fractal files
		GtkWidget *combo;
		combo = gtk_combo_box_text_new();
		// get ls files from fractals directory
		// TODO read fractal directory name from global conf file
    string dir = "fractals";
    string ext = "ls";
    int err = getdir(dir, LSfiles, ext);
		if(err){
			const char *msg = "Cannot open fractals directory.";
			const char *sec = "Create directory fractals in program directory, "
												"fill it with .ls files and restart application.";
			open_message_dialog(msg, sec);
      return;
		}
    for (unsigned int i = 0;i < LSfiles.size();i++) {
      // cout << LSfiles[i] << endl;
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), NULL, LSfiles[i].data());
		}

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 1);
		gtk_grid_attach(GTK_GRID(grid), combo, 0, 0, 1, 1);
		g_signal_connect(G_OBJECT(combo), "changed",
			G_CALLBACK(combo_changed), NULL);

		text_view = gtk_text_view_new ();
		gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
		text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
		gtk_text_buffer_set_text (text_buffer, "Hello, this is some text", -1);
		gtk_grid_attach(GTK_GRID(grid), text_view, 0, 1, 1, 1);

		GtkWidget *button1;
		button1 = gtk_button_new_with_label("Button 1");
		gtk_grid_attach(GTK_GRID(grid), button1, 1, 0, 1, 1);

		// drawing area
		GtkWidget *draw;
		draw = gtk_drawing_area_new();
		gtk_grid_attach(GTK_GRID(grid), draw, 1, 1, 1, 1);
		// FIXME expand drawing area to fill the window
	  gtk_widget_set_hexpand (draw, GTK_ALIGN_FILL);
	  gtk_widget_set_vexpand (draw, GTK_ALIGN_FILL);
		gtk_container_add(GTK_CONTAINER(window), grid);


		// signals
		g_signal_connect(G_OBJECT(draw), "draw",
			G_CALLBACK(do_draw), NULL);
		g_signal_connect(G_OBJECT(draw), "size-allocate",
			G_CALLBACK(resize), NULL);
	  g_signal_connect(window, "destroy",
	      G_CALLBACK(gtk_main_quit), NULL);


		gtk_widget_show_all(window);

		gtk_main();
}
