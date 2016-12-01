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
	double r = 6371008;
	double un = lat1 * M_PI / 180;
	double deux = lat2 * M_PI / 180;
	double delta = (lat2 - lat1) * M_PI / 180;
	double alpha = (lng1 - lng2) * M_PI / 180;

	double a = std::sin(delta / 2) * std::sin(delta / 2) +
	 						std::cos(un) * std::cos(deux) *
							std::sin(alpha / 2) * std::sin(alpha /2);
	double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
	return r * c;
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
						_arcTime.push_back(actualDistance / ((double)actualSpeed * 1000 / 3600));
						if (!isOneWay) {
							_graph.AddArc(nodeIndex, fromIndex);
							_arcTime.push_back(actualDistance / ((double)actualSpeed * 1000 / 3600));
						}
						fLat = actLat;
						fLng = actLng;
						actualDistance = 0.0f;
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
	double time = 0.0f;
	for (auto it = _arcTime.begin(); it != _arcTime.end(); it++) {
		time += *it;
	}
	std::cout << std::to_string(_graph.NumNodes()) + " nodes / " + std::to_string(_graph.NumArcs()) + " arcs / " + std::to_string(time) + " seconds" << std::endl;
}

void Parser::parse(string filename) {
	readFile(filename);
	countNodes();
	cleanNodesMap();
	_graph.AddNode(_nodeMap.size() - 1);
	computeNodesMap();
	display();
	/*for (auto it = this->_nodeMap.begin(); it != this->_nodeMap.end(); it++) {
		std::cout << std::to_string(it->first.first) + " ; " + std::to_string(it->first.second) + " ; " + std::to_string(it->second) << std::endl;
	}*/
}
