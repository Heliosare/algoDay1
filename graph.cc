#include "graph.h"

#include <algorithm>
#include <sstream>

#include "base.h"

int Graph::AddArc(int from, int to) {
  CHECK_GE(from, 0);
  CHECK_GE(to, 0);
  AddNode(std::max(from, to));
  const int arc_index = NumArcs();
  outgoing_arcs_[from].push_back(arc_index);
  incoming_arcs_[to].push_back(arc_index);
  tail_.push_back(from);
  head_.push_back(to);
  return arc_index;
}

void Graph::AddNode(int node) {
  if (NumNodes() <= node) {
    outgoing_arcs_.resize(node + 1);
    incoming_arcs_.resize(node + 1);
  }
}

void Graph::RemapNodes(const vector<int>& new_indices) {
  // Check that we do have a permutation as input.
  const int n = NumNodes();
  CHECK_EQ(new_indices.size(), n);
  vector<bool> mask(n, false);
  for (const int i : new_indices) {
    CHECK_GE(i, 0);
    CHECK_LT(i, n);
    CHECK(!mask[i]);
    mask[i] = true;
  }
  // Apply the permutation to the Tail/Head.
  for (int& x : tail_) x = new_indices[x];
  for (int& x : head_) x = new_indices[x];
  // Permute the outgoing_arcs_ and incoming_arcs_ vectors.
  for (vector<vector<int>>* v : {&outgoing_arcs_, &incoming_arcs_}) {
    vector<vector<int>> newv(n);
    for (int i = 0; i < n; ++i) newv[new_indices[i]].swap((*v)[i]);
    newv.swap(*v);
  }
}

string PrintGraphAsDot(const Graph& graph, const vector<double>& arc_lengths) {
  std::ostringstream os;
  os << "digraph g {\n";
  for (int arc = 0; arc < graph.NumArcs(); ++arc) {
    os << "  " << graph.Tail(arc) << " -> " << graph.Head(arc)
       << " [label=" << arc_lengths[arc] << "];\n";
  }
  os << "}\n";
  return os.str();
}

namespace {
  int GetRoot(vector<int>& root, int x) {
    int r = x;
    while (r != root[r]) r = root[r];
    while (x != r) {
      int p = root[x];
      root[x] = r;
      x = p;
    }
    return r;
  }
}  // namespace

vector<int> ConnectedComponentSizes(const Graph& graph) {
  const int n = graph.NumNodes();
  vector<int> root(n, -1);
  for (int i = 0; i < n; ++i) root[i] = i;
  for (int a = 0; a < graph.NumArcs(); ++a) {
    int r0 = GetRoot(root, graph.Tail(a));
    int r1 = GetRoot(root, graph.Head(a));
    root[r0] = r1;
  }
  vector<int> sizes(n, 0);
  for (int i = 0; i < n; ++i) sizes[GetRoot(root, i)]++;
  sizes.erase(std::remove(sizes.begin(), sizes.end(), 0), sizes.end());
  return sizes;
}

vector<int> Degrees(const Graph& graph) {
  vector<int> degs(graph.NumNodes(), 0);
  for (int a = 0; a < graph.NumArcs(); ++a) {
    ++degs[graph.Tail(a)];
    ++degs[graph.Head(a)];
  }
  vector<int> d2;
  for (int i = 0; i < graph.NumNodes(); ++i) {
    d2.push_back(graph.OutgoingArcs(i).size() + graph.IncomingArcs(i).size());
  }
  CHECK(degs == d2);
  return degs;
}

vector<int> NodesSortedByIncreasingDegreeProduct(const Graph& graph) {
  const int num_nodes = graph.NumNodes();
  vector<int> score(num_nodes, 0.0);
  for (int i = 0; i < num_nodes; ++i) {
    score[i] = graph.OutDegree(i) * graph.InDegree(i);
  }
  vector<int> indices(num_nodes, -1);
  for (int i = 0; i < num_nodes; ++i) indices[i] = i;
  std::stable_sort(indices.begin(), indices.end(),
                   [&score](int i, int j) { return score[i] < score[j]; });
  return indices;
}
