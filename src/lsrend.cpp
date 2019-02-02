#include "lsrend.hpp"

LSRenderer::LSRenderer(cairo_t *context, std::string string, draw_info_t di)
{
    this->context = context;
    this->string = string;
    this->turtle = Turtle(di.rotation);
    this->di = di;
    this->x1 = 1e+10;
    this->y1 = 1e+10;
    this->x2 = -1e+10;
    this->y2 = -1e+10;
}

// updates minimum and maximum extents of whole drawing
// x1 - left, x2 - right, y1 - top, y2 - bottom
inline void LSRenderer::updateExtents(double x1, double y1, double x2, double y2)
{
    // empty rectangle was returned because of empty path
    if(x1==0 && y1==0 && x2==0 && y2==0) return;
    if(x1 < this->x1) this->x1 = x1;
    if(y1 < this->y1) this->y1 = y1;
    if(x2 > this->x2) this->x2 = x2;
    if(y2 > this->y2) this->y2 = y2;
}

// returns extents of whole drawing
cairo_rectangle_t LSRenderer::getExtents()
{
    cairo_rectangle_t rect;
    rect.x = this->x1;
    rect.y = this->y1;
    rect.width = this->x2 - this->x1;
    rect.height = this->y2 - this->y1;
    return rect;
}

// renders L-System string with turtle graphics
void LSRenderer::render()
{
    double x1, y1, x2, y2;
    cairo_new_path(this->context);

    for(char c : this->string) {
        if(c >= 'A' && c <= 'Z')
            this->turtle.forwardLine(this->di.lineLength, this->context);
        else if(c == '+')
            this->turtle.turnLeft(this->di.angle);
        else if(c == '-')
            this->turtle.turnRight(this->di.angle);
        else if(c == '|')
            this->turtle.turnBack();
        else if(c == '[') {
            cairo_stroke_extents(this->context, &x1, &y1, &x2, &y2);
            this->updateExtents(x1, y1, x2, y2);
            cairo_close_path(this->context);  // close current path
            cairo_stroke(this->context);    // stroke it
            cairo_new_path(this->context);  // begin new path
            cairo_move_to(this->context, this->turtle.getX(), this->turtle.getY()); // first point of path
            this->turtleStack.push(this->turtle);
        }
        else if(c == ']')
            if(!this->turtleStack.empty()) {
                cairo_move_to(this->context, this->turtle.getX(), this->turtle.getY()); // last point of path
                cairo_stroke_extents(this->context, &x1, &y1, &x2, &y2);
                this->updateExtents(x1, y1, x2, y2);
                cairo_close_path(this->context);  // close current path
                cairo_stroke(this->context);    // stroke it
                cairo_new_path(this->context);  // begin new path
                this->turtle = this->turtleStack.top();
                this->turtleStack.pop();
            }
    }

    cairo_stroke_extents(this->context, &x1, &y1, &x2, &y2);
    this->updateExtents(x1, y1, x2, y2);
    if(this->di.closed)
        cairo_close_path(this->context); /* close drawing path (connect start & end point) */
    cairo_stroke(this->context);
}

LSRenderer::~LSRenderer() {}
