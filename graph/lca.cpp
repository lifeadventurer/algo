struct LCA {
  int n, lg;
  vector<int> depth;
  vector<vector<int>> up;

  LCA(const vector<vector<int>>& g, int root) {
    n = (int) g.size();
    lg = 1;
    while ((1 << lg) <= n) {
      lg++;
    }
    depth.assign(n, 0);
    up.assign(lg, vector<int>(n));
    dfs(root, root, g);
    for (int j = 1; j < lg; j++) {
      for (int i = 0; i < n; i++) {
        up[j][i] = up[j - 1][up[j - 1][i]];
      }
    }
  }

  void dfs(int v, int p, const vector<vector<int>>& g) {
    up[0][v] = p;
    for (int u : g[v]) {
      if (u != p) {
        depth[u] = depth[v] + 1;
        dfs(u, v, g);
      }
    }
  }

  int get(int a, int b) {
    if (depth[a] < depth[b]) {
      swap(a, b);
    }
    int k = depth[a] - depth[b];
    for (int j = lg - 1; j >= 0; j--) {
      if (k & (1 << j)) {
        a = up[j][a];
      }
    }
    if (a == b) {
      return a;
    }
    for (int j = lg - 1; j >= 0; j--) {
      if (up[j][a] != up[j][b]) {
        a = up[j][a];
        b = up[j][b];
      }
    }
    return up[0][a];
  }
};
