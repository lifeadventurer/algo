// a[n] = c[0]*a[n-1] + ... + c[d-1]*a[n-d], modulo MOD>=2.
// initial contains exactly a[0..d-1]. Empty recurrence means all zeros.
// O(d^2 log(k+1)) time, O(d) space. k is 0-based.
template <int MOD>
int linear_recurrence(vector<int> initial, vector<int> c, uint64_t k) {
  static_assert(MOD >= 2);
  int d = (int) c.size();
  assert((int) initial.size() == d);
  auto normalize = [](int x) {
    x %= MOD;
    return x < 0 ? x + MOD : x;
  };
  for (int& x : initial) {
    x = normalize(x);
  }
  for (int& x : c) {
    x = normalize(x);
  }
  if (d == 0) {
    return 0;
  }
  auto multiply = [&](const vector<int>& a, const vector<int>& b) {
    vector<long long> t(2 * d - 1);
    for (int i = 0; i < d; i++) {
      for (int j = 0; j < d; j++) {
        t[i + j] = (t[i + j] + (long long) a[i] * b[j]) % MOD;
      }
    }
    for (int i = 2 * d - 2; i >= d; i--) {
      for (int j = 1; j <= d; j++) {
        t[i - j] = (t[i - j] + t[i] * c[j - 1]) % MOD;
      }
    }
    return vector<int>(t.begin(), t.begin() + d);
  };
  vector<int> answer(d), base(d);
  answer[0] = 1;
  if (d == 1) {
    base[0] = c[0];
  } else {
    base[1] = 1;
  }
  while (k > 0) {
    if (k & 1) {
      answer = multiply(answer, base);
    }
    base = multiply(base, base);
    k >>= 1;
  }
  long long value = 0;
  for (int i = 0; i < d; i++) {
    value = (value + (long long) answer[i] * initial[i]) % MOD;
  }
  return (int) value;
}
