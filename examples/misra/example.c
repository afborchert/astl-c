int main() {
   int i = rand();
   int j;

   /* example failing Misra rule 60 */
   if (i < 10) {
      j = 1;
   } else if (j < 20) {
      j = 2;
   }

   /* example conforming to Misra rule 60 */
   if (i < 10) {
      j = 1;
   } else if (j < 20) {
      j = 2;
   } else {
      j = 3;
   }
}
