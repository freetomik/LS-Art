#ifndef LSGEN_H
#define LSGEN_H

#include <vector>
#include <string>

#include "lsystem.hpp"

class LSGenerator {
private:
  LSystem ls;
  std::vector<std::string> iterations;
public:
  LSGenerator ();

  LSystem getLS();
  void readLSFromFile(std::string filename);
  std::string getIteration(unsigned int n);

  virtual ~LSGenerator ();
};

#endif
