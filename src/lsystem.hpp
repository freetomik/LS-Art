#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <map>
#include <string>
#include <sstream>

// using namespace std;

// default values for drawing
typedef struct {
  double angle;
  unsigned int iterations;
  double lineLength;
  double startX, startY;
  double rotation;
} draw_info_t;

class LSystem {
private:
  std::string name;
  std::string axiom;
  typedef std::map<char, std::string> rule_map_t;
  rule_map_t rules;

  draw_info_t draw_info;

public:
  LSystem();
  LSystem(std::string filename);

  std::string getAxiom();
  void dump();
  std::string expand(const std::string strToExpand);
  void readFromFile(std::string filename);
  draw_info_t getDrawInfo();

  virtual ~LSystem();
};


// NumberToString, StringToNumber
// author Mattia Basaglia
// http://www.cplusplus.com/forum/articles/9645/
// TODO license
template <typename T>
std::string NumberToString ( T Number )
{
	std::stringstream ss;
	ss << Number;
	return ss.str();
}

template <typename T>
T StringToNumber ( const std::string &Text )//Text not by const reference so that the function can be used with a
{                               //character array as argument
	std::stringstream ss(Text);
	T result;
	return ss >> result ? result : 0;
}

#endif
