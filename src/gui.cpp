#include "gui.hpp"
#include "util.hpp"
#include "lsgen.hpp"
#include "lsrend.hpp"

#include <iostream>
#include <fstream>

using namespace std;

GtkWidget *window = NULL;
GtkWidget *draw_area = NULL;

cairo_surface_t *cairo_surface = NULL;

GtkWidget *ls_text_view = NULL;
GtkTextBuffer *ls_text_buffer = NULL;
GtkWidget *draw_string_text_view = NULL;
GtkTextBuffer *draw_string_text_buffer = NULL;

string filename;

LSGenerator *ls_gen;
LSRenderer *gtk_rend;
string draw_string;
draw_info_t draw_info;
unsigned short curr_iter;

vector<string> LS_files = vector<string>();

static gboolean do_draw(GtkWidget *draw_area, cairo_t *context, gpointer data)
{
	// get surface to global variable to use it for saving svg
	if(cairo_surface == NULL) {
		cairo_surface = cairo_surface_reference(cairo_get_target(context));
		// std::cout << "surface saved to global variable" << '\n';
	}
	// GtkAllocation alloc;
	//
	// gtk_widget_get_allocation(draw_area, &alloc);
	// g_print("width=%d height=%d\n", alloc.width, alloc.height);
	cairo_set_antialias(context, CAIRO_ANTIALIAS_DEFAULT);
	// TODO: set some nice LINE_JOIN

	// put draw string to text view
	// const char *draw_string_text;
	if(draw_string.length() > 100)
		gtk_text_buffer_set_text (draw_string_text_buffer, draw_string.substr(0, 100).data(), -1);
	else
		gtk_text_buffer_set_text (draw_string_text_buffer, draw_string.data(), -1);
	
	gtk_rend = new LSRenderer(context, draw_string, draw_info);
	gtk_rend->render();
	delete gtk_rend;

	/* cairo_fill/stroke ends the drawing */
	cairo_stroke(context);


	return TRUE; /* don't let go the event to higher place */
}

void increase_iteration()
{
		// TODO set upper boundary - if draw_string is very large
		if (true) {
			// get next iteration of L-System
			draw_string = ls_gen->getIteration(++curr_iter);
			// redraw drawing area
			gtk_widget_queue_draw(draw_area);
		}
}

void decrease_iteration()
{
		if (curr_iter > 1) {
			// get previous iteration of L-System
			draw_string = ls_gen->getIteration(--curr_iter);
			// redraw drawing area
			gtk_widget_queue_draw(draw_area);
		}
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
                                   "%s", message
                                   );
  gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                            "%s", secondary);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

void show_info_dialog(GtkButton *button, gpointer user_data)
{
		const char *msg = "L-System Art Generator";
		const char *sec = "This program generates string from specified L-System "
											"and renders it with turtle graphics.";
		open_message_dialog(msg, sec);
}

void save_svg(GtkButton *button, gpointer user_data)
{
	// TODO maybe make save dialog to create file name
	string svg_filename = "../svg/"+filename+".svg";
	cairo_surface_t *svg_surface = cairo_svg_surface_create(svg_filename.data(), 600, 600);
	cairo_t *svg_context = cairo_create(svg_surface);

	cairo_set_antialias(svg_context, CAIRO_ANTIALIAS_DEFAULT);
	// TODO: set some nice LINE_JOIN

	LSRenderer *svg_rend = new LSRenderer(svg_context, draw_string, draw_info);
	svg_rend->render();
	delete svg_rend;

	cairo_surface_flush(svg_surface);
	cairo_surface_finish(svg_surface);
	cairo_surface_destroy(svg_surface);
	// cairo_destroy(svg_context);
	std::cout << "svg image written to " << svg_filename << '\n';
	// std::cout << "cairo svg surface status: " << cairo_status_to_string(cairo_surface_status(svg_surface)) << '\n';
	// std::cout << "cairo svg context status: " << cairo_status_to_string(cairo_status(svg_context)) << '\n';
}

void combo_files_changed(GtkComboBox *widget, gpointer user_data)
{
		gint index = gtk_combo_box_get_active(widget);
		filename = LS_files[index];

		string file_path = string("../l-systems/") + filename;
		// string file_path = filename;
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
		// change text in l-system text view
		gtk_text_buffer_set_text (ls_text_buffer, file_str.data(), -1);

		// delete previous L-System generator
		if (ls_gen != NULL) {
			delete ls_gen;
			ls_gen = NULL;
		}
		// draw fractal based on L-System specification from file
		ls_gen = new LSGenerator();
		ls_gen->readLSFromFile(file_path);
		LSystem ls = ls_gen->getLS();
		draw_info = ls.getDrawInfo();
		draw_string = ls_gen->getIteration(draw_info.iterations);
		curr_iter = draw_info.iterations;
		// redraw drawing area
		gtk_widget_queue_draw(draw_area);
}

void runGUI(int argc, char **argv)
{
		gtk_init(&argc, &argv);

		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		gtk_window_set_title(GTK_WINDOW(window), "L-System Art Generator");
		gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
		gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_STATIC);

		// top-level grid
		GtkWidget *grid = gtk_grid_new();

		// combo box with input l-system files
		GtkWidget *combo_files = gtk_combo_box_text_new();
		// TODO read l-system directory name from global conf file
		string dir = "../l-systems";
		string ext = "ls";
		// get ls files from l-systems directory
		int err = getdir(dir, LS_files, ext);
		if(err){
			const char *msg = "Cannot open l-systems directory.";
			const char *sec = "Create directory l-systems in program directory, "
												"fill it with .ls files and restart application.";
			open_message_dialog(msg, sec);
			return;
		}
		for (unsigned int i = 0;i < LS_files.size();i++) {
			// cout << LS_files[i] << endl;
			gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_files), NULL, LS_files[i].data());
		}

		gtk_combo_box_set_active(GTK_COMBO_BOX(combo_files), 1);
		gtk_grid_attach(GTK_GRID(grid), combo_files, 0, 0, 1, 1);
		g_signal_connect(G_OBJECT(combo_files), "changed",
			G_CALLBACK(combo_files_changed), NULL);

		// text view for l-system definition string (is global)
		ls_text_view = gtk_text_view_new ();
		gtk_text_view_set_editable(GTK_TEXT_VIEW(ls_text_view), FALSE);
		ls_text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (ls_text_view));
		gtk_text_buffer_set_text (ls_text_buffer, "<l-system definition string>", -1);
		gtk_grid_attach(GTK_GRID(grid), ls_text_view, 0, 1, 1, 1);

		GtkWidget *decrease_button = gtk_button_new_with_label("-");
		gtk_grid_attach(GTK_GRID(grid), decrease_button, 1, 0, 1, 1);
		g_signal_connect(G_OBJECT(decrease_button), "clicked",
			G_CALLBACK(decrease_iteration), NULL);

		GtkWidget *increase_button = gtk_button_new_with_label("+");
		gtk_grid_attach(GTK_GRID(grid), increase_button, 2, 0, 1, 1);
		g_signal_connect(G_OBJECT(increase_button), "clicked",
			G_CALLBACK(increase_iteration), NULL);

		GtkWidget *about_button = gtk_button_new_with_label("About");
		gtk_grid_attach(GTK_GRID(grid), about_button, 3, 0, 1, 1);
		g_signal_connect(G_OBJECT(about_button), "clicked",
			G_CALLBACK(show_info_dialog), NULL);

		GtkWidget *save_button = gtk_button_new_with_label("Save SVG");
		gtk_grid_attach(GTK_GRID(grid), save_button, 4, 0, 1, 1);
		g_signal_connect(G_OBJECT(save_button), "clicked",
			G_CALLBACK(save_svg), NULL);

		// drawing area (is global)
		draw_area = gtk_drawing_area_new();
		gtk_widget_set_size_request(draw_area, 600, 600);
		gtk_grid_attach(GTK_GRID(grid), draw_area, 1, 1, 6, 1);
		// FIXME expand drawing area to fill the window
		gtk_widget_set_hexpand (draw_area, GTK_ALIGN_FILL);
		gtk_widget_set_vexpand (draw_area, GTK_ALIGN_FILL);

		// text view for draw string (is global)
		draw_string_text_view = gtk_text_view_new ();
		gtk_text_view_set_editable(GTK_TEXT_VIEW(draw_string_text_view), FALSE);
		draw_string_text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (draw_string_text_view));
		gtk_text_buffer_set_text (draw_string_text_buffer, "<draw string>", -1);
		gtk_grid_attach(GTK_GRID(grid), draw_string_text_view, 0, 2, 6, 1);

		// add grid to main window
		gtk_container_add(GTK_CONTAINER(window), grid);

		// init draw
		combo_files_changed(GTK_COMBO_BOX(combo_files), NULL);

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
