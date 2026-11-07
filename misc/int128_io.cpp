// Handles the minimum signed __int128 without negation overflow.
string to_string128(__int128 x) {
  bool neg = x < 0;
  __uint128_t v = neg ? -(__uint128_t) x : (__uint128_t) x;
  string s;
  do {
    s += char('0' + v % 10);
    v /= 10;
  } while (v);
  if (neg) {
    s += '-';
  }
  reverse(s.begin(), s.end());
  return s;
}
