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
  vector<string> iterations;
  stack<Turtle> turtleStack;
public:
  FractalGenerator ();

  LSystem getLS();
  void readLSFromFile(string filename);
  string getIteration(unsigned int n);

  virtual ~FractalGenerator ();
};

#endif
