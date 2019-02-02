# LS-Art
L-System art generator with graphical user interface in [Gtk+3](https://developer.gnome.org/gtk3/stable/) and using [Cairo](https://www.cairographics.org/) drawing library.

Program reads [L-System](https://en.wikipedia.org/wiki/L-system) description from text file in `ini` format, generates production string according to it and renders a drawing by interpreting this string with turtle graphics.

![Screenshot](https://raw.githubusercontent.com/freetomik/LS-Art/assets/screen.png)

## Usage
Program is intended to work under Linux operating system. Program must be run from `bin` folder within directory structure.

### Directories structure
- `src` - source code files
- `l-systems` - input text files describing L-Systems
- `svg` - folder for storing drawings as svg files
- `obj` - compiled object files
- `bin` - folder with final executable

`obj` and `bin` folders are created by `make` program before compiling source codes.

### Input files
Program reads from `../l-systems` folder files with `.ls` extension and having `ini` format:
```
# Sierpinski triangle

[L-System]
name=Sierpinski triangle
axiom=F-G-G
numRules=2
rule0=F>F-G+F+G-F
rule1=G>GG
angle=120
iterations=6
lineLength=30
closed=1
```
, where `closed` (value can be 0, or 1) means, if drawing path should be closed, i.e. start and end point connected
and other parameters are common for L-System definition.

### Output files
Drawings can be stored as SVG files in `../svg` folder.

## Development

### Tools
- GNU Make
- pkg-config
- g++

### Dependencies
Development packages for GTK3 and Cairo2 libraries - `libgtk-3-dev` and `libcairo2-dev` packages on Debian-based systems.
Normal packages for those libraries should be installed by default on modern Linux OS.

### Building
```
$ make
```

### Contributing
Feel free to fork and play with project. Pull requests with enhancements or fixes are also welcome!

### Third party
For reading `ini` files [ini-parser](https://github.com/Poordeveloper/ini-parser) is used.

### License
Distributed under the [MIT](https://tldrlegal.com/license/mit-license) License.
