// Linear sieve on [0, N). Marks 0, 1, and composites in nonprime.
// Returns primes in increasing order. Uses N bits plus the prime list.
template <size_t N>
vector<int> bitset_sieve(bitset<N>& nonprime) {
  static_assert(0 < N && N <= INT_MAX);
  nonprime.reset();
  nonprime[0] = true;
  if (N > 1) {
    nonprime[1] = true;
  }
  vector<int> primes;
  for (int i = 2; i < (int) N; i++) {
    if (!nonprime[i]) {
      primes.push_back(i);
    }
    for (int p : primes) {
      long long x = 1LL * i * p;
      if (x >= (int) N) {
        break;
      }
      nonprime[x] = true;
      if (i % p == 0) {
        break;
      }
    }
  }
  return primes;
}
