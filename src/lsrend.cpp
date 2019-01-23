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
    for(char c : this->string) {
        if(c >= 'A' && c <= 'Z')
            this->turtle.forwardLine(this->di.lineLength, this->context);
        else if(c == '+')
            this->turtle.turnLeft(this->di.angle);
        else if(c == '-')
            this->turtle.turnRight(this->di.angle);
        else if(c == '[')
            this->turtleStack.push(this->turtle);
        else if(c == ']')
            if(!this->turtleStack.empty()) {
                this->turtle = this->turtleStack.top();
                this->turtleStack.pop();
            }
    }
    
}

LSRenderer::~LSRenderer() {};
