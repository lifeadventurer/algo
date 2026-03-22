vector<long long> dijkstra(const vector<vector<pair<int, long long>>>& g, int source) {
  int n = (int) g.size();
  const long long INF = (long long) 4e18;
  vector<long long> dist(n, INF);
  priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
  dist[source] = 0;
  pq.push({0, source});
  while (!pq.empty()) {
    auto [d, v] = pq.top();
    pq.pop();
    if (d != dist[v]) {
      continue;
    }
    for (auto [u, w] : g[v]) {
      if (dist[u] > d + w) {
        dist[u] = d + w;
        pq.push({dist[u], u});
      }
    }
  }
  return dist;
}
