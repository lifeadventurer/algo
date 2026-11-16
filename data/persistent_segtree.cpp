// Point-add, range-sum; root 0 is an all-zero tree.
// Stored node sums and final query answers must fit long long.
struct PersistentSegtree {
  struct Node {
    int l = 0, r = 0;
    long long sum = 0;
  };

  vector<Node> t = {Node{}};
  int n;

  PersistentSegtree(int _n) : n(_n) {
    assert(n > 0);
  }

  int add(int old, int p, long long x, int l, int r) {
    int v = (int) t.size();
    t.push_back(t[old]);
    t[v].sum += x;
    if (r - l > 1) {
      int m = l + (r - l) / 2;
      if (p < m) {
        int c = add(t[v].l, p, x, l, m);
        t[v].l = c;
      } else {
        int c = add(t[v].r, p, x, m, r);
        t[v].r = c;
      }
    }
    return v;
  }

  int add(int root, int p, long long x) {
    assert(0 <= p && p < n);
    return add(root, p, x, 0, n);
  }

  __int128 query(int v, int a, int b, int l, int r) const {
    if (!v || b <= l || r <= a) {
      return 0;
    }
    if (a <= l && r <= b) {
      return t[v].sum;
    }
    int m = l + (r - l) / 2;
    return query(t[v].l, a, b, l, m) + query(t[v].r, a, b, m, r);
  }

  long long query(int root, int l, int r) const {
    __int128 ans = query(root, l, r, 0, n);
    assert(LLONG_MIN <= ans && ans <= LLONG_MAX);
    return (long long) ans;
  }

  // Counts in after - before must be nonnegative at each coordinate.
  // k is 1-based. Return the 0-based coordinate, or -1 for an invalid rank.
  int kth(int before, int after, __int128 k) const {
    if (k <= 0 || k > (__int128) t[after].sum - t[before].sum) {
      return -1;
    }
    int l = 0, r = n;
    while (r - l > 1) {
      int m = l + (r - l) / 2;
      int a = t[before].l, b = t[after].l;
      __int128 count = (__int128) t[b].sum - t[a].sum;
      if (k <= count) {
        before = a;
        after = b;
        r = m;
      } else {
        k -= count;
        before = t[before].r;
        after = t[after].r;
        l = m;
      }
    }
    return l;
  }
};
