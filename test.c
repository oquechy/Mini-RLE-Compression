#include "string.h"
#include "stdio.h"
#include "minirle.h"
#include "stdlib.h"


#define CHECK(cond) do { if (!(cond)) return -1; } while (0)

int test_hello() {
  uint8_t enc[20];
  uint8_t dec[20];
  size_t written;
  (rle_encode((uint8_t *)"hello", 5, enc, sizeof(enc), &written) == 0);
  CHECK(written == 8);
  uint8_t expected[8] = {1, 'h', 1, 'e', 2, 'l', 1, 'o' };
  CHECK(memcmp(enc, expected, 8) == 0);
  CHECK(rle_decode(enc, 8, dec, sizeof(dec), &written) == 0);
  CHECK(written == 5);
  CHECK(memcmp(dec, "hello", 5) == 0);
  return 0;
};

int test_empty_buffer() {
  uint8_t enc[20];
  size_t written;
  CHECK(rle_encode((uint8_t *)0, 0, enc, sizeof(enc), &written) == 0);
  CHECK(written == 0);
  return 0;
}

int test_short_buffer() {
  uint8_t enc[20];
  uint8_t dec[20];
  size_t written;
  CHECK(rle_encode((uint8_t *)"hello", 5, enc, 8, &written) == 0);
  CHECK(written == 8);
  CHECK(rle_encode((uint8_t *)"hello", 5, enc, 7, &written) == -1);

  CHECK(rle_decode(enc, 8, dec, 5, &written) == 0);
  CHECK(written == 5);
  CHECK(rle_decode(enc, 8, dec, 4, &written) == -1);
  return 0;
}

int test_random() {
  uint8_t src[600000];
  uint8_t enc[600000];
  uint8_t dec[600000];
  size_t written;
  
  int cur = 0, n = 0;
  for (; cur + n < 600000; ++cur) {
    cur += n;
    n = rand() % 400 + 50;
    for (int i = 0; cur + i < 600000 && i < n; ++i)
      src[cur + i] = n % 256;            
  } 
  
  CHECK(rle_encode((uint8_t *)src, cur, enc, 600000, &written) == 0);
  
  CHECK(rle_decode(enc, written, dec, 600000, &written) == 0);
  CHECK(written == cur);
  for (int i = 0; i < cur; ++i) {
    CHECK(src[i] == dec[i]);  
   // printf("%d ", dec[i]);
  }
  return 0;
}

#define TEST(test) do { if (test() != 0) { exit_code = -1; printf(#test " FAIL\n"); } else { printf(#test " OK\n"); } } while(0)

int main() {
  int exit_code = 0;
  TEST(test_hello);
  TEST(test_empty_buffer);
  TEST(test_short_buffer);
  TEST(test_random);
  return exit_code;
}
