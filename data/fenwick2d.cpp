template <typename T>
class FenwickTree2D {
 public:
  int n, m;
  vector<vector<T>> fenw;

  FenwickTree2D(int _n, int _m) : n(_n), m(_m) {
    fenw.assign(n, vector<T>(m, 0));
  }

  int lowbit(int x) {
    return x & -x;
  }

  void modify(int x, int y, T val) {
    while (x < n) {
      int yy = y;
      while (yy < m) {
        fenw[x][yy] += val;
        yy += lowbit(yy);
      }
      x += lowbit(x);
    }
  }

  T query(int x, int y) {
    T val{};
    while (x > 0) {
      int yy = y;
      while (yy > 0) {
        val += fenw[x][yy];
        yy -= lowbit(yy);
      }
      x -= lowbit(x);
    }
    return val;
  }

  T query(int x1, int y1, int x2, int y2) {
    return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
  }
};
