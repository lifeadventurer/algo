// Requires data/dsu.cpp. Returns minimum spanning forest.
struct MSTEdge {
  int u, v;
  long long w;
};

pair<long long, vector<MSTEdge>> kruskal(int n, vector<MSTEdge> edges) {
  sort(edges.begin(), edges.end(), [](auto a, auto b) {
    return a.w < b.w;
  });
  DSU d(n);
  long long cost = 0;
  vector<MSTEdge> used;
  for (auto e : edges) {
    if (d.unite(e.u, e.v)) {
      cost += e.w;
      used.push_back(e);
    }
  }
  return {cost, used};  // connected iff used.size()==n-1
}
