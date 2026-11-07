// Mod 998244353, primitive root 3, length <=2^23.
// transform() expects normalized coefficients; convolution() normalizes them.
namespace NTT {
const int mod = 998244353;

int power(int a, int b) {
  int r = 1;
  while (b > 0) {
    if (b & 1) {
      r = (long long) r * a % mod;
    }
    a = (long long) a * a % mod;
    b >>= 1;
  }
  return r;
}

void transform(vector<int>& a, bool invert) {
  int n = (int) a.size();
  assert(n && !(n & (n - 1)) && n <= (1 << 23));
  for (int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) {
      j ^= bit;
    }
    j ^= bit;
    if (i < j) {
      swap(a[i], a[j]);
    }
  }
  for (int len = 2; len <= n; len *= 2) {
    int root = power(3, (mod - 1) / len);
    if (invert) {
      root = power(root, mod - 2);
    }
    for (int i = 0; i < n; i += len) {
      long long w = 1;
      for (int j = 0; j < len / 2; j++) {
        int u = a[i + j], v = w * a[i + j + len / 2] % mod;
        a[i + j] = u + v;
        if (a[i + j] >= mod) {
          a[i + j] -= mod;
        }
        a[i + j + len / 2] = u - v;
        if (a[i + j + len / 2] < 0) {
          a[i + j + len / 2] += mod;
        }
        w = w * root % mod;
      }
    }
  }
  if (invert) {
    int inv = power(n, mod - 2);
    for (int& x : a) {
      x = (long long) x * inv % mod;
    }
  }
}

vector<int> convolution(vector<int> a, vector<int> b) {
  if (a.empty() || b.empty()) {
    return {};
  }
  int need = (int) a.size() + (int) b.size() - 1;
  int n = 1;
  while (n < need) {
    n *= 2;
  }
  assert(n <= (1 << 23));
  for (int& x : a) {
    x %= mod;
    if (x < 0) {
      x += mod;
    }
  }
  for (int& x : b) {
    x %= mod;
    if (x < 0) {
      x += mod;
    }
  }
  a.resize(n);
  b.resize(n);
  transform(a, false);
  transform(b, false);
  for (int i = 0; i < n; i++) {
    a[i] = (long long) a[i] * b[i] % mod;
  }
  transform(a, true);
  a.resize(need);
  return a;
}
}  // namespace NTT
