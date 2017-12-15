#include "lsystem.hpp"
#include "ini.cpp"

#include <iostream>

using namespace std;

LSystem::LSystem(){}

unsigned int numRules = 0;
// reads L-System specification from ini file
LSystem::LSystem(string filename)
{
    this->readFromFile(filename);
}

void LSystem::readFromFile(string filename)
{
    // read ini file with https://github.com/Poordeveloper/ini-parser
    INI::Parser ini(filename.data());
    #define LS_INI ini.top()("L-System")
    this->name = LS_INI["name"];
    this->axiom = LS_INI["axiom"];
    /*unsigned int*/ numRules = StringToNumber<int>(LS_INI["numRules"]);
    // parse rules
    for (size_t i = 0; i <= numRules; i++) {
      string ruleName = "rule" + NumberToString<size_t>(i);
      string ruleValue = LS_INI[ruleName];
      // regex of rule is: .=.+ ; e.g. "F=F+-F"
      char leftSide = ruleValue[0];
      ruleValue.erase(0, 2);
      this->rules[leftSide] = ruleValue;
    }
    // drawing stuff
    this->draw_info.angle = StringToNumber<double>(LS_INI["angle"]);
    this->draw_info.lineLength = StringToNumber<double>(LS_INI["lineLength"]);
    this->draw_info.iter = StringToNumber<int>(LS_INI["iter"]);
    this->draw_info.x = StringToNumber<int>(LS_INI["x"]);
    this->draw_info.y = StringToNumber<int>(LS_INI["y"]);
}

draw_info_t LSystem::getDrawInfo()
{
    return this->draw_info;
}

// Goes through string and replaces characters with strings
// according to L-System rules
string LSystem::expand(const string strToExpand)
{
    char c;
    string expandedStr(strToExpand);
    rule_map_t::iterator rule_map_iter;
    for (size_t i = 0; i < expandedStr.length(); i++) {
      c = expandedStr.at(i);
      rule_map_iter = this->rules.find(c);
      if(rule_map_iter != this->rules.end()) {
        expandedStr.replace(i, 1, rule_map_iter->second);
        i += rule_map_iter->second.length() - 1;
      }
    }
    return expandedStr;
}

string LSystem::getAxiom()
{
    return this->axiom;
}

// void LSystem::dump()
// {
//     cout << "name: " << this->name << '\n';
//     cout << "axiom: " << this->axiom << '\n';
//     cout << "numRules: " << numRules << '\n';
//     for(auto rule : this->rules)
//       cout << "rule: " << rule.first << " > " << rule.second <<  "\n";
//     cout << "angle: " << this->angle << '\n';
// }

LSystem::~LSystem(){}
