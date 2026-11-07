struct Sieve {
  vector<int> primes, spf, phi, mu;

  Sieve(int n) : spf(n + 1), phi(n + 1), mu(n + 1) {
    if (n >= 1) {
      phi[1] = mu[1] = 1;
    }
    for (int i = 2; i <= n; i++) {
      if (!spf[i]) {
        spf[i] = i;
        primes.push_back(i);
        phi[i] = i - 1;
        mu[i] = -1;
      }
      for (int p : primes) {
        if (1LL * i * p > n) {
          break;
        }
        spf[i * p] = p;
        if (i % p == 0) {
          phi[i * p] = phi[i] * p;
          break;
        }
        phi[i * p] = phi[i] * (p - 1);
        mu[i * p] = -mu[i];
      }
    }
  }

  vector<pair<int, int>> factor(int x) const {
    assert(1 <= x && x < (int) spf.size());
    vector<pair<int, int>> f;
    while (x > 1) {
      int p = spf[x], e = 0;
      do {
        x /= p;
        e++;
      } while (x % p == 0);
      f.push_back({p, e});
    }
    return f;
  }
};
