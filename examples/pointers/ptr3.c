int main() {
   int* ip = malloc(sizeof(int));
   if (ip) {
      *ip = 1;
      free(ip);
   }
   free(ip);
}
