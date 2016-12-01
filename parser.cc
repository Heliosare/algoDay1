#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

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
	index = 0;
	it = this->_nodeMap.begin();
	while (it != this->_nodeMap.end()) {
		it->second = index;
		index++;
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

double Parser::calcDistance(double lat1, double lng1, double lat2, double lng2) {
	return (2.0 * 6371.0088 * std::asin(std::sqrt(std::pow(std::sin((lat2 - lat1) / 2.0), 2.0)) + std::cos(lat1) * std::cos(lat2) * std::pow(std::sin((lng2 - lng1) / 2.0), 2.0)));
}

int Parser::isANode(double lat, double lng) {
	if (this->_nodeMap.count({lat, lng}) > 0)
		return (this->_nodeMap[{lat, lng}]);
	else
		return (-1);
}

void Parser::computeNodesMap() {
	int index = 0, nodeIndex = 0;
	int numpoint = 0;
	bool alt = false;
	double fLat = 0.0f, fLng = 0.0f;
	double actLat = 0.0f, actLng = 0.0f;
	double oldLat = 0.0f, oldLng = 0.0f;
	double actualDistance = 0.0f;
	bool isOneWay = false;
	int actualSpeed = 0;

	for (auto it = _fields.begin(); it != _fields.end(); it++, index++) {
		if (index == 0) {
			it += 2;
			actualSpeed = stoi(*it);
			it += 2;
			if (stoi(*it) == 1)
				isOneWay = true;
			else
				isOneWay = false;
			it += 1;
			numpoint = stoi(*it) * 2;
		} else if (index <= numpoint) {
			if (!alt) {
				if (index == 1) {
					fLat = stod(*it);
					actLat = fLat;
				} else {
					oldLat = actLat;
					actLat = stod(*it);
				}
				alt = true;
			} else {
				if (index == 2) {
					fLng = stod(*it);
					actLng = fLng;
				} else {
					oldLng = actLng;
					actLng = stod(*it);
					actualDistance += calcDistance(oldLat, oldLng, actLat, actLng);
					nodeIndex = isANode(actLat, actLng);
					if (nodeIndex != -1) {
						int fromIndex = isANode(fLat, fLng);
						_graph.AddArc(fromIndex, nodeIndex);
						if (!isOneWay) {
							_graph.AddArc(nodeIndex, fromIndex);
						}
						fLat = actLat;
						fLng = actLng;
						//calcule temps et set dans un vector avec index de l'arc retourner à la création de larc
						//set actual distance to 0.0
						//std::cout << std::to_string(actualDistance) << std::endl;
					}
				}
				alt = false;
			}
		} else {
			index = 0;
			numpoint = 0;
			it += 2;
			actualSpeed = stoi(*it);
			it += 2;
			if (stoi(*it) == 1)
				isOneWay = true;
			else
				isOneWay = false;
			it += 1;
			numpoint = stoi(*it) * 2;
		}
	}
}

void Parser::display() {
	std::cout << std::to_string(_graph.NumNodes()) + " nodes / " + std::to_string(_graph.NumArcs()) + " arcs" << std::endl;
}

void Parser::parse(string filename) {
	readFile(filename);
	countNodes();
	cleanNodesMap();
	_graph.AddNode(_nodeMap.size() - 1);
	computeNodesMap();
	display();
	std::cout << calcDistance(33, 1, -45, -9) << std::endl;
	/*for (auto it = this->_nodeMap.begin(); it != this->_nodeMap.end(); it++) {
		std::cout << std::to_string(it->first.first) + " ; " + std::to_string(it->first.second) + " ; " + std::to_string(it->second) << std::endl;
	}*/
}
