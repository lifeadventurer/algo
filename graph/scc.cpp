// Iterative Kosaraju. Edge u->v implies comp[u]<=comp[v].
struct SCC {
  int count = 0;
  vector<int> comp;

  SCC(const vector<vector<int>>& g) {
    int n = (int) g.size();
    vector<vector<int>> rev(n);
    for (int v = 0; v < n; v++) {
      for (int u : g[v]) {
        rev[u].push_back(v);
      }
    }
    vector<int> seen(n), next(n), order;
    for (int s = 0; s < n; s++) {
      if (!seen[s]) {
        vector<int> st = {s};
        seen[s] = 1;
        while (!st.empty()) {
          int v = st.back();
          if (next[v] == (int) g[v].size()) {
            order.push_back(v);
            st.pop_back();
          } else {
            int u = g[v][next[v]++];
            if (!seen[u]) {
              seen[u] = 1;
              st.push_back(u);
            }
          }
        }
      }
    }
    comp.assign(n, -1);
    reverse(order.begin(), order.end());
    for (int s : order) {
      if (comp[s] < 0) {
        vector<int> st = {s};
        comp[s] = count;
        while (!st.empty()) {
          int v = st.back();
          st.pop_back();
          for (int u : rev[v]) {
            if (comp[u] < 0) {
              comp[u] = count;
              st.push_back(u);
            }
          }
        }
        count++;
      }
    }
  }
};
