#ifndef LSGEN_H
#define LSGEN_H

#include <vector>
#include <string>

#include "lsystem.hpp"

class LSGenerator {
private:
  LSystem ls;
  vector<string> iterations;
public:
  LSGenerator ();

  LSystem getLS();
  void readLSFromFile(string filename);
  string getIteration(unsigned int n);

  virtual ~LSGenerator ();
};

#endif
