extern int f(int);
int main() {
   for (int i, j = 0, k; i = f(j); j += i) {
      // ...
   }
}
