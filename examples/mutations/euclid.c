unsigned int ggt(unsigned int x, unsigned int y) {
   while (x != y) {
      if (x > y) {
         x = x - y;
      } else {
         y = y - x;
      }
   }
}
