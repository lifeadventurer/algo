// Nonnegative capacities; total flow must fit long long. DFS is recursive.
struct Dinic {
  struct Edge {
    int to;
    long long cap;
  };

  vector<Edge> e;
  vector<vector<int>> g;
  vector<int> level, ptr;

  Dinic(int _n) : g(_n), level(_n), ptr(_n) {}

  void add(int u, int v, long long cap) {
    g[u].push_back(e.size());
    e.push_back({v, cap});
    g[v].push_back(e.size());
    e.push_back({u, 0});
  }

  bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    queue<int> q;
    q.push(s);
    level[s] = 0;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int id : g[v]) {
        if (e[id].cap && level[e[id].to] < 0) {
          level[e[id].to] = level[v] + 1;
          q.push(e[id].to);
        }
      }
    }
    return level[t] >= 0;
  }

  long long dfs(int v, int t, long long pushed) {
    if (!pushed || v == t) {
      return pushed;
    }
    for (int& i = ptr[v]; i < (int) g[v].size(); i++) {
      int id = g[v][i], u = e[id].to;
      if (level[u] != level[v] + 1) {
        continue;
      }
      long long f = dfs(u, t, min(pushed, e[id].cap));
      if (f) {
        e[id].cap -= f;
        e[id ^ 1].cap += f;
        return f;
      }
    }
    return 0;
  }

  long long flow(int s, int t) {
    assert(s != t);
    long long ans = 0;
    while (bfs(s, t)) {
      fill(ptr.begin(), ptr.end(), 0);
      while (long long f = dfs(s, t, LLONG_MAX)) {
        ans += f;
      }
    }
    return ans;
  }
};
