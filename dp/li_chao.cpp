// Minimum lines at known, discrete x coordinates.
// |a|,|b|,|x|<=1e18; values and answers use __int128.
struct LiChao {
  struct Line {
    long long a = 0, b = 0;

    __int128 eval(long long x) const {
      return (__int128) a * x + b;
    }
  };

  vector<long long> xs;
  vector<Line> t;
  vector<bool> has;
  static constexpr __int128 INF = (__int128) 1 << 126;

  LiChao(vector<long long> x) : xs(move(x)) {
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    assert(!xs.empty());
    t.resize(4 * xs.size());
    has.resize(t.size());
  }

  void add(Line z, int v, int l, int r) {
    if (!has[v]) {
      t[v] = z;
      has[v] = true;
      return;
    }
    int m = (l + r) / 2;
    bool left = z.eval(xs[l]) < t[v].eval(xs[l]);
    bool mid = z.eval(xs[m]) < t[v].eval(xs[m]);
    if (mid) {
      swap(z, t[v]);
    }
    if (r - l == 1) {
      return;
    }
    if (left != mid) {
      add(z, v * 2, l, m);
    } else {
      add(z, v * 2 + 1, m, r);
    }
  }

  void add(long long a, long long b) {
    add({a, b}, 1, 0, (int) xs.size());
  }

  __int128 query(int p, int v, int l, int r) const {
    __int128 ans = has[v] ? t[v].eval(xs[p]) : INF;
    if (r - l == 1) {
      return ans;
    }
    int m = (l + r) / 2;
    return min(ans, p < m ? query(p, v * 2, l, m) : query(p, v * 2 + 1, m, r));
  }

  __int128 query(long long x) const {
    int p = (int) (lower_bound(xs.begin(), xs.end(), x) - xs.begin());
    assert(p < (int) xs.size() && xs[p] == x);
    return query(p, 1, 0, (int) xs.size());
  }
};
