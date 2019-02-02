#ifndef TURTLE_H
#define TURTLE_H

#include <cairo.h>

class Turtle {
private:
	double x;
	double y;
	double rotation;

public:
	Turtle();
	Turtle(double r);
	Turtle(double x, double y, double r);
	
	double getX();
	double getY();
	double getRotation();

	void turnLeft(double angle);
	void turnRight(double angle);
	void turnBack();

	void forwardLine(double distance, cairo_t *context);
	void forwardGo(double distance, cairo_t *context);

	virtual ~Turtle();
};

#endif
