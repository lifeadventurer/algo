#include <bits/stdc++.h>
#include <cassert>
using namespace std;
// clang-format off
#include "geometry/integer_geometry.cpp"
#include "geometry/floating_geometry.cpp"
#include "geometry/closest_pair.cpp"
#include "geometry/min_enclosing_circle.cpp"
#include "geometry/polygon_diameter.cpp"
#include "graph/hld.cpp"
#include "graph/hungarian.cpp"
#include "graph/virtual_tree.cpp"
#include "numeric/berlekamp_massey.cpp"
#include "numeric/gauss_binary.cpp"
#include "numeric/linear_recurrence.cpp"
#include "string/min_rotation.cpp"
// clang-format on

mt19937 rng(20261108);

int random_int(int l, int r) {
  return uniform_int_distribution<int>(l, r)(rng);
}

void assignment_tests() {
  for (int rep = 0; rep < 600; rep++) {
    int n = random_int(0, 5), m = random_int(0, 6);
    vector<vector<long long>> cost(n, vector<long long>(m));
    for (auto& row : cost) {
      for (auto& x : row) {
        x = random_int(-30, 30);
      }
    }
    auto result = hungarian(cost);
    assert(result.has_value() == (n <= m));
    if (!result) {
      continue;
    }
    long long best = LLONG_MAX;
    function<void(int, int, long long)> brute = [&](int row, int used, long long value) {
      if (row == n) {
        best = min(best, value);
        return;
      }
      for (int col = 0; col < m; col++) {
        if (!(used >> col & 1)) {
          brute(row + 1, used | (1 << col), value + cost[row][col]);
        }
      }
    };
    brute(0, 0, 0);
    assert(result->first == best && (int) result->second.size() == n);
    set<int> used;
    long long actual = 0;
    for (int i = 0; i < n; i++) {
      int j = result->second[i];
      assert(0 <= j && j < m && used.insert(j).second);
      actual += cost[i][j];
    }
    assert(actual == best);
  }
  assert(hungarian({{LLONG_MIN}})->first == LLONG_MIN);
  assert(hungarian({{LLONG_MAX}})->first == LLONG_MAX);
  assert(hungarian({{LLONG_MIN, LLONG_MAX}, {0, LLONG_MAX}})->first == -1);
}

template <int MOD>
void recurrence_tests() {
  for (int rep = 0; rep < 150; rep++) {
    int d = random_int(1, 8);
    vector<int> c(d), values(90);
    for (int& x : c) {
      x = random_int(0, MOD - 1);
    }
    for (int i = 0; i < d; i++) {
      values[i] = random_int(0, MOD - 1);
    }
    for (int i = d; i < (int) values.size(); i++) {
      for (int j = 0; j < d; j++) {
        values[i] = (values[i] + (long long) c[j] * values[i - j - 1]) % MOD;
      }
    }
    vector<int> initial(values.begin(), values.begin() + d);
    vector<int> sample(values.begin(), values.begin() + 2 * d + 8);
    auto found = berlekamp_massey<MOD>(sample);
    assert((int) found.size() <= d);
    vector<int> short_initial(values.begin(), values.begin() + found.size());
    for (int i = 0; i < (int) values.size(); i++) {
      assert(linear_recurrence<MOD>(initial, c, i) == values[i]);
      assert(linear_recurrence<MOD>(short_initial, found, i) == values[i]);
    }
  }
  assert(berlekamp_massey<MOD>({}).empty());
  assert(berlekamp_massey<MOD>(vector<int>(20)).empty());
  assert(linear_recurrence<MOD>({}, {}, UINT64_MAX) == 0);
}

void binary_and_rotation_tests() {
  for (int rep = 0; rep < 800; rep++) {
    int n = random_int(0, 12), m = random_int(0, 8);
    vector<bitset<9>> rows(n);
    for (auto& row : rows) {
      for (int j = 0; j <= m; j++) {
        row[j] = random_int(0, 1);
      }
    }
    vector<int> solutions;
    for (int mask = 0; mask < (1 << m); mask++) {
      bool valid = true;
      for (auto row : rows) {
        int parity = 0;
        for (int j = 0; j < m; j++) {
          parity ^= (mask >> j & 1) && row[j];
        }
        valid &= parity == row[m];
      }
      if (valid) {
        solutions.push_back(mask);
      }
    }
    bitset<8> answer;
    int rank = gauss_binary<8>(rows, m, answer);
    assert((rank == -1) == solutions.empty());
    if (rank >= 0) {
      assert((int) solutions.size() == (1 << (m - rank)));
      assert(find(solutions.begin(), solutions.end(), answer.to_ulong()) != solutions.end());
    }
    string text;
    for (int i = random_int(0, 35); i > 0; i--) {
      text += (char) random_int(0, 255);
    }
    int at = min_rotation(text);
    if (text.empty()) {
      assert(at == 0);
      continue;
    }
    string expected = text;
    for (int i = 0; i < (int) text.size(); i++) {
      expected = min(expected, text.substr(i) + text.substr(0, i));
    }
    assert(text.substr(at) + text.substr(0, at) == expected);
  }
  for (const string text : {"aaaa", "baba", "caba", "z", ""}) {
    int at = min_rotation(text);
    for (int i = 0; i < (int) text.size(); i++) {
      assert(text.substr(at) + text.substr(0, at) <= text.substr(i) + text.substr(0, i));
    }
  }
  for (int n = 1; n <= 12; n++) {
    for (int mask = 0; mask < (1 << n); mask++) {
      string text(n, 'a');
      for (int i = 0; i < n; i++) {
        text[i] += mask >> i & 1;
      }
      int best = 0;
      for (int i = 1; i < n; i++) {
        if (text.substr(i) + text.substr(0, i) < text.substr(best) + text.substr(0, best)) {
          best = i;
        }
      }
      assert(min_rotation(text) == best);
    }
  }
  vector<bitset<131>> wide(130);
  for (int i = 0; i < 130; i++) {
    wide[i][i] = 1;
    wide[i][130] = i % 2;
  }
  bitset<130> answer;
  assert(gauss_binary<130>(wide, 130, answer) == 130);
  for (int i = 0; i < 130; i++) {
    assert(answer[i] == i % 2);
  }
  bitset<131> contradiction;
  contradiction[130] = 1;
  wide.push_back(contradiction);
  assert(gauss_binary<130>(wide, 130, answer) == -1);
}

void virtual_tree_tests() {
  for (int rep = 0; rep < 300; rep++) {
    int n = random_int(1, 45);
    vector<vector<int>> g(n);
    for (int i = 1; i < n; i++) {
      int p = random_int(0, i - 1);
      g[i].push_back(p);
      g[p].push_back(i);
    }
    HLD tree(g, random_int(0, n - 1));
    auto ancestor = [&](int a, int b) {
      return tree.pos[a] <= tree.pos[b] && tree.pos[b] < tree.pos[a] + tree.size[a];
    };
    auto lca = [&](int a, int b) {
      while (a != b) {
        if (tree.depth[a] < tree.depth[b]) {
          swap(a, b);
        }
        a = tree.parent[a];
      }
      return a;
    };
    vector<int> marked;
    for (int i = random_int(0, 30); i > 0; i--) {
      marked.push_back(random_int(0, n - 1));
    }
    VirtualTree small(tree, marked);
    set<int> closure(marked.begin(), marked.end());
    for (int a : marked) {
      for (int b : marked) {
        closure.insert(lca(a, b));
      }
    }
    assert(set<int>(small.vertex.begin(), small.vertex.end()) == closure);
    int unique_count = set<int>(marked.begin(), marked.end()).size();
    assert((int) small.vertex.size() <= max(0, 2 * unique_count - 1));
    int edges = 0;
    for (int i = 0; i < (int) small.vertex.size(); i++) {
      int expected = -1;
      for (int j = 0; j < (int) small.vertex.size(); j++) {
        if (i != j && ancestor(small.vertex[j], small.vertex[i]) &&
            (expected == -1 || tree.depth[small.vertex[j]] > tree.depth[small.vertex[expected]])) {
          expected = j;
        }
      }
      assert(small.parent[i] == expected);
      for (auto [child, distance] : small.g[i]) {
        assert(small.parent[child] == i);
        assert(distance == tree.depth[small.vertex[child]] - tree.depth[small.vertex[i]]);
        edges++;
      }
    }
    assert(edges == max(0, (int) small.vertex.size() - 1));
  }
  int n = 100000;
  vector<vector<int>> chain(n);
  for (int i = 1; i < n; i++) {
    chain[i].push_back(i - 1);
    chain[i - 1].push_back(i);
  }
  HLD tree(chain);
  VirtualTree small(tree, {12345, 99999, 12345});
  assert((small.vertex == vector<int>{12345, 99999}));
  assert((small.g[0] == vector<pair<int, int>>{{1, 87654}}));
}

long long squared_distance(Point a, Point b) {
  a = a - b;
  return (long long) ((__int128) a.x * a.x + (__int128) a.y * a.y);
}

void geometry_tests() {
  for (int rep = 0; rep < 600; rep++) {
    vector<Point> points;
    for (int i = random_int(0, 40); i > 0; i--) {
      points.push_back({random_int(-30, 30), random_int(-30, 30)});
    }
    auto closest = closest_pair(points);
    assert(closest.has_value() == (points.size() >= 2));
    long long nearest = LLONG_MAX, farthest = 0;
    for (int i = 0; i < (int) points.size(); i++) {
      for (int j = i + 1; j < (int) points.size(); j++) {
        nearest = min(nearest, squared_distance(points[i], points[j]));
        farthest = max(farthest, squared_distance(points[i], points[j]));
      }
    }
    if (closest) {
      auto [distance, a, b] = *closest;
      assert(a != b && distance == nearest && squared_distance(points[a], points[b]) == nearest);
    }
    auto hull = convex_hull(points);
    auto diameter = polygon_diameter(hull);
    assert(diameter.has_value() == !points.empty());
    if (diameter) {
      auto [distance, a, b] = *diameter;
      assert(distance == farthest && squared_distance(hull[a], hull[b]) == farthest);
    }
  }
  vector<Point> extremes = {{-1000000000, -1000000000}, {1000000000, 1000000000}};
  assert(get<0>(*closest_pair(extremes)) == 8000000000000000000LL);
  assert(get<0>(*polygon_diameter(extremes)) == 8000000000000000000LL);

  using namespace Geo;
  assert(!minimum_enclosing_circle({}));
  for (int rep = 0; rep < 500; rep++) {
    vector<P> points;
    for (int i = random_int(1, 8); i > 0; i--) {
      points.push_back({(D) random_int(-8, 8), (D) random_int(-8, 8)});
    }
    D best = numeric_limits<D>::infinity();
    auto consider = [&](P center) {
      D radius = 0;
      for (P p : points) {
        radius = max(radius, norm(p - center));
      }
      best = min(best, radius);
    };
    for (P p : points) {
      consider(p);
    }
    for (P a : points) {
      for (P b : points) {
        consider((a + b) * 0.5L);
      }
    }
    for (P a : points) {
      for (P b : points) {
        for (P c : points) {
          D det = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
          if (det == 0) {
            continue;
          }
          D aa = dot(a, a), bb = dot(b, b), cc = dot(c, c);
          consider({(aa * (b.y - c.y) + bb * (c.y - a.y) + cc * (a.y - b.y)) / det,
                    (aa * (c.x - b.x) + bb * (a.x - c.x) + cc * (b.x - a.x)) / det});
        }
      }
    }
    auto result = minimum_enclosing_circle(points, rep);
    assert(result && fabsl(result->radius - best) < 1e-7L);
    for (P p : points) {
      assert(norm(p - result->center) <= result->radius + 1e-7L);
    }
  }
  auto line = minimum_enclosing_circle({{-10, 0}, {0, 0}, {10, 0}, {10, 0}});
  assert(line && fabsl(line->radius - 10) < 1e-9L && norm(line->center) < 1e-9L);
}

int main(int argc, char** argv) {
  unsigned int seed = argc > 1 ? (unsigned int) stoul(argv[1]) : 20261108;
  rng.seed(seed);
  assignment_tests();
  recurrence_tests<2>();
  recurrence_tests<101>();
  recurrence_tests<1000000007>();
  recurrence_tests<2147483647>();
  binary_and_rotation_tests();
  virtual_tree_tests();
  geometry_tests();
  assert(linear_recurrence<1000>({0, 1}, {1, 1}, 10) == 55);
  assert(linear_recurrence<101>({-7}, {-1}, UINT64_MAX) == 7);
  assert((berlekamp_massey<101>({-7, 7, -7, 7}) == vector<int>{100}));
  cout << "Seed " << seed << ": nine new topics passed differential and boundary tests.\n";
}
