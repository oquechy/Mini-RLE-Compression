#include "stdint.h"
#include "stddef.h"

int rle_encode(uint8_t* src, size_t src_len, uint8_t* dst, size_t dst_len, size_t* bytes_written);
int rle_decode(uint8_t* src, size_t src_len, uint8_t* dst, size_t dst_len, size_t* bytes_written);
