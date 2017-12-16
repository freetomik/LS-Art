#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <map>
#include <string>
#include <sstream>

using namespace std;

// default values for drawing
typedef struct {
  double angle;
  unsigned int iter;
  double lineLength;
  double x, y;
} draw_info_t;

class LSystem {
private:
  string name;
  string axiom;
  typedef map<char, string> rule_map_t;
  rule_map_t rules;

  draw_info_t draw_info;

public:
  LSystem();
  LSystem(string filename);

  string getAxiom();
  void dump();
  string expand(const string strToExpand);
  void readFromFile(string filename);
  draw_info_t getDrawInfo();

  virtual ~LSystem();
};


// NumberToString, StringToNumber
// author Mattia Basaglia
// http://www.cplusplus.com/forum/articles/9645/
// TODO license
template <typename T>
string NumberToString ( T Number )
{
	stringstream ss;
	ss << Number;
	return ss.str();
}

template <typename T>
T StringToNumber ( const string &Text )//Text not by const reference so that the function can be used with a
{                               //character array as argument
	stringstream ss(Text);
	T result;
	return ss >> result ? result : 0;
}

#endif
