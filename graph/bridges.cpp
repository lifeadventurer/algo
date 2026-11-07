// Undirected multigraph: add each edge once via add().
struct Lowlink {
  int timer = 0;
  vector<vector<pair<int, int>>> g;
  vector<int> tin, low;
  vector<bool> bridge, cut;

  Lowlink(int _n) : g(_n), tin(_n, -1), low(_n), cut(_n) {}

  void add(int a, int b) {
    int id = (int) bridge.size();
    bridge.push_back(false);
    g[a].push_back({b, id});
    g[b].push_back({a, id});
  }

  void dfs(int v, int pe = -1) {
    tin[v] = low[v] = timer++;
    int children = 0;
    for (auto [u, id] : g[v]) {
      if (id == pe) {
        continue;
      }
      if (tin[u] >= 0) {
        low[v] = min(low[v], tin[u]);
      } else {
        dfs(u, id);
        children++;
        low[v] = min(low[v], low[u]);
        bridge[id] = (low[u] > tin[v]);
        if (pe != -1 && low[u] >= tin[v]) {
          cut[v] = true;
        }
      }
    }
    if (pe == -1) {
      cut[v] = (children > 1);
    }
  }

  void run() {
    for (int v = 0; v < (int) g.size(); v++) {
      if (tin[v] < 0) {
        dfs(v);
      }
    }
  }
};
