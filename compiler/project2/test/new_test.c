int main() {
  int x, y;
  x = 1;
  y = x * (2 + 3) - 4;
  if (y > 2 && y < 10) {
    x++;
  } else {
    --x;
  }
  return x;
}
