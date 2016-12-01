#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

Parser::Parser() {

}

Parser::~Parser() {

}

void Parser::cleanNodesMap() {
	int index = 0;
	auto it = this->_nodeMap.begin();
	while (it != this->_nodeMap.end()) {
		if (it->second <= 1) {
			this->_nodeMap.erase(it++);
			index++;
		}
		else
			it++;
	}
}

void Parser::countNodes() {
	int index = 0;
	int numpoint = 0;
	bool alt = false;
	double lat = 0.0f;
	for (auto it = _fields.begin(); it != _fields.end(); it++, index++) {
		if (index == 0) {
			it += 5;
			numpoint = stoi(*it) * 2;
		} else if (index <= numpoint) {
			if (!alt) {
				lat = stod(*it);
				alt = true;
			} else {
				if (index == 2 || index == numpoint)
					++this->_nodeMap[{lat, stod(*it)}];
				++this->_nodeMap[{lat, stod(*it)}];
				alt = false;
			}
		} else {
			index = 0;
			numpoint = 0;
			it += 5;
			numpoint = stoi(*it) * 2;
		}
	}
}

void Parser::readFile(string filename) {
	std::ifstream iss(filename);
	for (string line; getline(iss, line);) {
		std::istringstream line_is(line);
		for (string field; getline(line_is, field, ',');) {
			this->_fields.push_back(field);
		}
	}
}

void Parser::computeNodesMap() {
	int index = 0;
	int numpoint = 0;
	bool alt = false;
	double lat, lng = 0.0f;
	for (auto it = _fields.begin(); it != _fields.end(); it++, index++) {
		if (index == 0) {
			it += 5;
			numpoint = stoi(*it) * 2;
		} else if (index <= numpoint) {
			if (!alt) {
				lat = stod(*it);
				alt = true;
			} else {
				lng = stod(*it);
				alt = false;
			}
		} else {
			index = 0;
			numpoint = 0;
			it += 5;
			numpoint = stoi(*it) * 2;
		}
	}
}

void Parser::displayNodeNum() {
	std::cout << std::to_string(_graph.NumNodes()) + " nodes" << std::endl;
}

void Parser::parse(string filename) {
	readFile(filename);
	countNodes();
	cleanNodesMap();
	_graph.AddNode(_nodeMap.size() - 1);
	displayNodeNum();
	computeNodesMap();
	/*for (auto it = this->_nodeMap.begin(); it != this->_nodeMap.end(); it++) {
		std::cout << std::to_string(it->first.first) + " ; " + std::to_string(it->first.second) + " ; " + std::to_string(it->second) << std::endl;
	}*/
}
