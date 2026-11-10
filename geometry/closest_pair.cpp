// Requires integer_geometry.cpp (Point). Coordinates |x|,|y|<=1e9.
// Returns (squared distance, original index, original index), or nullopt.
// O(n log n) time, O(n) extra space. Input is not modified.
optional<tuple<long long, int, int>> closest_pair(const vector<Point>& points) {
  int n = (int) points.size();
  if (n < 2) {
    return nullopt;
  }

  struct Node {
    Point p;
    int id;
  };

  vector<Node> a(n), buffer(n);
  for (int i = 0; i < n; i++) {
    a[i] = {points[i], i};
  }
  sort(a.begin(), a.end(), [](const Node& x, const Node& y) {
    return x.p < y.p;
  });
  auto by_y = [](const Node& x, const Node& y) {
    return tie(x.p.y, x.p.x) < tie(y.p.y, y.p.x);
  };
  long long best = LLONG_MAX;
  int first = -1, second = -1;
  auto relax = [&](const Node& x, const Node& y) {
    Point delta = x.p - y.p;
    long long distance = (long long) ((__int128) delta.x * delta.x + (__int128) delta.y * delta.y);
    if (distance < best) {
      best = distance;
      first = x.id;
      second = y.id;
    }
  };
  function<void(int, int)> solve = [&](int l, int r) {
    if (r - l <= 3) {
      for (int i = l; i < r; i++) {
        for (int j = i + 1; j < r; j++) {
          relax(a[i], a[j]);
        }
      }
      sort(a.begin() + l, a.begin() + r, by_y);
      return;
    }
    int m = (l + r) / 2;
    long long mid_x = a[m].p.x;
    solve(l, m);
    solve(m, r);
    merge(a.begin() + l, a.begin() + m, a.begin() + m, a.begin() + r, buffer.begin() + l, by_y);
    copy(buffer.begin() + l, buffer.begin() + r, a.begin() + l);
    vector<Node> strip;
    for (int i = l; i < r; i++) {
      long long dx = a[i].p.x - mid_x;
      if ((__int128) dx * dx >= best) {
        continue;
      }
      for (int j = (int) strip.size() - 1; j >= 0; j--) {
        long long dy = a[i].p.y - strip[j].p.y;
        if ((__int128) dy * dy >= best) {
          break;
        }
        relax(a[i], strip[j]);
      }
      strip.push_back(a[i]);
    }
  };
  solve(0, n);
  return make_tuple(best, first, second);
}
