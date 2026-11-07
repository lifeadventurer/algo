// Returns empty on a cycle (also empty for an empty graph).
vector<int> toposort(const vector<vector<int>>& g) {
  int n = (int) g.size();
  vector<int> in(n), order;
  for (auto& adj : g) {
    for (int u : adj) {
      in[u]++;
    }
  }
  queue<int> q;
  for (int v = 0; v < n; v++) {
    if (!in[v]) {
      q.push(v);
    }
  }
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    order.push_back(v);
    for (int u : g[v]) {
      in[u]--;
      if (in[u] == 0) {
        q.push(u);
      }
    }
  }
  if ((int) order.size() != n) {
    return {};
  }
  return order;
}
