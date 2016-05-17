int h() {
   if (rand() % 2 == 0) {
      h();
   }
}

int g() {
   if (rand() % 2 == 0) {
      g();
   } else {
      lock();
   }
}

int f() {
   if (rand() % 2 == 0) {
      unlock();
      g();
   } else {
      f();
   }
}

int main() {
   lock();
   if (rand() % 2 == 0) {
      f();
   }
}
