// General CRT, positive moduli; merged modulus must fit int64.
// Returns nullopt for inconsistent congruences.
using i128 = __int128_t;

i128 crt_gcd(i128 a, i128 b, i128& x, i128& y) {
  if (!b) {
    x = 1;
    y = 0;
    return a;
  }
  i128 g = crt_gcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}

optional<pair<long long, long long>> crt_merge(long long a, long long m, long long b, long long n) {
  assert(m > 0 && n > 0);
  a %= m;
  if (a < 0) {
    a += m;
  }
  b %= n;
  if (b < 0) {
    b += n;
  }
  i128 x, y;
  i128 g = crt_gcd(m, n, x, y);
  i128 delta = (i128) b - a;
  if (delta % g) {
    return nullopt;
  }
  i128 mod = n / g;
  i128 k = (delta / g * x) % mod;
  if (k < 0) {
    k += mod;
  }
  i128 lcm = (i128) m * mod;
  assert(lcm <= LLONG_MAX);
  return pair<long long, long long>{(a + (i128) m * k) % lcm, lcm};
}
