#include <bits/stdc++.h>

#include <cassert>
using namespace std;
#include "data/dsu.cpp"
#include "data/fenwick.cpp"
#include "data/fenwick2d.cpp"
#include "data/lazy_segtree.cpp"
#include "data/persistent_segtree.cpp"
#include "data/rollback_dsu.cpp"
#include "data/segtree.cpp"
#include "data/sparsetable.cpp"
#include "dp/li_chao.cpp"
#include "dp/lis.cpp"
#include "dp/subset_transforms.cpp"
#include "geometry/floating_geometry.cpp"
#include "geometry/integer_geometry.cpp"
#include "graph/bellman_ford.cpp"
#include "graph/bfs.cpp"
#include "graph/bridges.cpp"
#include "graph/dijkstra.cpp"
#include "graph/dinic.cpp"
#include "graph/euler_trail.cpp"
#include "graph/floyd_warshall.cpp"
#include "graph/hld.cpp"
#include "graph/hopcroft_karp.cpp"
#include "graph/kruskal.cpp"
#include "graph/lca.cpp"
#include "graph/mincost_flow.cpp"
#include "graph/scc.cpp"
#include "graph/toposort.cpp"
#include "graph/two_sat.cpp"
#include "graph/zero_one_bfs.cpp"
#include "misc/int128_io.cpp"
#include "numeric/combinatorics.cpp"
#include "numeric/crt.cpp"
#include "numeric/extgcd.cpp"
#include "numeric/fastpow.cpp"
#include "numeric/gauss.cpp"
#include "numeric/miller_rabin.cpp"
#include "numeric/ntt.cpp"
#include "numeric/pollard_rho.cpp"
#include "numeric/sieve.cpp"
#include "string/aho_corasick.cpp"
#include "string/manacher.cpp"
#include "string/prefix_kmp.cpp"
#include "string/suffix_array.cpp"
#include "string/suffix_automaton.cpp"
#include "string/z_function.cpp"

mt19937 gen(20261106);

int rnd(int l, int r) {
  return uniform_int_distribution<int>(l, r)(gen);
}

const long long INF = 4'000'000'000'000'000'000LL;

void data_tests() {
  for (int rep = 0; rep < 120; ++rep) {
    int n = rnd(1, 35);
    LazySegtree seg(n);
    FenwickTree<long long> bit(n + 1);
    vector<long long> a(n);
    PersistentSegtree pst(n);
    vector<int> roots = {0};
    vector<vector<long long>> states = {a};
    for (int k = 0; k < 120; ++k) {
      int l = rnd(0, n), r = rnd(l, n), p = rnd(0, n - 1), x = rnd(-100, 100);
      seg.add(l, r, x);
      for (int i = l; i < r; ++i) {
        a[i] += x, bit.modify(i + 1, x);
      }
      l = rnd(0, n);
      r = rnd(l, n);
      long long expect = accumulate(a.begin() + l, a.begin() + r, 0LL);
      assert(seg.query(l, r) == expect && bit.query(r) - bit.query(l) == expect);
      int ver = rnd(0, roots.size() - 1);
      auto b = states[ver];
      b[p] += x;
      roots.push_back(pst.add(roots[ver], p, x));
      states.push_back(b);
      ver = rnd(0, roots.size() - 1);
      assert(pst.query(roots[ver], l, r) ==
             accumulate(states[ver].begin() + l, states[ver].begin() + r, 0LL));
    }
    RollbackDSU d(n);
    DSU original(n);
    for (int k = 0; k < 100; ++k) {
      int p = rnd(0, n - 1), q = rnd(0, n - 1);
      assert(d.unite(p, q) == original.unite(p, q));
      int snap = d.snapshot(), count = d.components;
      auto saved = d.p;
      for (int j = 0; j < 10; ++j) {
        d.unite(rnd(0, n - 1), rnd(0, n - 1));
      }
      d.rollback(snap);
      assert(d.p == saved && d.components == count);
    }
    Segtree mx(n);
    vector<int> values(n);
    for (int i = 0; i < n; ++i) {
      mx.modify(i, values[i] = rnd(0, 1000));
    }
    auto op = [](int a, int b) {
      return min(a, b);
    };
    SparseTable st(values, op);
    for (int l = 0; l < n; ++l) {
      for (int r = l; r < n; ++r) {
        assert(mx.query(l, r + 1) == *max_element(values.begin() + l, values.begin() + r + 1));
        assert(st.get(l, r) == *min_element(values.begin() + l, values.begin() + r + 1));
      }
    }
  }
  FenwickTree2D<int> f(6, 7);
  f.modify(2, 3, 5);
  f.modify(5, 6, 8);
  assert(f.query(1, 1, 5, 6) == 13 && f.query(2, 3, 2, 3) == 5);
}

void graph_tests() {
  for (int rep = 0; rep < 200; ++rep) {
    int n = rnd(1, 8);
    vector<vector<int>> g(n);
    vector<vector<pair<int, int>>> z(n);
    vector<vector<pair<int, long long>>> w(n);
    vector<vector<long long>> d(n, vector<long long>(n, INF));
    vector<vector<bool>> reach(n, vector<bool>(n));
    vector<BFEdge> edges;
    for (int v = 0; v < n; ++v) {
      d[v][v] = 0, reach[v][v] = true;
    }
    for (int i = 0; i < n * n; ++i) {
      if (rnd(0, 3) == 0) {
        int a = rnd(0, n - 1), b = rnd(0, n - 1), c = rnd(0, 1);
        g[a].push_back(b);
        z[a].push_back({b, c});
        w[a].push_back({b, c});
        edges.push_back({a, b, c});
        d[a][b] = min(d[a][b], (long long) c);
        reach[a][b] = true;
      }
    }
    for (int k = 0; k < n; ++k) {
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
        }
      }
    }
    SCC s(g);
    floyd_warshall(d);
    for (int i = 0; i < n; ++i) {
      auto zz = zero_one_bfs(z, i);
      auto dd = dijkstra(w, i);
      auto bf = bellman_ford(n, edges, i);
      auto b = bfs(g, i);
      for (int j = 0; j < n; ++j) {
        assert((s.comp[i] == s.comp[j]) == (reach[i][j] && reach[j][i]));
        assert(dd[j] == d[i][j] && bf.d[j] == d[i][j] && !bf.neg[j]);
        assert((zz[j] == INT_MAX ? INF : zz[j]) == d[i][j]);
        assert((b[j] >= 0) == reach[i][j]);
      }
      for (int u : g[i]) {
        assert(s.comp[i] <= s.comp[u]);
      }
    }
    auto order = toposort(g);
    bool cycle = false;
    for (int u = 0; u < n; ++u) {
      for (int v : g[u]) {
        cycle |= reach[v][u];
      }
    }
    assert(order.empty() == cycle);
    if (!cycle) {
      vector<int> at(n);
      for (int i = 0; i < n; ++i) {
        at[order[i]] = i;
      }
      for (int v = 0; v < n; ++v) {
        for (int u : g[v]) {
          assert(at[v] < at[u]);
        }
      }
    }
    // Signed weights: Floyd is an independent negative-cycle oracle.
    vector<BFEdge> signed_edges;
    vector<vector<long long>> sd(n, vector<long long>(n, INF));
    for (int i = 0; i < n; ++i) {
      sd[i][i] = 0;
    }
    for (auto e : edges) {
      e.w = rnd(-5, 8);
      signed_edges.push_back(e);
      sd[e.u][e.v] = min(sd[e.u][e.v], e.w);
    }
    floyd_warshall(sd);
    for (int src = 0; src < n; ++src) {
      auto bf = bellman_ford(n, signed_edges, src);
      for (int v = 0; v < n; ++v) {
        bool bad = false;
        for (int k = 0; k < n; ++k) {
          bad |= reach[src][k] && sd[k][k] < 0 && reach[k][v];
        }
        assert(bf.neg[v] == bad);
        if (!bad) {
          assert(bf.d[v] == sd[src][v]);
        }
      }
    }
    TwoSAT sat(n);
    vector<pair<int, int>> clauses;
    for (int k = 0; k < 20; ++k) {
      int a = rnd(0, 2 * n - 1), b = rnd(0, 2 * n - 1);
      sat.either(a, b);
      clauses.push_back({a, b});
    }
    auto valid = [&](int mask) {
      for (auto [a, b] : clauses) {
        if (((mask >> (a / 2) & 1) != (a & 1)) && ((mask >> (b / 2) & 1) != (b & 1))) {
          return false;
        }
      }
      return true;
    };
    bool possible = false;
    for (int mask = 0; mask < (1 << n); ++mask) {
      possible |= valid(mask);
    }
    assert(sat.solve() == possible);
    if (possible) {
      int mask = 0;
      for (int i = 0; i < n; ++i) {
        mask |= sat.value[i] << i;
      }
      assert(valid(mask));
    }
  }
}

void tree_tests() {
  for (int rep = 0; rep < 100; ++rep) {
    int n = rnd(1, 60);
    vector<vector<int>> g(n);
    for (int v = 1; v < n; ++v) {
      int p = rnd(0, v - 1);
      g[v].push_back(p);
      g[p].push_back(v);
    }
    int root = rnd(0, n - 1);
    HLD h(g, root);
    LCA l(g, root);
    for (int k = 0; k < 100; ++k) {
      int a = rnd(0, n - 1), b = rnd(0, n - 1), u = a, v = b;
      vector<int> expect, actual;
      while (u != v) {
        if (h.depth[u] < h.depth[v]) {
          swap(u, v);
        }
        expect.push_back(h.pos[u]);
        u = h.parent[u];
      }
      assert(l.get(a, b) == u);
      auto get = [&](int x, int y) {
        for (; x < y; ++x) {
          actual.push_back(x);
        }
      };
      h.path(a, b, get, true);
      sort(expect.begin(), expect.end());
      sort(actual.begin(), actual.end());
      assert(expect == actual);
      expect.push_back(h.pos[u]);
      actual.clear();
      h.path(a, b, get);
      sort(expect.begin(), expect.end());
      sort(actual.begin(), actual.end());
      assert(expect == actual);
    }
    for (int v = 0; v < n; ++v) {
      for (int u = 0; u < n; ++u) {
        int x = u;
        while (h.depth[x] > h.depth[v]) {
          x = h.parent[x];
        }
        assert((x == v) == (h.pos[v] <= h.pos[u] && h.pos[u] < h.pos[v] + h.size[v]));
      }
    }
  }
  // Long-chain constructions that explicitly promise iterative traversal.
  int n = 100000;
  vector<vector<int>> chain(n), directed(n);
  for (int i = 1; i < n; ++i) {
    chain[i - 1].push_back(i), chain[i].push_back(i - 1), directed[i - 1].push_back(i);
  }
  HLD h(chain);
  SCC s(directed);
  assert(h.size[0] == n && s.count == n);
}

void flow_matching_tests() {
  for (int rep = 0; rep < 150; ++rep) {
    int n = rnd(2, 7);
    Dinic d(n);
    vector<tuple<int, int, int>> edges;
    for (int i = 0; i < 20; ++i) {
      int a = rnd(0, n - 1), b = rnd(0, n - 1), c = rnd(0, 6);
      d.add(a, b, c);
      edges.push_back({a, b, c});
    }
    long long best = LLONG_MAX;
    for (int mask = 1; mask < (1 << n); ++mask) {
      if ((mask & 1) && !(mask >> (n - 1) & 1)) {
        long long cost = 0;
        for (auto [a, b, c] : edges) {
          if ((mask >> a & 1) && !(mask >> b & 1)) {
            cost += c;
          }
        }
        best = min(best, cost);
      }
    }
    assert(d.flow(0, n - 1) == best);
    int m = rnd(1, 7);
    HopcroftKarp hk(n, m);
    for (int a = 0; a < n; ++a) {
      for (int b = 0; b < m; ++b) {
        if (rnd(0, 2) == 0) {
          hk.g[a].push_back(b);
        }
      }
    }
    function<int(int, int)> brute = [&](int a, int used) {
      if (a == n) {
        return 0;
      }
      int ans = brute(a + 1, used);
      for (int b : hk.g[a]) {
        if (!(used >> b & 1)) {
          ans = max(ans, 1 + brute(a + 1, used | (1 << b)));
        }
      }
      return ans;
    };
    assert(hk.run() == brute(0, 0));
    for (int a = 0; a < n; ++a) {
      if (hk.left[a] >= 0) {
        assert(hk.right[hk.left[a]] == a);
      }
    }
    MinCostFlow mf(n);
    vector<tuple<int, int, int, int>> me;
    for (int k = 0; k < 6; ++k) {
      int a = rnd(0, n - 2), b = rnd(a + 1, n - 1), cap = rnd(1, 2), cost = rnd(-5, 8);
      mf.add(a, b, cap, cost);
      me.push_back({a, b, cap, cost});
    }
    int need = rnd(0, 5), bestflow = -1;
    long long bestcost = LLONG_MAX;
    vector<int> balance(n);
    function<void(int, long long)> enumerate = [&](int k, long long cost) {
      if (k == (int) me.size()) {
        for (int i = 1; i < n - 1; ++i) {
          if (balance[i]) {
            return;
          }
        }
        int f = -balance[0];
        if (f < 0 || f > need || balance[n - 1] != f) {
          return;
        }
        if (f > bestflow || (f == bestflow && cost < bestcost)) {
          bestflow = f, bestcost = cost;
        }
        return;
      }
      auto [a, b, cap, c] = me[k];
      for (int f = 0; f <= cap; ++f) {
        balance[a] -= f;
        balance[b] += f;
        enumerate(k + 1, cost + f * c);
        balance[a] += f;
        balance[b] -= f;
      }
    };
    enumerate(0, 0);
    assert(mf.run(0, n - 1, need) == make_pair((long long) bestflow, bestcost));
  }
}

void connectivity_tests() {
  for (int rep = 0; rep < 150; ++rep) {
    int n = rnd(1, 7), m = rnd(0, 8);
    vector<pair<int, int>> es;
    vector<MSTEdge> weighted;
    Lowlink low(n);
    for (int k = 0; k < m; ++k) {
      int a = rnd(0, n - 1), b = rnd(0, n - 1);
      es.push_back({a, b});
      low.add(a, b);
      weighted.push_back({a, b, rnd(-5, 10)});
    }
    auto components = [&](int skipedge, int skipvertex) {
      DSU d(n);
      for (int i = 0; i < m; ++i) {
        if (i != skipedge && es[i].first != skipvertex && es[i].second != skipvertex) {
          d.unite(es[i].first, es[i].second);
        }
      }
      int c = 0;
      for (int v = 0; v < n; ++v) {
        if (v != skipvertex && d.get(v) == v) {
          ++c;
        }
      }
      return c;
    };
    low.run();
    int baseline = components(-1, -1);
    for (int i = 0; i < m; ++i) {
      assert(low.bridge[i] == (components(i, -1) > baseline));
    }
    for (int v = 0; v < n; ++v) {
      assert(low.cut[v] == (components(-1, v) > baseline));
    }
    long long best = LLONG_MAX;
    for (int mask = 0; mask < (1 << m); ++mask) {
      if (__builtin_popcount((unsigned) mask) == n - baseline) {
        DSU d(n);
        long long cost = 0;
        bool valid = true;
        for (int i = 0; i < m; ++i) {
          if (mask >> i & 1) {
            valid &= d.unite(es[i].first, es[i].second), cost += weighted[i].w;
          }
        }
        if (valid) {
          best = min(best, cost);
        }
      }
    }
    auto [cost, used] = kruskal(n, weighted);
    assert(cost == best && (int) used.size() == n - baseline);
    int start = rnd(0, n - 1);
    function<bool(int, int)> walk = [&](int v, int mask) {
      if (mask == (1 << m) - 1) {
        return true;
      }
      for (int i = 0; i < m; ++i) {
        if (!(mask >> i & 1) && es[i].first == v && walk(es[i].second, mask | (1 << i))) {
          return true;
        }
      }
      return false;
    };
    auto trail = euler_trail(n, es, start);
    assert(trail.has_value() == walk(start, 0));
    if (trail) {
      int v = start;
      vector<int> usededge(m);
      for (int id : *trail) {
        assert(es[id].first == v && !usededge[id]++);
        v = es[id].second;
      }
      assert((int) trail->size() == m);
    }
  }
}

void string_tests() {
  for (int rep = 0; rep < 400; ++rep) {
    int n = rnd(0, 30);
    string s;
    for (int i = 0; i < n; ++i) {
      s += char('a' + rnd(0, 2));
    }
    SuffixArray sa(s);
    vector<int> sorted(n);
    iota(sorted.begin(), sorted.end(), 0);
    sort(sorted.begin(), sorted.end(), [&](int a, int b) {
      return s.substr(a) < s.substr(b);
    });
    assert(sa.sa == sorted);
    for (int i = 0; i + 1 < n; ++i) {
      int a = sa.sa[i], b = sa.sa[i + 1], l = 0;
      while (a + l < n && b + l < n && s[a + l] == s[b + l]) {
        ++l;
      }
      assert(sa.lcp[i] == l);
    }
    auto [odd, even] = manacher(s);
    auto z = z_function(s);
    auto pi = prefix_function(s);
    for (int i = 0; i < n; ++i) {
      int o = 1, e = 0, l = 0, p = 0;
      while (i - o >= 0 && i + o < n && s[i - o] == s[i + o]) {
        ++o;
      }
      while (i - e - 1 >= 0 && i + e < n && s[i - e - 1] == s[i + e]) {
        ++e;
      }
      while (i + l < n && s[l] == s[i + l]) {
        ++l;
      }
      for (int k = 1; k <= i; ++k) {
        if (s.substr(0, k) == s.substr(i - k + 1, k)) {
          p = k;
        }
      }
      assert(odd[i] == o && even[i] == e && z[i] == l && pi[i] == p);
    }
    set<string> substrings;
    SuffixAutomaton sam;
    for (char c : s) {
      sam.extend(c);
    }
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j <= n; ++j) {
        substrings.insert(s.substr(i, j - i));
      }
    }
    assert(sam.distinct() == (long long) substrings.size());
    AhoCorasick ac;
    long long total = 0;
    for (int k = 0; k < 12; ++k) {
      string p;
      for (int i = rnd(0, 5); i > 0; --i) {
        p += char('a' + rnd(0, 2));
      }
      vector<int> expect;
      for (int i = 0; i + (int) p.size() <= n; ++i) {
        if (s.substr(i, p.size()) == p) {
          expect.push_back(i);
        }
      }
      assert(kmp(s, p) == expect);
      if (!p.empty()) {
        ac.add(p), total += expect.size();
      }
    }
    ac.build();
    assert(ac.count(s) == total);
  }
  string bytes;
  bytes.push_back('\0');
  bytes.push_back(char(255));
  bytes.push_back('a');
  SuffixArray sa(bytes);
  assert((sa.sa == vector<int>{0, 2, 1}));
}

void numeric_tests() {
  Sieve s(100000);
  for (int n = 0; n <= 100000; ++n) {
    assert(is_prime(n) == (n >= 2 && s.spf[n] == n));
  }
  for (int n = 1; n <= 1000; ++n) {
    int phi = 0;
    for (int k = 1; k <= n; ++k) {
      phi += gcd(k, n) == 1;
    }
    assert(s.phi[n] == phi);
    int prod = 1;
    for (auto [p, e] : s.factor(n)) {
      while (e--) {
        prod *= p;
      }
    }
    assert(prod == n);
    int sum = 0;
    for (int d = 1; d <= n; ++d) {
      if (n % d == 0) {
        sum += s.mu[d];
      }
    }
    assert(sum == (n == 1));
    long long x, y, g = extgcd((long long) n, 561LL, x, y);
    assert(n * x + 561 * y == g && g == gcd(n, 561));
    assert(fastpow(n, 7, 101) == (int) pow_mod(n, 7, 101));
  }
  assert(is_prime(18446744073709551557ULL));
  for (u64 n : {341550071728321ULL, 3825123056546413051ULL, 18446744073709551615ULL}) {
    assert(!is_prime(n));
  }
  vector<u64> numbers = {1, 2, 4, 561, 1000000007ULL * 1000000009ULL, 18446744073709551615ULL};
  for (int i = 0; i < 60; ++i) {
    numbers.push_back(rnd(1, 10000000));
  }
  for (u64 n : numbers) {
    vector<u64> f;
    factor64(n, f);
    u128 product = 1;
    for (u64 p : f) {
      assert(is_prime(p)), product *= p;
    }
    assert(product == n);
  }
  for (int k = 0; k < 600; ++k) {
    int m = rnd(1, 40), n = rnd(1, 40), a = rnd(-100, 100), b = rnd(-100, 100), found = -1,
        l = lcm(m, n);
    for (int x = 0; x < l; ++x) {
      if ((x - a) % m == 0 && (x - b) % n == 0) {
        found = x;
        break;
      }
    }
    auto c = crt_merge(a, m, b, n);
    assert(c.has_value() == (found >= 0));
    if (c) {
      assert(c->first == found && c->second == l);
    }
  }
  Combinations<101> c(100);
  vector<long long> row = {1};
  for (int n = 0; n <= 100; ++n) {
    for (int k = 0; k <= n; ++k) {
      assert(c.C(n, k) == row[k]);
    }
    vector<long long> next(n + 2, 1);
    for (int k = 1; k <= n; ++k) {
      next[k] = (row[k - 1] + row[k]) % 101;
    }
    row = next;
  }
  for (int k = 0; k < 120; ++k) {
    vector<int> a(rnd(0, 45)), b(rnd(0, 45));
    for (int& x : a) {
      x = rnd(-1000, 1000);
    }
    for (int& x : b) {
      x = rnd(-1000, 1000);
    }
    auto actual = NTT::convolution(a, b);
    vector<int> expect(a.empty() || b.empty() ? 0 : a.size() + b.size() - 1);
    for (int i = 0; i < (int) a.size(); ++i) {
      for (int j = 0; j < (int) b.size(); ++j) {
        long long x = (expect[i + j] + 1LL * a[i] * b[j]) % NTT::mod;
        if (x < 0) {
          x += NTT::mod;
        }
        expect[i + j] = x;
      }
    }
    assert(actual == expect);
    vector<double> ans;
    assert(gauss({{1, 1, 2}, {2, 2, 4}}, ans) == 2);
    assert(gauss({{1, 1, 2}, {2, 2, 5}}, ans) == 0);
    int n = rnd(1, 8);
    vector<double> solution(n);
    for (auto& x : solution) {
      x = rnd(-10, 10);
    }
    vector<vector<double>> mat(n, vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        mat[i][j] = (i == j ? 100 : rnd(-3, 3)), mat[i][n] += mat[i][j] * solution[j];
      }
    }
    assert(gauss(mat, ans) == 1);
    for (int i = 0; i < n; ++i) {
      assert(abs(ans[i] - solution[i]) < 1e-7);
    }
  }
}

void geometry_dp_tests() {
  assert(segments_intersect({0, 0}, {2, 0}, {1, 0}, {3, 0}));
  assert(!segments_intersect({0, 0}, {1, 0}, {2, 0}, {3, 0}));
  assert(segments_intersect({0, 0}, {2, 2}, {0, 2}, {2, 0}));
  vector<Point> square = {{0, 0}, {3, 0}, {3, 3}, {0, 3}};
  assert(area2(square) == 18 && point_in_polygon(square, {1, 1}) == 1);
  assert(point_in_polygon(square, {0, 2}) == 0 && point_in_polygon(square, {4, 2}) == -1);
  auto hit = Geo::line_intersection({0, 0}, {2, 2}, {0, 2}, {2, 0});
  assert(hit && fabsl(hit->x - 1) < 1e-10);
  assert(!Geo::line_intersection({0, 0}, {1, 0}, {0, 1}, {1, 1}));
  assert(fabsl(Geo::distance_to_segment({1, 2}, {0, 0}, {2, 0}) - 2) < 1e-10);
  assert(!Geo::circle_intersections({0, 0}, 1, {0, 0}, 1));
  assert(Geo::circle_intersections({0, 0}, 1, {2, 0}, 1)->size() == 1);
  assert(Geo::circle_intersections({0, 0}, 1, {1, 0}, 1)->size() == 2);
  assert(Geo::circle_intersections({0, 0}, 1, {3, 0}, 1)->empty());
  assert(Geo::circle_intersections({0, 0}, 0, {0, 0}, 0)->size() == 1);
  for (int k = 0; k < 200; ++k) {
    vector<Point> p;
    for (int i = rnd(0, 30); i > 0; --i) {
      p.push_back({rnd(-10, 10), rnd(-10, 10)});
    }
    auto h = convex_hull(p);
    if (h.size() >= 3) {
      for (int i = 0; i < (int) h.size(); ++i) {
        assert(orient(h[i], h[(i + 1) % h.size()], h[(i + 2) % h.size()]) > 0);
      }
      for (auto q : p) {
        assert(point_in_polygon(h, q) >= 0);
      }
    } else if (h.size() == 2) {
      for (auto q : p) {
        assert(on_segment(h[0], h[1], q));
      }
    }
    vector<int> a(rnd(0, 35));
    for (int& x : a) {
      x = rnd(-20, 20);
    }
    vector<int> d(a.size(), 1);
    int best = 0;
    for (int i = 0; i < (int) a.size(); ++i) {
      for (int j = 0; j < i; ++j) {
        if (a[j] < a[i]) {
          d[i] = max(d[i], d[j] + 1);
        }
      }
      best = max(best, d[i]);
    }
    auto indices = lis(a);
    assert((int) indices.size() == best);
    for (int i = 1; i < (int) indices.size(); ++i) {
      assert(indices[i - 1] < indices[i] && a[indices[i - 1]] < a[indices[i]]);
    }
    vector<long long> xs;
    for (int i = 0; i < 20; ++i) {
      xs.push_back(rnd(-50, 50));
    }
    LiChao lc(xs);
    assert(lc.query(xs[0]) == LiChao::INF);
    vector<pair<long long, long long>> lines;
    for (int i = 0; i < 50; ++i) {
      long long a = rnd(-100, 100), b = rnd(-100, 100);
      lc.add(a, b);
      lines.push_back({a, b});
      for (auto x : xs) {
        __int128 expect = LiChao::INF;
        for (auto [aa, bb] : lines) {
          expect = min(expect, (__int128) aa * x + bb);
        }
        assert(lc.query(x) == expect);
      }
    }
    int n = 1 << rnd(0, 7);
    vector<long long> f(n);
    for (auto& x : f) {
      x = rnd(-50, 50);
    }
    auto z = f;
    subset_zeta(z);
    for (int mask = 0; mask < n; ++mask) {
      long long expected = 0;
      for (int sub = mask;; sub = (sub - 1) & mask) {
        expected += f[sub];
        if (!sub) {
          break;
        }
      }
      assert(z[mask] == expected);
    }
    subset_zeta(z, true);
    assert(z == f);
  }
  __int128 minimum = -((__int128) 1 << 126);
  minimum += minimum;
  assert(to_string128(minimum) == "-170141183460469231731687303715884105728");
  assert(to_string128(0) == "0" && to_string128(-123) == "-123");
}

void boundary_tests() {
  {
    LazySegtree s(8);
    PersistentSegtree persistent(8);
    int root = 0;
    const long long B = 4000000000000000000LL;
    vector<long long> a = {-2 * B, B, B, B, -B, -B, -B, 2 * B};
    for (int p : {0, 1, 7, 4, 2, 5, 3, 6}) {
      s.add(p, p + 1, a[p]);
      root = persistent.add(root, p, a[p]);
    }
    assert(s.query(1, 7) == 0);
    assert(persistent.query(root, 1, 7) == 0);
    assert(persistent.query(root, 0, 8) == 0);
    assert(persistent.query(0, 1, 7) == 0);
  }

  {
    LazySegtree s(2);
    s.add(0, 1, -4000000000000000000LL);
    s.add(1, 2, -4000000000000000000LL);
    s.add(0, 2, 8000000000000000000LL);
    assert(s.query(0, 2) == 8000000000000000000LL);
    assert(s.query(0, 1) == 4000000000000000000LL);
    assert(s.query(1, 2) == 4000000000000000000LL);
    s.add(0, 2, -8000000000000000000LL);
    assert(s.query(0, 2) == -8000000000000000000LL);
    assert(s.query(0, 1) == -4000000000000000000LL);
  }

  auto same_modulus = crt_merge(LLONG_MIN, LLONG_MAX, LLONG_MAX - 1, LLONG_MAX);
  assert(same_modulus && same_modulus->first == LLONG_MAX - 1);
  assert(same_modulus->second == LLONG_MAX);
  auto large_modulus = crt_merge(-1, 3000000000LL, -1, 2999999999LL);
  assert(large_modulus && large_modulus->second == 8999999997000000000LL);
  assert(large_modulus->first == large_modulus->second - 1);
  assert(!crt_merge(0, 2, 1, 2));

  for (int n = 1; n <= 4096; n *= 2) {
    vector<int> values(n);
    for (int& x : values) {
      x = rnd(0, NTT::mod - 1);
    }
    auto original = values;
    NTT::transform(values, false);
    NTT::transform(values, true);
    assert(values == original);
  }
  assert(NTT::convolution({NTT::mod - 1}, {NTT::mod - 1}) == vector<int>{1});

  const long long X = 1000000000000000000LL;
  LiChao lines({-X, 0, X, X});
  lines.add(X, X);
  lines.add(-X, -X);
  lines.add(X, -X);
  assert(lines.query(X) == -(__int128) X * X - X);
  assert(lines.query(-X) == -(__int128) X * X - X);
  assert(lines.query(0LL) == -X);

  const long long C = 1000000000LL;
  vector<Point> square = {{-C, -C}, {C, -C}, {C, C}, {-C, C}};
  assert(area2(square) == (__int128) 8 * C * C);
  assert(point_in_polygon(square, {0, 0}) == 1);
  assert(point_in_polygon(square, {C, 0}) == 0);
  assert(segments_intersect({-C, -C}, {C, C}, {-C, C}, {C, -C}));
  assert(convex_hull({}).empty());
  assert((convex_hull({{1, 1}, {1, 1}}) == vector<Point>{{1, 1}}));
  assert((convex_hull({{0, 0}, {1, 1}, {2, 2}}) == vector<Point>{{0, 0}, {2, 2}}));

  assert(SCC({}).count == 0);
  assert(toposort({}).empty());
  assert(euler_trail(1, {}, 0) == optional<vector<int>>(vector<int>{}));
  assert(TwoSAT(0).solve());
  HopcroftKarp empty_left(0, 3), empty_right(3, 0);
  assert(empty_left.run() == 0 && empty_right.run() == 0);

  auto check_bytes = [](const string& text) {
    SuffixArray s(text);
    vector<int> expected(text.size());
    iota(expected.begin(), expected.end(), 0);
    sort(expected.begin(), expected.end(), [&](int a, int b) {
      return text.substr(a) < text.substr(b);
    });
    assert(s.sa == expected);
  };
  string bytes;
  for (int i = 0; i < 256; i++) {
    bytes += (char) i;
  }
  check_bytes(bytes);
  reverse(bytes.begin(), bytes.end());
  check_bytes(bytes);
}

int main(int argc, char** argv) {
  unsigned int seed = argc > 1 ? (unsigned int) stoul(argv[1]) : 20261106;
  gen.seed(seed);
  data_tests();
  graph_tests();
  tree_tests();
  flow_matching_tests();
  connectivity_tests();
  string_tests();
  numeric_tests();
  geometry_dp_tests();
  boundary_tests();
  cout << "Seed " << seed << ": all randomized comparisons and boundary cases passed.\n";
}
