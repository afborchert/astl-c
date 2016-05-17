int f() {
   if (!locked()) {
      lock();
   }
   if (rand() % 2 == 0) {
      if (locked()) {
	 unlock();
      }
   } else {
      f();
      if (!locked()) {
	 lock();
      }
   }
}

int main() {
   lock();
   f();
   if (locked()) {
      unlock();
      unlock();
   }
}
