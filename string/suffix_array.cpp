// Doubling + std::sort: O(n log^2 n), arbitrary byte strings.
struct SuffixArray {
  vector<int> sa, rank, lcp;  // lcp[i] = LCP(sa[i],sa[i+1])

  SuffixArray(const string& s) {
    int n = (int) s.size();
    sa.resize(n);
    rank.resize(n);
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; i++) {
      rank[i] = (unsigned char) s[i];
    }
    for (int k = 1; k < n; k *= 2) {
      auto key = [&](int i) {
        return pair<int, int>{rank[i], i + k < n ? rank[i + k] : -1};
      };
      sort(sa.begin(), sa.end(), [&](int a, int b) {
        return key(a) < key(b);
      });
      vector<int> nr(n);
      for (int i = 1; i < n; i++) {
        nr[sa[i]] = nr[sa[i - 1]] + (key(sa[i - 1]) != key(sa[i]));
      }
      rank = move(nr);
      if (rank[sa.back()] == n - 1) {
        break;
      }
    }
    // n==1 must also receive inverse rank 0.
    for (int i = 0; i < n; i++) {
      rank[sa[i]] = i;
    }
    lcp.resize(max(0, n - 1));
    for (int i = 0, h = 0; i < n; i++) {
      int r = rank[i];
      if (r == n - 1) {
        h = 0;
        continue;
      }
      int j = sa[r + 1];
      while (i + h < n && j + h < n && s[i + h] == s[j + h]) {
        h++;
      }
      lcp[r] = h;
      if (h) {
        h--;
      }
    }
  }
};
