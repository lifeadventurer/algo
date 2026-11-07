// Requires miller_rabin.cpp. Expected time, randomized.
u64 pollard_rho(u64 n) {
  if (n % 2 == 0) {
    return 2;
  }
  static mt19937_64 gen(712367821);
  for (;;) {
    u64 c = gen() % (n - 1) + 1;
    u64 x = gen() % (n - 2) + 2;
    u64 y = x;
    u64 d = 1;
    auto f = [&](u64 v) -> u64 {
      return ((u128) mul_mod(v, v, n) + c) % n;
    };
    while (d == 1) {
      x = f(x);
      y = f(f(y));
      d = gcd(x > y ? x - y : y - x, n);
    }
    if (d != n) {
      return d;
    }
  }
}

void factor64(u64 n, vector<u64>& out) {
  assert(n >= 1);
  if (n == 1) {
    return;
  }
  if (is_prime(n)) {
    out.push_back(n);
    return;
  }
  u64 d = pollard_rho(n);
  factor64(d, out);
  factor64(n / d, out);
}
