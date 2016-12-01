#ifndef PARSER_H_
#define PARSER_H_

#include "graph.h"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::pair;

class Parser {
	Graph _graph;
	vector<string> _fields;
	map<pair<double, double>, int> _nodeMap;
	void readFile(string filename);
	void countNodes();
	void cleanNodesMap();
	void computeNodesMap();
	void displayNodeNum();
public:
	Parser();
	~Parser();
	void parse(string filename);
};

#endif