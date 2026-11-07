// No path compression. Save snapshot(), then rollback(s).
struct RollbackDSU {
  vector<int> p;
  vector<pair<int, int>> hist;
  int components;

  RollbackDSU(int _n) : p(_n, -1), components(_n) {}

  int find(int x) const {
    while (p[x] >= 0) {
      x = p[x];
    }
    return x;
  }

  int snapshot() const {
    return (int) hist.size();
  }

  bool unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a == b) {
      return false;
    }
    if (p[a] > p[b]) {
      swap(a, b);
    }
    hist.push_back({a, p[a]});
    hist.push_back({b, p[b]});
    p[a] += p[b];
    p[b] = a;
    components--;
    return true;
  }

  void rollback(int s) {
    while (snapshot() > s) {
      auto [b, pb] = hist.back();
      hist.pop_back();
      auto [a, pa] = hist.back();
      hist.pop_back();
      p[a] = pa;
      p[b] = pb;
      components++;
    }
  }
};
