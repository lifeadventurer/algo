// Left [0,n), right [0,m). Add edges to g[left].
struct HopcroftKarp {
  int n, m;
  vector<vector<int>> g;
  vector<int> left, right, d;

  HopcroftKarp(int _n, int _m) : n(_n), m(_m), g(n), left(n, -1), right(m, -1), d(n) {}

  bool dfs(int v, int limit) {
    for (int u : g[v]) {
      int w = right[u];
      if ((w < 0 && d[v] + 1 == limit) || (w >= 0 && d[w] == d[v] + 1 && dfs(w, limit))) {
        left[v] = u;
        right[u] = v;
        return true;
      }
    }
    d[v] = INT_MAX;
    return false;
  }

  int run() {
    int ans = count_if(left.begin(), left.end(), [](int x) {
      return x >= 0;
    });
    while (true) {
      queue<int> q;
      fill(d.begin(), d.end(), INT_MAX);
      for (int v = 0; v < n; v++) {
        if (left[v] < 0) {
          d[v] = 0;
          q.push(v);
        }
      }
      int limit = INT_MAX;
      while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (d[v] >= limit) {
          continue;
        }
        for (int u : g[v]) {
          int w = right[u];
          if (w < 0) {
            limit = d[v] + 1;
          } else if (d[w] == INT_MAX) {
            d[w] = d[v] + 1;
            q.push(w);
          }
        }
      }
      if (limit == INT_MAX) {
        return ans;
      }
      for (int v = 0; v < n; v++) {
        if (left[v] < 0 && dfs(v, limit)) {
          ans++;
        }
      }
    }
  }
};
