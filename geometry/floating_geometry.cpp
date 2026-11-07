// Absolute EPS: adjust to input scale. Coordinates use long double.
namespace Geo {
using D = long double;
const D EPS = 1e-12L, PI = acosl(-1.L);

struct P {
  D x, y;

  P operator+(P p) const {
    return {x + p.x, y + p.y};
  }

  P operator-(P p) const {
    return {x - p.x, y - p.y};
  }

  P operator*(D k) const {
    return {x * k, y * k};
  }
};

D dot(P a, P b) {
  return a.x * b.x + a.y * b.y;
}

D cross(P a, P b) {
  return a.x * b.y - a.y * b.x;
}

D norm(P a) {
  return sqrtl(dot(a, a));
}

P perp(P a) {
  return {-a.y, a.x};
}

// Infinite lines a+t*(b-a), c+u*(d-c). nullopt=parallel/coincident.
optional<P> line_intersection(P a, P b, P c, P d) {
  D den = cross(b - a, d - c);
  if (fabsl(den) < EPS) {
    return nullopt;
  }
  return a + (b - a) * (cross(c - a, d - c) / den);
}

D distance_to_segment(P p, P a, P b) {
  P v = b - a;
  D len = dot(v, v);
  if (len < EPS * EPS) {
    return norm(p - a);
  }
  D t = clamp(dot(p - a, v) / len, (D) 0, (D) 1);
  return norm(p - (a + v * t));
}

// nullopt=coincident circles (infinitely many intersections).
// Empty=no intersection; singleton=tangent. Radii nonnegative.
optional<vector<P>> circle_intersections(P a, D r, P b, D s) {
  D d = norm(b - a);
  if (d < EPS) {
    if (fabsl(r - s) >= EPS) {
      return vector<P>{};
    }
    if (r < EPS) {
      return vector<P>{a};
    }
    return nullopt;
  }
  if (d > r + s + EPS || d < fabsl(r - s) - EPS) {
    return vector<P>{};
  }
  D x = (d * d + r * r - s * s) / (2 * d);
  D h2 = r * r - x * x;
  P unit = (b - a) * (1 / d);
  P mid = a + unit * x;
  if (h2 <= EPS * EPS) {
    return vector<P>{mid};
  }
  P offset = perp(unit) * sqrtl(h2);
  return vector<P>{mid + offset, mid - offset};
}
}  // namespace Geo
