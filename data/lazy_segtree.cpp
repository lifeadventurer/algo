// Range add and range sum, 0-based half-open intervals.
// Node sums (including after pushing), lazy tags, and answers must fit long long.
struct LazySegtree {
  int n;
  vector<long long> sum, lazy;

  LazySegtree(int _n) : n(_n), sum(4 * max(1, n)), lazy(4 * max(1, n)) {}

  void apply(int v, int l, int r, long long x) {
    __int128 next_sum = (__int128) sum[v] + (__int128) (r - l) * x;
    assert(LLONG_MIN <= next_sum && next_sum <= LLONG_MAX);
    sum[v] = (long long) next_sum;
    lazy[v] += x;
  }

  void push(int v, int l, int r) {
    int m = (l + r) / 2;
    apply(v * 2, l, m, lazy[v]);
    apply(v * 2 + 1, m, r, lazy[v]);
    lazy[v] = 0;
  }

  void add(int a, int b, long long x, int v, int l, int r) {
    if (b <= l || r <= a) {
      return;
    }
    if (a <= l && r <= b) {
      apply(v, l, r, x);
      return;
    }
    push(v, l, r);
    int m = (l + r) / 2;
    add(a, b, x, v * 2, l, m);
    add(a, b, x, v * 2 + 1, m, r);
    sum[v] = sum[v * 2] + sum[v * 2 + 1];
  }

  __int128 query(int a, int b, int v, int l, int r) {
    if (b <= l || r <= a) {
      return 0;
    }
    if (a <= l && r <= b) {
      return sum[v];
    }
    push(v, l, r);
    int m = (l + r) / 2;
    return query(a, b, v * 2, l, m) + query(a, b, v * 2 + 1, m, r);
  }

  void add(int l, int r, long long x) {
    if (l < r) {
      add(l, r, x, 1, 0, n);
    }
  }

  long long query(int l, int r) {
    __int128 ans = l < r ? query(l, r, 1, 0, n) : 0;
    assert(LLONG_MIN <= ans && ans <= LLONG_MAX);
    return (long long) ans;
  }
};
