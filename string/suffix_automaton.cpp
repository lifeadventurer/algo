// Lowercase a-z. At most 2*n states; transitions use O(26*n).
struct SuffixAutomaton {
  struct State {
    array<int, 26> next;
    int len = 0, link = -1;

    State() {
      next.fill(-1);
    }
  };

  vector<State> t = {State{}};
  int last = 0;

  void extend(char ch) {
    int c = ch - 'a';
    assert(0 <= c && c < 26);
    int cur = (int) t.size();
    t.emplace_back();
    t[cur].len = t[last].len + 1;
    int p = last;
    while (p != -1 && t[p].next[c] < 0) {
      t[p].next[c] = cur;
      p = t[p].link;
    }
    if (p < 0) {
      t[cur].link = 0;
    } else {
      int q = t[p].next[c];
      if (t[p].len + 1 == t[q].len) {
        t[cur].link = q;
      } else {
        int clone = (int) t.size();
        t.push_back(t[q]);
        t[clone].len = t[p].len + 1;
        while (p != -1 && t[p].next[c] == q) {
          t[p].next[c] = clone;
          p = t[p].link;
        }
        t[q].link = t[cur].link = clone;
      }
    }
    last = cur;
  }

  long long distinct() const {
    long long ans = 0;
    for (int v = 1; v < (int) t.size(); v++) {
      ans += t[v].len - t[t[v].link].len;
    }
    return ans;
  }
};
