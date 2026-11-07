// Directed or undirected; every weight must be 0 or 1.
vector<int> zero_one_bfs(const vector<vector<pair<int, int>>>& g, int s) {
  vector<int> d(g.size(), INT_MAX);
  deque<pair<int, int>> q;
  d[s] = 0;
  q.push_front({0, s});
  while (!q.empty()) {
    auto [dv, v] = q.front();
    q.pop_front();
    if (dv != d[v]) {
      continue;
    }
    for (auto [u, w] : g[v]) {
      assert(w == 0 || w == 1);
      if (d[u] <= dv + w) {
        continue;
      }
      d[u] = dv + w;
      if (w) {
        q.push_back({d[u], u});
      } else {
        q.push_front({d[u], u});
      }
    }
  }
  return d;
}
