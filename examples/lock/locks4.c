int f() {
   if (rand() % 2 == 0) {
      unlock();
   } else {
      f();
      lock();
   }
}

int main() {
   lock();
   f();
}
