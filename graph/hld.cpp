// Tree, 0-based; iterative construction avoids deep recursion.
struct HLD {
  vector<int> parent, depth, size, heavy, head, pos;

  HLD(const vector<vector<int>>& g, int root = 0) {
    int n = (int) g.size();
    assert(n > 0);
    parent.assign(n, -1);
    depth.resize(n);
    size.assign(n, 1);
    heavy.assign(n, -1);
    head.resize(n);
    pos.resize(n);
    vector<int> order = {root};
    parent[root] = root;
    for (int i = 0; i < (int) order.size(); i++) {
      int v = order[i];
      for (int u : g[v]) {
        if (u != parent[v]) {
          parent[u] = v;
          depth[u] = depth[v] + 1;
          order.push_back(u);
        }
      }
    }
    for (int i = n - 1; i > 0; i--) {
      int v = order[i], p = parent[v];
      size[p] += size[v];
      if (heavy[p] < 0 || size[v] > size[heavy[p]]) {
        heavy[p] = v;
      }
    }
    int timer = 0;
    vector<pair<int, int>> st = {{root, root}};
    while (!st.empty()) {
      auto [v, h] = st.back();
      st.pop_back();
      for (; v != -1; v = heavy[v]) {
        head[v] = h;
        pos[v] = timer++;
        for (int u : g[v]) {
          if (parent[u] == v && u != heavy[v]) {
            st.push_back({u, u});
          }
        }
      }
    }
  }

  // Calls f(l,r) for vertex ranges [l,r); order unspecified.
  // For edge values at child positions, set edges=true.
  template <typename F>
  void path(int a, int b, F f, bool edges = false) {
    while (head[a] != head[b]) {
      if (depth[head[a]] < depth[head[b]]) {
        swap(a, b);
      }
      f(pos[head[a]], pos[a] + 1);
      a = parent[head[a]];
    }
    if (depth[a] > depth[b]) {
      swap(a, b);
    }
    if (pos[a] + edges < pos[b] + 1) {
      f(pos[a] + edges, pos[b] + 1);
    }
  }

  // Subtree of v is [pos[v], pos[v]+size[v]).
};
