#include "gui.hpp"
#include "util.cpp"
#include "fragen.hpp"

#include <fstream>

GtkWidget *window = NULL;
GtkWidget *text_view = NULL;
GtkWidget *draw_area = NULL;
GtkTextBuffer *text_buffer;

FractalGenerator fragen;
string draw_string;
draw_info_t draw_info;

vector<string> LSfiles = vector<string>();

static gboolean do_draw(GtkWidget *draw_area, cairo_t *cr, gpointer data)
{
	// GtkAllocation alloc;
	//
	// gtk_widget_get_allocation(draw_area, &alloc);
	// g_print("width=%d height=%d\n", alloc.width, alloc.height);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
	// TODO: set some nice LINE_JOIN

	cairo_new_path(cr);	/* nova kresba */

	double lineLength = draw_info.lineLength;
	double angle = draw_info.angle;
	double startx = draw_info.x, starty = draw_info.y, startr = 0;
	Turtle turtle = Turtle(startx, starty, startr, cr);

	for (char c : draw_string) {
		// cout << c;
		if(c >= 'A' && c <= 'Z')
			turtle.forwardLine(lineLength);
		else if(c == '+')
			turtle.turnLeft(angle);
		else if(c == '-')
			turtle.turnRight(angle);
	}

	// cairo_close_path(cr);	/* ukoncit cestu */

	/* cairo_fill/stroke ukoncuje kresbu */
	cairo_stroke(cr);

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

void show_info_dialog(GtkButton *button, gpointer user_data)
{
		const char *msg = "Fractal Generator v0.1";
		const char *sec = "This program generates string from specified L-System"
											"and renders it with turtle graphics.";
		open_message_dialog(msg, sec);
}

void combo_changed(GtkComboBox *widget, gpointer user_data)
{
		gint index = gtk_combo_box_get_active(widget);
		string filename = LSfiles[index];

		// string file_path = string("fractals/") + filename;
		string file_path = filename;
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

		// draw fractal based on L-System specification from file
		fragen.readLSFromFile(file_path);
		LSystem ls = fragen.getLS();
		draw_info = ls.getDrawInfo();
		draw_string = fragen.getIteration(draw_info.iter);
		// redraw drawing area
		gtk_widget_queue_draw(draw_area);
}

void runGUI(int argc, char **argv)
{
		gtk_init(&argc, &argv);

		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
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
    string dir = ".";
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

		GtkWidget *info_button;
		info_button = gtk_button_new_with_label("Info");
		gtk_grid_attach(GTK_GRID(grid), info_button, 1, 0, 1, 1);
		g_signal_connect(G_OBJECT(info_button), "clicked",
			G_CALLBACK(show_info_dialog), NULL);

		// drawing area (is global)
		draw_area = gtk_drawing_area_new();
		gtk_widget_set_size_request(draw_area, 500, 450);
		gtk_grid_attach(GTK_GRID(grid), draw_area, 1, 1, 1, 1);
		// FIXME expand drawing area to fill the window
	  gtk_widget_set_hexpand (draw_area, GTK_ALIGN_FILL);
	  gtk_widget_set_vexpand (draw_area, GTK_ALIGN_FILL);
		gtk_container_add(GTK_CONTAINER(window), grid);

		// init draw
		combo_changed(GTK_COMBO_BOX(combo), NULL);

		// signals
		g_signal_connect(G_OBJECT(draw_area), "draw",
			G_CALLBACK(do_draw), NULL);
		g_signal_connect(G_OBJECT(draw_area), "size-allocate",
			G_CALLBACK(resize), NULL);
	  g_signal_connect(window, "destroy",
	      G_CALLBACK(gtk_main_quit), NULL);


		gtk_widget_show_all(window);

		gtk_main();
}
