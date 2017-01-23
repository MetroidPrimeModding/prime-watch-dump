#include <cstddef>
#include "GameMemory.h"

using namespace std;

extern char *memory;
extern size_t memory_size;

namespace GameMemory {
    template<typename T>
    T swap_endian(T u) {
      union {
          T u;
          unsigned char u8[sizeof(T)];
      } source, dest;

      source.u = u;

      for (size_t k = 0; k < sizeof(T); k++) {
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
      }

      return dest.u;
    }

    uint32_t read_u32(uint32_t address) {
      uint32_t addr = address & (~0x80000000);
      if (addr >= memory_size) {
        return 0;
      }
      char *ptr = memory + addr;
      uint32_t res = *(reinterpret_cast<uint32_t *>(ptr));
      return swap_endian(res);
    }

    uint64_t read_u64(uint32_t address) {
      uint32_t addr = address & (~0x80000000);
      if (addr >= memory_size) {
        return 0;
      }
      char *ptr = memory + addr;
      uint64_t res = *(reinterpret_cast<uint64_t *>(ptr));
      return swap_endian(res);
    }

    float read_float(uint32_t address) {
      union {
          uint32_t i;
          float f;
      } fi;
      fi.i = read_u32(address);
      return fi.f;
    }

    double read_double(uint32_t address) {
      union {
          uint64_t i;
          double d;
      } di;
      di.i = read_u64(address);
      return di.d;
    }
}