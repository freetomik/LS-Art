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
	Turtle(double x, double y, double r);

	void turnLeft(double angle);
	void turnRight(double angle);
	void turnBackwards();

	void forwardLine(double distance, cairo_t *context);
	void forwardGo(double distance, cairo_t *context);

	virtual ~Turtle();
};

#endif
