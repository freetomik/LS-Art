#ifndef FRAGEN_H
#define FRAGEN_H

#include <stack>
#include <vector>
#include <string>

#include "lsystem.hpp"
#include "turtle.hpp"

class FractalGenerator {
private:
  LSystem ls;
  vector<string> *iterations;   // use pointer or not?
  stack<Turtle> turtleStack;
public:
  FractalGenerator ();
  virtual ~FractalGenerator ();
};

#endif
