// Augmented n x (m+1) matrix; approximate real arithmetic.
// Returns 0=no solution, 1=unique, 2=infinitely many.
int gauss(vector<vector<double>> a, vector<double>& ans) {
  const double EPS = 1e-9;
  int n = (int) a.size();
  assert(n > 0);
  int m = (int) a[0].size() - 1;
  vector<int> where(m, -1);
  for (int col = 0, row = 0; col < m && row < n; col++) {
    int sel = row;
    for (int i = row; i < n; i++) {
      if (abs(a[i][col]) > abs(a[sel][col])) {
        sel = i;
      }
    }
    if (abs(a[sel][col]) < EPS) {
      continue;
    }
    swap(a[sel], a[row]);
    where[col] = row;
    double div = a[row][col];
    for (int j = col; j <= m; j++) {
      a[row][j] /= div;
    }
    for (int i = 0; i < n; i++) {
      if (i != row) {
        double c = a[i][col];
        for (int j = col; j <= m; j++) {
          a[i][j] -= c * a[row][j];
        }
      }
    }
    row++;
  }
  ans.assign(m, 0);
  for (int j = 0; j < m; j++) {
    if (where[j] != -1) {
      ans[j] = a[where[j]][m];
    }
  }
  for (int i = 0; i < n; i++) {
    double s = 0;
    for (int j = 0; j < m; j++) {
      s += a[i][j] * ans[j];
    }
    if (abs(s - a[i][m]) > EPS) {
      return 0;
    }
  }
  return find(where.begin(), where.end(), -1) == where.end() ? 1 : 2;
}
