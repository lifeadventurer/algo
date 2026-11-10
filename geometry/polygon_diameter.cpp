// Requires integer_geometry.cpp. Use a strict CCW hull (no repeated endpoint).
// Coordinates |x|,|y|<=1e9. Returns (squared distance, hull index, hull index).
// O(n) rotating calipers; empty -> nullopt, singleton -> (0,0,0).
optional<tuple<long long, int, int>> polygon_diameter(const vector<Point>& hull) {
  int n = (int) hull.size();
  if (n == 0) {
    return nullopt;
  }
  long long best = 0;
  int first = 0, second = 0;
  auto relax = [&](int i, int j) {
    Point delta = hull[i] - hull[j];
    long long distance = (long long) ((__int128) delta.x * delta.x + (__int128) delta.y * delta.y);
    if (distance > best) {
      best = distance;
      first = i;
      second = j;
    }
  };
  if (n == 2) {
    relax(0, 1);
  } else if (n >= 3) {
    int j = 1;
    for (int i = 0; i < n; i++) {
      int next = (i + 1) % n;
      auto area = [&](int k) {
        return orient(hull[i], hull[next], hull[k]);
      };
      while (area((j + 1) % n) > area(j)) {
        j = (j + 1) % n;
      }
      relax(i, j);
      relax(next, j);
      if (area((j + 1) % n) == area(j)) {
        relax(i, (j + 1) % n);
        relax(next, (j + 1) % n);
      }
    }
  }
  return make_tuple(best, first, second);
}
