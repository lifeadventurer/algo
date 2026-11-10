// Minimum-cost assignment of every row to a distinct column, O(n^2*m).
// Negative costs are allowed. The final cost must fit long long.
// Returns nullopt when rows > columns; matching[row] is 0-based.
optional<pair<long long, vector<int>>> hungarian(const vector<vector<long long>>& cost) {
  int n = (int) cost.size();
  if (n == 0) {
    return make_pair(0LL, vector<int>{});
  }
  int m = (int) cost[0].size();
  for (const auto& row : cost) {
    assert((int) row.size() == m);
  }
  if (n > m) {
    return nullopt;
  }
  const __int128 INF = (__int128) 1 << 126;
  vector<__int128> u(n + 1), v(m + 1);
  vector<int> p(m + 1), prev(m + 1);
  for (int row = 1; row <= n; row++) {
    p[0] = row;
    int col = 0;
    vector<__int128> slack(m + 1, INF);
    vector<bool> used(m + 1);
    do {
      used[col] = true;
      int i = p[col], next = 0;
      __int128 delta = INF;
      for (int j = 1; j <= m; j++) {
        if (!used[j]) {
          __int128 reduced = (__int128) cost[i - 1][j - 1] - u[i] - v[j];
          if (reduced < slack[j]) {
            slack[j] = reduced;
            prev[j] = col;
          }
          if (slack[j] < delta) {
            delta = slack[j];
            next = j;
          }
        }
      }
      for (int j = 0; j <= m; j++) {
        if (used[j]) {
          u[p[j]] += delta;
          v[j] -= delta;
        } else {
          slack[j] -= delta;
        }
      }
      col = next;
    } while (p[col] != 0);
    do {
      int next = prev[col];
      p[col] = p[next];
      col = next;
    } while (col != 0);
  }
  vector<int> matching(n);
  for (int j = 1; j <= m; j++) {
    if (p[j] != 0) {
      matching[p[j] - 1] = j - 1;
    }
  }
  __int128 total = 0;
  for (int i = 0; i < n; i++) {
    total += cost[i][matching[i]];
  }
  assert(LLONG_MIN <= total && total <= LLONG_MAX);
  return make_pair((long long) total, matching);
}
