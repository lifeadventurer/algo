// Lowercase a-z, nonempty patterns. Add all, then build once.
struct AhoCorasick {
  struct Node {
    array<int, 26> next;
    int link = 0;
    long long out = 0;

    Node() {
      next.fill(-1);
    }
  };

  vector<Node> t = {Node{}};

  int add(const string& s) {
    assert(!s.empty());
    int v = 0;
    for (char ch : s) {
      int c = ch - 'a';
      assert(0 <= c && c < 26);
      if (t[v].next[c] < 0) {
        t[v].next[c] = (int) t.size();
        t.emplace_back();
      }
      v = t[v].next[c];
    }
    t[v].out++;
    return v;
  }

  void build() {
    queue<int> q;
    for (int c = 0; c < 26; c++) {
      int u = t[0].next[c];
      if (u < 0) {
        t[0].next[c] = 0;
      } else {
        q.push(u);
      }
    }
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      int p = t[v].link;
      t[v].out += t[p].out;
      for (int c = 0; c < 26; c++) {
        int u = t[v].next[c];
        if (u < 0) {
          t[v].next[c] = t[p].next[c];
        } else {
          t[u].link = t[p].next[c];
          q.push(u);
        }
      }
    }
  }

  long long count(const string& text) const {
    int v = 0;
    long long ans = 0;
    for (char ch : text) {
      int c = ch - 'a';
      assert(0 <= c && c < 26);
      v = t[v].next[c];
      ans += t[v].out;
    }
    return ans;  // all occurrences, overlaps and duplicates
  }
};
