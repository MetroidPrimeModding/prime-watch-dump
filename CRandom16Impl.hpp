#ifndef PRIME_WATCH_DUMP_CRANDOM16IMPL_HPP
#define PRIME_WATCH_DUMP_CRANDOM16IMPL_HPP

using u32 = uint32_t;
using s32 = int32_t;

class CRandom16Impl
{
    u32 m_seed;
public:
    CRandom16Impl() = default;
    CRandom16Impl(u32 p) : m_seed(p) {}

    inline u32 Next()
    {
      m_seed = (m_seed * 0x41c64e6d) + 0x00003039;
      return m_seed >> 16;
    }

    inline u32 GetSeed() const
    {
      return m_seed;
    }

    inline void SetSeed(u32 p)
    {
      m_seed = p;
    }

    inline float Float()
    {
      return Next() * 0.000015259022;
    }

    inline float Range(float min, float max)
    {
      return min + Float() * (max - min);
    }

    inline s32 Range(s32 min, s32 max)
    {
      s32 diff = max - min;
      s32 rand = -1;
      while (rand < 0)
        rand = s32((Next() << 16) | Next());
      return rand % diff + min;
    }
};

#endif //PRIME_WATCH_DUMP_CRANDOM16IMPL_HPP
