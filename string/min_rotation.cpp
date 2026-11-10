// Index of a lexicographically smallest rotation, O(n) time, O(1) space.
// Bytes compare as unsigned; empty input returns 0.
int min_rotation(const string& s) {
  int n = (int) s.size();
  if (n == 0) {
    return 0;
  }
  int i = 0, j = 1, length = 0;
  while (i < n && j < n && length < n) {
    unsigned char a = s[(i + length) % n];
    unsigned char b = s[(j + length) % n];
    if (a == b) {
      length++;
      continue;
    }
    if (a > b) {
      i += length + 1;
      if (i == j) {
        i++;
      }
    } else {
      j += length + 1;
      if (i == j) {
        j++;
      }
    }
    length = 0;
  }
  return min(i, j);
}
