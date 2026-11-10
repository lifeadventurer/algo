// GF(2) system with m variables, m<=MAXV; RHS is bit m of each row.
// Returns rank, or -1 if inconsistent. Free variables are set to 0.
// Unique iff rank==m. Time O(rows*m*ceil((MAXV+1)/word_bits)).
template <size_t MAXV>
int gauss_binary(vector<bitset<MAXV + 1>> a, int m, bitset<MAXV>& answer) {
  assert(0 <= m && (size_t) m <= MAXV);
  int n = (int) a.size(), rank = 0;
  vector<int> where(m, -1);
  answer.reset();
  for (int col = 0; col < m && rank < n; col++) {
    int pivot = rank;
    while (pivot < n && !a[pivot][col]) {
      pivot++;
    }
    if (pivot == n) {
      continue;
    }
    swap(a[pivot], a[rank]);
    where[col] = rank;
    for (int i = 0; i < n; i++) {
      if (i != rank && a[i][col]) {
        a[i] ^= a[rank];
      }
    }
    rank++;
  }
  for (int i = rank; i < n; i++) {
    if (a[i][m]) {
      return -1;
    }
  }
  for (int col = 0; col < m; col++) {
    if (where[col] >= 0) {
      answer[col] = a[where[col]][m];
    }
  }
  return rank;
}
