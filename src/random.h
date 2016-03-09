// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2015-2016 Silk Network
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DARKSILK_RANDOM_H
#define DARKSILK_RANDOM_H

#include "uint256.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <stdint.h>

bool GetRandBytes(unsigned char* buf, int num);
int GetRandInt(int nMax);
uint64_t GetRand(uint64_t nMax);
uint256 GetRandHash();

void RandAddSeed();
void RandAddSeedPerfmon();

inline int64_t GetPerformanceCounter()
{
    int64_t nCounter = 0;
#ifdef WIN32
    QueryPerformanceCounter((LARGE_INTEGER*)&nCounter);
#else
    timeval t;
    gettimeofday(&t, NULL);
    nCounter = (int64_t) t.tv_sec * 1000000 + t.tv_usec;
#endif
    return nCounter;
}

/**
 * MWC RNG of George Marsaglia
 * This is intended to be fast. It has a period of 2^59.3, though the
 * least significant 16 bits only have a period of about 2^30.1.
 *
 * @return random value
 */
extern uint32_t insecure_rand_Rz;
extern uint32_t insecure_rand_Rw;
static inline uint32_t insecure_rand(void)
{
  insecure_rand_Rz=36969*(insecure_rand_Rz&65535)+(insecure_rand_Rz>>16);
  insecure_rand_Rw=18000*(insecure_rand_Rw&65535)+(insecure_rand_Rw>>16);
  return (insecure_rand_Rw<<16)+insecure_rand_Rz;
}

/**
 * Seed insecure_rand using the random pool.
 * @param Deterministic Use a determinstic seed
 */
void seed_insecure_rand(bool fDeterministic=false);

#endif // DARKSILK_RANDOM_H
