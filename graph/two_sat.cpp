// Requires scc.cpp. Literal 2*v=false, 2*v+1=true.
struct TwoSAT {
  int n;
  vector<vector<int>> g;
  vector<int> value;

  TwoSAT(int _n) : n(_n), g(2 * n) {}

  void either(int a, int b) {
    g[a ^ 1].push_back(b);
    g[b ^ 1].push_back(a);
  }

  void force(int a) {
    either(a, a);
  }

  bool solve() {
    SCC s(g);
    value.resize(n);
    for (int i = 0; i < n; i++) {
      if (s.comp[2 * i] == s.comp[2 * i + 1]) {
        return false;
      }
      value[i] = s.comp[2 * i] < s.comp[2 * i + 1];
    }
    return true;
  }
};
