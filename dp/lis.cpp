// Indices of a strictly increasing subsequence, O(n log n).
// Change lower_bound to upper_bound for nondecreasing.
template <typename T>
vector<int> lis(const vector<T>& a) {
  vector<T> tail;
  vector<int> at, prev(a.size(), -1);
  for (int i = 0; i < (int) a.size(); i++) {
    int p = (int) (lower_bound(tail.begin(), tail.end(), a[i]) - tail.begin());
    if (p) {
      prev[i] = at[p - 1];
    }
    if (p == (int) tail.size()) {
      tail.push_back(a[i]);
      at.push_back(i);
    } else {
      tail[p] = a[i];
      at[p] = i;
    }
  }
  vector<int> ans;
  for (int v = at.empty() ? -1 : at.back(); v != -1; v = prev[v]) {
    ans.push_back(v);
  }
  reverse(ans.begin(), ans.end());
  return ans;
}
