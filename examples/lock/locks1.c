int f() {
   if (rand() % 2 == 0) {
      unlock();
   } else {
      f();
   }
}

int main() {
   lock();
   f();
}
