#include "lsrend.hpp"

LSRenderer::LSRenderer(cairo_t *context, std::string string, draw_info_t di)
{
    this->context = context;
    this->string = string;
    this->turtle = Turtle(di.startX, di.startY, di.rotation);
    this->di = di; 
}

void LSRenderer::render()
{
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
            cairo_close_path(this->context);  // close current path
            cairo_stroke(this->context);    // stroke it
            cairo_new_path(this->context);  // begin new path
            cairo_move_to(this->context, this->turtle.getX(), this->turtle.getY()); // first point of path
            this->turtleStack.push(this->turtle);
        }
        else if(c == ']')
            if(!this->turtleStack.empty()) {
                cairo_move_to(this->context, this->turtle.getX(), this->turtle.getY()); // last point of path
                cairo_close_path(this->context);  // close current path
                cairo_stroke(this->context);    // stroke it
                cairo_new_path(this->context);  // begin new path
                this->turtle = this->turtleStack.top();
                this->turtleStack.pop();
            }
    }

    cairo_stroke(this->context);
	// cairo_close_path(this->context);	/* close drawing path (connect start & end point) */
	// good for sierpinski triangle, koch snowflake, but not for hilbert curve
}

LSRenderer::~LSRenderer() {};
