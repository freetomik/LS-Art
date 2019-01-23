#include "lsgen.hpp"

LSGenerator::LSGenerator() {}

LSystem LSGenerator::getLS()
{
    return this->ls;
}

void LSGenerator::readLSFromFile(std::string filename)
{
    this->ls.readFromFile(filename);
}

std::string LSGenerator::getIteration(unsigned int n)
{
    size_t curr_max_iter = this->iterations.size();
    // 0th iteration is the axiom
    if(n == 0) return this->ls.getAxiom();
    // if we don't have nth iteration pre-generated, generate it
    if(curr_max_iter < n) {
      // we have nothing pre-generated
      if(curr_max_iter == 0) {
        // generate first iteration from axiom
        std::string first_iteration = this->ls.expand(this->ls.getAxiom());
        this->iterations.push_back(first_iteration);
        curr_max_iter++;
      }
      // generate nth iteration
      for (; curr_max_iter < n; curr_max_iter++) {
        std::string new_iteration = this->ls.expand(this->iterations.at(curr_max_iter-1));
        this->iterations.push_back(new_iteration);
      }
    }
    return this->iterations[n-1];
}

LSGenerator::~LSGenerator(){}
