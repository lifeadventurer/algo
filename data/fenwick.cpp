template <typename T>
class FenwickTree {
 public:
  int n;
  vector<T> fenw;

  FenwickTree(int _n) : n(_n) {
    fenw.assign(n, 0);
  }

  int lowbit(int x) {
    return x & -x;
  }

  void modify(int x, T val) {
    while (x < n) {
      fenw[x] += val;
      x += lowbit(x);
    }
  }

  T query(int x) {
    T val{};
    while (x > 0) {
      val += fenw[x];
      x -= lowbit(x);
    }
    return val;
  }

  // Nonnegative counts. Return the first index with prefix sum >= k.
  // k is 1-based. Return n for an invalid rank.
  int kth(T k) const {
    if (k <= T{} || n <= 1) {
      return n;
    }
    int x = 0, step = 1;
    while (step <= (n - 1) / 2) {
      step *= 2;
    }
    for (; step > 0; step /= 2) {
      int y = x + step;
      if (y < n && fenw[y] < k) {
        x = y;
        k -= fenw[y];
      }
    }
    return x + 1;
  }
};
