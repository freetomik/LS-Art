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

public:
    LSRenderer (cairo_t *context, std::string string, draw_info_t di);

    void render();

    virtual ~LSRenderer ();
};

#endif
