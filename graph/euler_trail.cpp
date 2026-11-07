// Directed graph. Edges listed once; returns edge IDs.
// nullopt if no trail from s uses every edge. Empty is valid.
optional<vector<int>> euler_trail(int n, const vector<pair<int, int>>& edges, int s) {
  vector<vector<pair<int, int>>> g(n);
  vector<int> in(n), out(n), it(n);
  for (int i = 0; i < (int) edges.size(); i++) {
    auto [u, v] = edges[i];
    g[u].push_back({v, i});
    out[u]++;
    in[v]++;
  }
  int plus = 0, minus = 0;
  for (int v = 0; v < n; v++) {
    int delta = out[v] - in[v];
    if (delta == 1) {
      plus++;
      if (v != s) {
        return nullopt;
      }
    } else if (delta == -1) {
      minus++;
    } else if (delta != 0) {
      return nullopt;
    }
  }
  if (!((plus == 0 && minus == 0) || (plus == 1 && minus == 1))) {
    return nullopt;
  }
  vector<pair<int, int>> st = {{s, -1}};
  vector<int> ans;
  while (!st.empty()) {
    int v = st.back().first;
    if (it[v] < (int) g[v].size()) {
      st.push_back(g[v][it[v]++]);
    } else {
      if (st.back().second != -1) {
        ans.push_back(st.back().second);
      }
      st.pop_back();
    }
  }
  if (ans.size() != edges.size()) {
    return nullopt;
  }
  reverse(ans.begin(), ans.end());
  return ans;
}
