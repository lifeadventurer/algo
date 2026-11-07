// a[mask] <- sum of original a[sub] for sub subset of mask.
// O(b*2^b). Apply inverse=true to undo (Mobius inversion).
template <typename T>
void subset_zeta(vector<T>& a, bool inverse = false) {
  int n = (int) a.size();
  assert(n && !(n & (n - 1)));
  for (int bit = 1; bit < n; bit *= 2) {
    for (int mask = 0; mask < n; mask++) {
      if (mask & bit) {
        if (inverse) {
          a[mask] -= a[mask ^ bit];
        } else {
          a[mask] += a[mask ^ bit];
        }
      }
    }
  }
}

// Enumerate all submasks (including 0):
// for (int s = mask; ; s = (s - 1) & mask) {
//   // Process s here.
//   if (s == 0) {
//     break;
//   }
// }
