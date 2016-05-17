int h() {
   if (rand() % 2 == 0) {
      h();
   }
}

int g() {
   if (rand() % 2 == 0) {
      h();
   } else {
      lock();
      f();
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
   f();
}
