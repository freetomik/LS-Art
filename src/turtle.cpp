#include "turtle.hpp"

#include <math.h>

#define PI 3.141592653589793

Turtle::Turtle() : x(0), y(0), rotation(0) {}

Turtle::Turtle(double r) : x(0), y(0), rotation(r) {}

Turtle::Turtle(double x, double y, double r) : x(x), y(y), rotation(r) {}
	
double Turtle::getX() {
    return this->x;
}

double Turtle::getY() {
    return this->y;
}

double Turtle::getRotation() {
    return this->rotation;
}


// Implementation of methods is inspired by
// Simple array-based turtle graphics engine in C
// written by Mike Lam at James Madison University in August 2015
// and released with GNU GPL3+ license

void Turtle::turnLeft(double angle) {
    // change turtle rotation
    this->rotation += angle;

    // constrain rotation to range: [0.0, 360.0)
    if (this->rotation < 0.0)
        this->rotation += 360.0;
    else if (this->rotation >= 360.0)
        this->rotation -= 360.0;
    // (or use modulo)
}

void Turtle::turnRight(double angle) {
    // opposite of "turn left"
    this->turnLeft(-angle);
}

void Turtle::turnBack() {
    this->turnLeft(180.0);
}

void Turtle::forwardLine(double distance, cairo_t *context) {
    // calculate (x,y) movement vector from heading
    double radians = this->rotation * PI / 180.0;
    double dx = cos(radians) * distance;
    double dy = sin(radians) * distance;

    this->x += dx; this->y += dy;
    cairo_line_to(context, this->x, this->y);
    // or try cairo_rel_line_to(context, dx, dy);
}

void Turtle::forwardGo(double distance, cairo_t *context) {
    // calculate (x,y) movement vector from heading
    double radians = this->rotation * PI / 180.0;
    double dx = cos(radians) * distance;
    double dy = sin(radians) * distance;

    this->x += dx; this->y += dy;
    cairo_move_to(context, this->x, this->y);
    // or try cairo_rel_move_to(context, dx, dy);
}

Turtle::~Turtle(){}
