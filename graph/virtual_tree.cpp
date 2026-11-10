// Requires hld.cpp. Build the LCA closure of marked vertices in a tree.
// vertex[local] is the original ID; g has directed parent->child edges
// weighted by original edge counts. Local root is 0; empty input is allowed.
// O(k log k + k log n) time; at most 2*k-1 vertices after deduplication.
struct VirtualTree {
  vector<int> vertex, parent;
  vector<vector<pair<int, int>>> g;

  VirtualTree(const HLD& tree, vector<int> marked) {
    auto by_pos = [&](int a, int b) {
      return tree.pos[a] < tree.pos[b];
    };
    auto lca = [&](int a, int b) {
      while (tree.head[a] != tree.head[b]) {
        if (tree.depth[tree.head[a]] < tree.depth[tree.head[b]]) {
          swap(a, b);
        }
        a = tree.parent[tree.head[a]];
      }
      return tree.depth[a] < tree.depth[b] ? a : b;
    };
    for (int v : marked) {
      assert(0 <= v && v < (int) tree.pos.size());
    }
    sort(marked.begin(), marked.end(), by_pos);
    marked.erase(unique(marked.begin(), marked.end()), marked.end());
    vertex = marked;
    for (int i = 1; i < (int) marked.size(); i++) {
      vertex.push_back(lca(marked[i - 1], marked[i]));
    }
    sort(vertex.begin(), vertex.end(), by_pos);
    vertex.erase(unique(vertex.begin(), vertex.end()), vertex.end());
    int n = (int) vertex.size();
    parent.assign(n, -1);
    g.resize(n);
    vector<int> stack;
    for (int i = 0; i < n; i++) {
      int v = vertex[i];
      while (!stack.empty()) {
        int u = vertex[stack.back()];
        if (tree.pos[u] <= tree.pos[v] && tree.pos[v] < tree.pos[u] + tree.size[u]) {
          break;
        }
        stack.pop_back();
      }
      if (!stack.empty()) {
        parent[i] = stack.back();
        int distance = tree.depth[v] - tree.depth[vertex[parent[i]]];
        g[parent[i]].push_back({i, distance});
      }
      stack.push_back(i);
    }
  }
};
