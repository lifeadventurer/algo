// Prime MOD. Finds the shortest recurrence consistent with the sample.
// Output c means a[n] = c[0]*a[n-1] + ... + c[d-1]*a[n-d].
// For a known order bound d, provide at least 2*d terms. O(sample^2).
template <int MOD>
vector<int> berlekamp_massey(vector<int> a) {
  static_assert(MOD >= 2);
  for (int& x : a) {
    x %= MOD;
    if (x < 0) {
      x += MOD;
    }
  }
  auto power = [](long long x, int e) {
    long long ans = 1;
    while (e > 0) {
      if (e & 1) {
        ans = ans * x % MOD;
      }
      x = x * x % MOD;
      e >>= 1;
    }
    return ans;
  };
  vector<int> c = {1}, previous = {1};
  int length = 0, shift = 1, last = 1;
  for (int n = 0; n < (int) a.size(); n++) {
    long long discrepancy = a[n];
    for (int i = 1; i <= length; i++) {
      discrepancy = (discrepancy + (long long) c[i] * a[n - i]) % MOD;
    }
    if (discrepancy == 0) {
      shift++;
      continue;
    }
    auto saved = c;
    long long factor = discrepancy * power(last, MOD - 2) % MOD;
    c.resize(max(c.size(), previous.size() + shift));
    for (int i = 0; i < (int) previous.size(); i++) {
      long long value = (c[i + shift] - factor * previous[i]) % MOD;
      c[i + shift] = (int) (value < 0 ? value + MOD : value);
    }
    if (2LL * length <= n) {
      length = n + 1 - length;
      previous = move(saved);
      last = (int) discrepancy;
      shift = 1;
    } else {
      shift++;
    }
  }
  vector<int> answer(length);
  for (int i = 0; i < length; i++) {
    answer[i] = c[i + 1] == 0 ? 0 : MOD - c[i + 1];
  }
  return answer;
}
