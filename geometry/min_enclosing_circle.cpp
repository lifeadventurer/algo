// Requires floating_geometry.cpp. Expected O(n) after random shuffle.
// Geo::EPS is a relative containment tolerance; adjust for input scale.
namespace Geo {
struct Circle {
  P center;
  D radius;
};

optional<Circle> minimum_enclosing_circle(vector<P> points, uint64_t seed = 712367821) {
  if (points.empty()) {
    return nullopt;
  }
  mt19937_64 rng(seed);
  shuffle(points.begin(), points.end(), rng);
  auto diameter = [](P a, P b) {
    P center = a + (b - a) * 0.5L;
    return Circle{center, norm(a - center)};
  };
  auto contains = [](Circle c, P p) {
    return norm(p - c.center) <= c.radius + EPS * max((D) 1, c.radius);
  };
  auto through_three = [&](P a, P b, P c) {
    P u = b - a, v = c - a;
    D det = 2 * cross(u, v);
    if (det == 0) {
      Circle result = diameter(a, b);
      for (Circle candidate : {diameter(a, c), diameter(b, c)}) {
        if (candidate.radius > result.radius) {
          result = candidate;
        }
      }
      return result;
    }
    D uu = dot(u, u), vv = dot(v, v);
    P offset = {(uu * v.y - vv * u.y) / det, (u.x * vv - v.x * uu) / det};
    return Circle{a + offset, norm(offset)};
  };
  Circle answer{points[0], 0};
  for (int i = 0; i < (int) points.size(); i++) {
    if (contains(answer, points[i])) {
      continue;
    }
    answer = {points[i], 0};
    for (int j = 0; j < i; j++) {
      if (contains(answer, points[j])) {
        continue;
      }
      answer = diameter(points[i], points[j]);
      for (int k = 0; k < j; k++) {
        if (!contains(answer, points[k])) {
          answer = through_three(points[i], points[j], points[k]);
        }
      }
    }
  }
  return answer;
}
}  // namespace Geo
