#ifndef LSREND_H
#define LSREND_H

#include <stack>
#include <string>

#include "turtle.hpp"
#include "lsystem.hpp"

class LSRenderer {
private:
    cairo_t *context;
    std::string string;

    draw_info_t di;
    Turtle turtle;
    std::stack<Turtle> turtleStack;
    
    // extents of drawing
    // x1 - left, x2 - right, y1 - top, y2 - bottom
    double x1, y1, x2, y2;

    inline void updateExtents(double x1, double y1, double x2, double y2);

public:
    LSRenderer (cairo_t *context, std::string string, draw_info_t di);

    void render();
    
    cairo_rectangle_t getExtents();

    virtual ~LSRenderer ();
};

#endif
