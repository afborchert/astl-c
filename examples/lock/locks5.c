int f() {
   if (rand() % 2 == 0) {
      if (locked()) {
	 unlock();
      }
   } else {
      f();
   }
}

int main() {
   lock();
   f();
}
