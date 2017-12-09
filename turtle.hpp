#ifndef TURTLE_H
#define TURTLE_H

#include <cairo.h>

class Turtle {
private:
	double x;
	double y;
	double rotation;

  cairo_t *cr;  // cairo drawing context

public:
	Turtle(double x, double y, double r, cairo_t *cr);

  void turnLeft(double angle);
  void turnRight(double angle);
  void turnBackwards();

  void forwardLine(double distance);
  void forwardGo(double distance);

	virtual ~Turtle();
};

#endif
