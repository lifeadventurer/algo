vector<int> bfs(const vector<vector<int>>& g, int source) {
  int n = (int) g.size();
  vector<int> dist(n, -1);
  queue<int> q;
  dist[source] = 0;
  q.push(source);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int u : g[v]) {
      if (dist[u] == -1) {
        dist[u] = dist[v] + 1;
        q.push(u);
      }
    }
  }
  return dist;
}
