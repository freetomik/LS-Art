#include "lsystem.hpp"
#include "ini.cpp"

#include <iostream>

using namespace std;
unsigned int numRules = 0;
LSystem::LSystem(string filename)
{
    // read ini file with https://github.com/Poordeveloper/ini-parser
    INI::Parser ini(filename.data());
    // stringstream out;
    // ini.dump(out);
    // cout << out.rdbuf() << '\n';
    #define LS_INI ini.top()("L-System")
    this->name = LS_INI["name"];
    this->axiom = LS_INI["axiom"];
    /*unsigned int*/ numRules = StringToNumber<int>(LS_INI["numRules"]);
    // parse rules
    for (size_t i = 1; i <= numRules; i++) {
      string ruleName = "rule" + NumberToString<size_t>(i);
      string ruleValue = LS_INI[ruleName];
      // regex of rule is: .=.+ ; e.g. "F=F+-F"
      char leftSide = ruleValue[0];
      ruleValue.erase(0, 2);
      this->rules[leftSide] = ruleValue;
    }
    this->angle = StringToNumber<double>(LS_INI["angle"]);
}

void LSystem::dump()
{
    cout << "name: " << this->name << '\n';
    cout << "axiom: " << this->axiom << '\n';
    cout << "numRules: " << numRules << '\n';
    for(auto rule : this->rules)
      cout << "rule: " << rule.first << " > " << rule.second <<  "\n";
    cout << "angle: " << this->angle << '\n';
}

LSystem::~LSystem(){}