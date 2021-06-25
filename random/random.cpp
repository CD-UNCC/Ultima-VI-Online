
#include "random.h" 
#include <stdlib.h>

extern void isaac_init(unsigned long seed);
extern float isaac_rand();
//wincrypt is NOT compatible with the very old win98 psdk include files please use some other random method if compiling with the old shit

//#define USE_MERS
#define USE_ISAAC

#define MERS_N   351
#define MERS_M   175
#define MERS_R   19
#define MERS_U   11
#define MERS_S   7
#define MERS_T   15
#define MERS_L   17
#define MERS_A   0xE4BD75F5
#define MERS_B   0x655E5280
#define MERS_C   0xFFD58000


static unsigned int mt[MERS_N];
static int mti=0;
static unsigned char initialized=0;

void Randomc(unsigned long seed) {
  initialized=1;
#ifdef USE_MERS
   const unsigned int factor = 1812433253UL;
   mt[0]= seed;
   for (mti=1; mti < MERS_N; mti++) {
      mt[mti] = (factor * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
   }
#else
#ifdef USE_ISAAC
  isaac_init(seed);
#else
   srand(seed);
#endif
#endif
}



float Randomc() {
  if(!initialized){
    return ((float)rand())/32768.0f;
  }
    
#ifdef USE_MERS
   // Generate 32 random bits
   unsigned int y;
   if (mti >= MERS_N) {
      // Generate MERS_N words at one time
      const unsigned int LOWER_MASK = (1LU << MERS_R) - 1;       // Lower MERS_R bits
      const unsigned int UPPER_MASK = 0xFFFFFFFF << MERS_R;      // Upper (32 - MERS_R) bits
      static const unsigned int mag01[2] = {0, MERS_A};

      int kk;
      for (kk=0; kk < MERS_N-MERS_M; kk++) {    
         y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
         mt[kk] = mt[kk+MERS_M] ^ (y >> 1) ^ mag01[y & 1];}

      for (; kk < MERS_N-1; kk++) {    
         y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
         mt[kk] = mt[kk+(MERS_M-MERS_N)] ^ (y >> 1) ^ mag01[y & 1];}      

      y = (mt[MERS_N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
      mt[MERS_N-1] = mt[MERS_M-1] ^ (y >> 1) ^ mag01[y & 1];
      mti = 0;
   }
   y = mt[mti++];

   // Tempering (May be omitted):
   y ^=  y >> MERS_U;
   y ^= (y << MERS_S) & MERS_B;
   y ^= (y << MERS_T) & MERS_C;
   y ^=  y >> MERS_L;

   return ((float)(y))/(65536.0f*65536.0f);
#else
#ifdef USE_ISAAC
  return isaac_rand();
#else
  return ((float)rand())/32768.0f;
#endif
#endif
}
