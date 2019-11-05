#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
int guard(char* err, int x) { if (x == -1) { perror(err); exit(1); } return x; }
void put(uint8_t c) { guard("could not write byte", write(1, &c, 1)); }
int get(uint8_t* buf) { return guard("could not read byte", read(0, buf, 1)); }
void encode(void) {
  uint8_t c;
  if (get(&c) == 0) return;
  put(c);
  uint8_t n = 1;
  uint8_t new_c;
  while (get(&new_c) != 0) {
    if (new_c == c) {
      if (n == UINT8_MAX) { put(n); n = 1; put(c); }
      else { n++; }
    } else {
      put(n); put(new_c);
      c = new_c; n = 1;
    }
  }
  put(n);
}
void decode(void) {
  uint8_t c; uint8_t n;
  while (get(&c) != 0) {
    get(&n);
    for (int i = 0; i < n; i++) put(c);
  }
}
void usage(char* pname) { fprintf(stderr, "Usage: %s (encode|decode)\n", pname); exit(1); }
int main(int argc, char* argv[]) {
  if (argc < 2) usage(argv[0]);
  else if (strcmp(argv[1], "encode") == 0) encode();
  else if (strcmp(argv[1], "decode") == 0) decode();
  else usage(argv[0]);
  return 0;
}

