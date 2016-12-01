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
	vector<double> _arcTime;
	void readFile(string filename);
	void countNodes();
	void cleanNodesMap();
	void computeNodesMap();
	void display();
	int isANode(double lat, double lng);
	double calcDistance(double lat1, double lng1, double lat2, double lng2);
public:
	Parser();
	~Parser();
	void parse(string filename);
};

#endif