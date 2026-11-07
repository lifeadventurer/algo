struct BFEdge {
  int u, v;
  long long w;
};

struct BFResult {
  vector<long long> d;
  vector<bool> neg;  // reachable from a reachable negative cycle
};

BFResult bellman_ford(int n, const vector<BFEdge>& e, int s) {
  const long long INF = 4'000'000'000'000'000'000LL;
  vector<long long> d(n, INF);
  d[s] = 0;
  // Keep n * sum(abs(w)) well below INF (no saturation).
  for (int k = 1; k < n; k++) {
    bool changed = false;
    for (auto [u, v, w] : e) {
      if (d[u] != INF && d[v] > d[u] + w) {
        d[v] = d[u] + w;
        changed = true;
      }
    }
    if (!changed) {
      break;
    }
  }
  vector<bool> neg(n);
  for (int k = 0; k < n; k++) {
    for (auto [u, v, w] : e) {
      if (d[u] != INF && (neg[u] || d[v] > d[u] + w)) {
        neg[v] = true;
      }
    }
  }
  return {d, neg};
}
