#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <map>
#include <string>
#include <sstream>

using namespace std;

class LSystem {
private:
  string name;
  string axiom;
  map<char, string> rules;
  double angle;

public:
  LSystem (string filename);
  void dump();
  virtual ~LSystem ();
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