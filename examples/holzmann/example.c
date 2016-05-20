int main() {
   int i, j, k, l;
   {
      {
	 i = 1;
	 j = 2;
	 k = 3;
      }
      {
	 ++i;
      }
   }
   ++k;
}
