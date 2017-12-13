#include <iostream>
#include <fstream>
#include <string>

#include "fragen.hpp"
#include "lsystem.hpp"
#include "gui.hpp"

using namespace std;

string filename;

int main(int argc, char **argv)
{
		if (argc < 2) {
			printf("Program reads L-System string from file and renders it.\n");
			printf("Usage: %s file\n", argv[0]);
			return 1;
		}
		filename.assign(argv[1]);
		LSystem ls(filename);
    ls.dump();
		cout << ls.expand(ls.expand(ls.getAxiom())) << '\n';
		// return 0;

		runGUI(argc, argv);

		return 0;
}
