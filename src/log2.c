#include <log2.h>

unsigned int log2_lookup(uint64_t x) {
 uint64_t temp;
 if ((temp = (x >> 32))) {
    // TOP 32
    if ((x = (temp >> 16))) {
      // TOP 16
      if ((temp = (x >> 8))) {
        // TOP 8
        return 56 + lookup_table[temp];
      } else {
        // 48 - 56
        return 48 + lookup_table[x];
      }
    } else {
      // 32 - 48
      if ((x = (temp >> 8))) {
        // 40 - 48
        return 40 + lookup_table[x]; 
      } else{
        // 32 - 40
        return 32 + lookup_table[temp]; 
      }
    }
 } else {
    // BOTTOM 32
    if ((temp = (x >> 16))) {
      if ((x = (temp >> 8))) {
        // 24 - 32
        return 24 + lookup_table[x];
      } else {
        // 16 - 24
        return 16 + lookup_table[temp];
      }
    } else {
      // BOTTOM 16
      if ((temp = (x >> 8))) {
        // 8 - 16
        return 8 + lookup_table[temp];
      } else {
        // BOTTOM 8
        return lookup_table[x];
      }
    }
 }
}
