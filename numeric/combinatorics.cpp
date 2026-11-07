// Prime modulus; 0<=n<MOD. Precompute in O(n+log MOD).
template <int MOD>
struct Combinations {
  vector<long long> f, invf;

  long long power(long long a, long long b) const {
    long long r = 1;
    while (b > 0) {
      if (b & 1) {
        r = r * a % MOD;
      }
      a = a * a % MOD;
      b >>= 1;
    }
    return r;
  }

  Combinations(int n) : f(n + 1, 1), invf(n + 1, 1) {
    assert(0 <= n && n < MOD);
    for (int i = 1; i <= n; i++) {
      f[i] = f[i - 1] * i % MOD;
    }
    invf[n] = power(f[n], MOD - 2);
    for (int i = n; i > 0; i--) {
      invf[i - 1] = invf[i] * i % MOD;
    }
  }

  long long C(int n, int k) const {
    if (k < 0 || k > n) {
      return 0;
    }
    assert(n < (int) f.size());
    return f[n] * invf[k] % MOD * invf[n - k] % MOD;
  }
};
