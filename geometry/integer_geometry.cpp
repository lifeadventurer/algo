// |coordinates|<=1e9; exact predicates using __int128.
struct Point {
  long long x, y;

  bool operator<(Point p) const {
    return tie(x, y) < tie(p.x, p.y);
  }

  bool operator==(Point p) const {
    return x == p.x && y == p.y;
  }

  Point operator-(Point p) const {
    return {x - p.x, y - p.y};
  }
};

__int128 cross(Point a, Point b) {
  return (__int128) a.x * b.y - (__int128) a.y * b.x;
}

__int128 orient(Point a, Point b, Point c) {
  return cross(b - a, c - a);
}

int sign128(__int128 x) {
  return (x > 0) - (x < 0);
}

bool on_segment(Point a, Point b, Point p) {
  return orient(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
         min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

bool segments_intersect(Point a, Point b, Point c, Point d) {
  if (on_segment(a, b, c) || on_segment(a, b, d) || on_segment(c, d, a) || on_segment(c, d, b)) {
    return true;
  }
  return sign128(orient(a, b, c)) * sign128(orient(a, b, d)) < 0 &&
         sign128(orient(c, d, a)) * sign128(orient(c, d, b)) < 0;
}

// CCW hull, no repeated first point, excludes collinear interior.
vector<Point> convex_hull(vector<Point> p) {
  sort(p.begin(), p.end());
  p.erase(unique(p.begin(), p.end()), p.end());
  if (p.size() < 2) {
    return p;
  }
  vector<Point> h;
  for (auto q : p) {
    while (h.size() > 1 && orient(h[h.size() - 2], h.back(), q) <= 0) {
      h.pop_back();
    }
    h.push_back(q);
  }
  int lower = (int) h.size();
  for (int i = (int) p.size() - 2; i >= 0; i--) {
    while ((int) h.size() > lower && orient(h[h.size() - 2], h.back(), p[i]) <= 0) {
      h.pop_back();
    }
    h.push_back(p[i]);
  }
  h.pop_back();
  return h;
}

__int128 area2(const vector<Point>& p) {
  __int128 a = 0;
  for (int i = 0; i < (int) p.size(); i++) {
    a += cross(p[i], p[(i + 1) % p.size()]);
  }
  return a < 0 ? -a : a;
}

// Simple polygon, either orientation: -1 outside, 0 boundary, 1 inside.
int point_in_polygon(const vector<Point>& p, Point q) {
  int winding = 0;
  for (int i = 0; i < (int) p.size(); i++) {
    Point a = p[i], b = p[(i + 1) % p.size()];
    if (on_segment(a, b, q)) {
      return 0;
    }
    if (a.y <= q.y && b.y > q.y && orient(a, b, q) > 0) {
      winding++;
    }
    if (a.y > q.y && b.y <= q.y && orient(a, b, q) < 0) {
      winding--;
    }
  }
  return winding ? 1 : -1;
}
