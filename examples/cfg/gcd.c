int main() {
   int a = 42; int b = 54;
   while (a != b) {
      if (a > b) {
	 a -= b;
      } else {
	 b -= a;
      }
   }
}
