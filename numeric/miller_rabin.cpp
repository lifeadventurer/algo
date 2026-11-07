// Deterministic for all unsigned 64-bit integers.
using u64 = uint64_t;
using u128 = __uint128_t;

u64 mul_mod(u64 a, u64 b, u64 m) {
  return (u128) a * b % m;
}

u64 pow_mod(u64 a, u64 e, u64 m) {
  u64 r = 1 % m;
  while (e > 0) {
    if (e & 1) {
      r = mul_mod(r, a, m);
    }
    a = mul_mod(a, a, m);
    e >>= 1;
  }
  return r;
}

bool is_prime(u64 n) {
  if (n < 2) {
    return false;
  }
  for (u64 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
    if (n % p == 0) {
      return n == p;
    }
  }
  u64 d = n - 1;
  int s = 0;
  while (!(d & 1)) {
    d >>= 1;
    s++;
  }
  for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
    if (a % n == 0) {
      continue;
    }
    u64 x = pow_mod(a % n, d, n);
    if (x == 1 || x == n - 1) {
      continue;
    }
    bool ok = false;
    for (int r = 1; r < s; r++) {
      x = mul_mod(x, x, n);
      if (x == n - 1) {
        ok = true;
        break;
      }
    }
    if (!ok) {
      return false;
    }
  }
  return true;
}
