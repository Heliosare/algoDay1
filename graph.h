#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>
#include <vector>

using std::vector;
using std::string;

// In this graph, nodes are integers in 0..num_nodes-1, and arcs are also
// integers in 0..num_arcs-1.
// Thus, when you AddArc(4, 128), you implicitly declare the existence of
// at least 129 nodes (0..128).
class Graph {
 public:
  // Return the arc index, which will be the number of times AddArc()
  // has been called before.
  int AddArc(int from, int to);

  // Optional: nodes are automatically added upon AddArc().
  void AddNode(int node);

  int NumNodes() const { return outgoing_arcs_.size(); }
  int NumArcs() const { return tail_.size(); }

  // Gets the tail ("from") and head ("to") of an arc.
  int Tail(int arc) const { return tail_[arc]; }
  int Head(int arc) const { return head_[arc]; }

  // Returns a list of all the arc indices whose Tail is "from".
  const vector<int>& OutgoingArcs(int from) const {
    return outgoing_arcs_[from];
  }
  // Returns a list of all the arc indices whose Head is "to".
  const vector<int>& IncomingArcs(int to) const {
    return incoming_arcs_[to];
  }

  int OutDegree(int node) const { return OutgoingArcs(node).size(); }
  int InDegree(int node) const { return IncomingArcs(node).size(); }

  bool NodeIsValid(int node) const { return node >= 0 && node < NumNodes(); }
  bool ArcIsValid(int arc) const { return arc >= 0 && arc < NumArcs(); }

  void RemapNodes(const vector<int>& new_indices);

 private:
  vector<vector<int>> outgoing_arcs_;
  vector<vector<int>> incoming_arcs_;
  vector<int> tail_;
  vector<int> head_;
};

// "Dot" format. https://en.wikipedia.org/wiki/DOT_(graph_description_language)
string PrintGraphAsDot(const Graph& graph, const vector<double>& arc_lengths);

vector<int> ConnectedComponentSizes(const Graph& graph);

vector<int> Degrees(const Graph& graph);

vector<int> NodesSortedByIncreasingDegreeProduct(const Graph& graph);

#endif  // GRAPH_H_
