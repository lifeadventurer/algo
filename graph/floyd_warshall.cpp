// Initialize diagonal to 0; parallel edges take min weight.
// Unreachable = INF. d[k][k]<0 detects a negative cycle.
// Finite shortest-path values must stay strictly between -INF and INF.
void floyd_warshall(vector<vector<long long>>& d) {
  const long long INF = 4'000'000'000'000'000'000LL;
  int n = (int) d.size();
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      if (d[i][k] != INF) {
        for (int j = 0; j < n; j++) {
          if (d[k][j] != INF) {
            __int128 x = (__int128) d[i][k] + d[k][j];
            d[i][j] = min(d[i][j], (long long) max(-(__int128) INF, x));
          }
        }
      }
    }
  }
}
