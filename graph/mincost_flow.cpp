// Successive shortest paths using Bellman-Ford.
// Negative edge costs OK; no negative-cost residual cycles.
// Nonnegative capacities; keep distance/cost intermediates within +/-4e18.
struct MinCostFlow {
  struct Edge {
    int u, v;
    long long cap, cost;
  };

  int n;
  vector<Edge> e;

  MinCostFlow(int _n) : n(_n) {}

  void add(int u, int v, long long cap, long long cost) {
    e.push_back({u, v, cap, cost});
    e.push_back({v, u, 0, -cost});
  }

  pair<long long, long long> run(int s, int t, long long need) {
    assert(s != t);
    const long long INF = 4'000'000'000'000'000'000LL;
    long long flow = 0, cost = 0;
    while (flow < need) {
      vector<long long> d(n, INF);
      vector<int> p(n, -1);
      d[s] = 0;
      for (int k = 1; k < n; k++) {
        bool change = false;
        for (int id = 0; id < (int) e.size(); id++) {
          auto [u, v, cap, w] = e[id];
          if (cap && d[u] != INF && d[v] > d[u] + w) {
            d[v] = d[u] + w;
            p[v] = id;
            change = true;
          }
        }
        if (!change) {
          break;
        }
      }
      if (p[t] < 0) {
        break;
      }
      long long f = need - flow;
      for (int v = t; v != s; v = e[p[v]].u) {
        f = min(f, e[p[v]].cap);
      }
      for (int v = t; v != s; v = e[p[v]].u) {
        e[p[v]].cap -= f;
        e[p[v] ^ 1].cap += f;
      }
      flow += f;
      cost += f * d[t];
    }
    return {flow, cost};
  }
};
