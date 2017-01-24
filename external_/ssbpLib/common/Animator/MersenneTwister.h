#ifndef __MERSENNETWISTER__
#define __MERSENNETWISTER__

/*
The MIT License (MIT)

A C-program for MT19937, with initialization improved 2002/1/26.
Coded by Takuji Nishimura and Makoto Matsumoto.

Before using, initialize the state by using init_genrand(seed)  
or init_by_array(init_key, key_length).

Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
All rights reserved.



Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:



The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <vector>
using namespace std;

class CMersenneTwister {
private:
	const int             m_iN          ;
	const int             m_iM          ;
	const int             m_ulMATRIX_A  ;
	const unsigned long   m_ulUPPER_MASK;
	const unsigned long   m_ulLOWER_MASK;
	vector<unsigned long> m_veculMT     ;
	int                   m_iMTI        ;
public:
	CMersenneTwister()
	: m_iN          (624)
	, m_iM          (397)
	, m_ulMATRIX_A  (0x9908b0dfUL)	// constant vector a
	, m_ulUPPER_MASK(0x80000000UL)	// most significant w-r bits
	, m_ulLOWER_MASK(0x7fffffffUL)	// least significant r bits
	{
		m_veculMT.resize(m_iN);
		m_iMTI = m_iN + 1;			// mti==N+1 means mt[N] is not initialized
		unsigned long aulInit[4] = { 0x123, 0x234, 0x345, 0x456 };	// use default seed
		init_by_array(aulInit, sizeof aulInit / sizeof aulInit[0]);
	}

	~CMersenneTwister(){
    	m_veculMT.clear();
	}


	// initializes mt[N] with a seed
	void init_genrand(unsigned long ulSeed) {
		m_veculMT[0] = ulSeed & 0xffffffffUL;
		{for (m_iMTI = 1; m_iMTI < m_iN; m_iMTI++) {
			m_veculMT[m_iMTI] = (1812433253UL * (m_veculMT[m_iMTI - 1] ^ (m_veculMT[m_iMTI - 1] >> 30)) + m_iMTI); 
			// See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
			// In the previous versions, MSBs of the seed affect
			// only MSBs of the array mt[].
			// 2002/01/09 modified by Makoto Matsumoto
			m_veculMT[m_iMTI] &= 0xffffffffUL;	// for >32 bit machines
		}}
	}
	// initialize by an array with array-length
	// init_key is the array for initializing keys
	// key_length is its length
	void init_by_array(unsigned long aulInitKey[], int iKeyLength) {
		init_genrand(19650218UL);
		int iI = 1;
		int iJ = 0;
		{for (int k = m_iN > iKeyLength ? m_iN : iKeyLength; k; k--) {
			m_veculMT[iI] = (m_veculMT[iI] ^ ((m_veculMT[iI - 1] ^ (m_veculMT[iI - 1] >> 30)) * 1664525UL))
			  + aulInitKey[iJ] + iJ; /* non linear */
			m_veculMT[iI] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
			iI++; iJ++;
			if (iI >= m_iN) { m_veculMT[0] = m_veculMT[m_iN - 1]; iI = 1; }
			if (iJ >= iKeyLength) { iJ = 0; }
		}}
		{for (int k = m_iN - 1; k; k--) {
			m_veculMT[iI] = (m_veculMT[iI] ^ ((m_veculMT[iI - 1] ^ (m_veculMT[iI - 1] >> 30)) * 1566083941UL)) - iI; // non linear
			m_veculMT[iI] &= 0xffffffffUL;	// for WORDSIZE > 32 machines
			iI++;
			if (iI >= m_iN) { m_veculMT[0] = m_veculMT[m_iN - 1]; iI = 1; }
		}}
		m_veculMT[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
	}
	// generates a random number on [0,0xffffffff]-interval
	unsigned long genrand_uint32() {
		unsigned long aulMag01[2] = { 0x0UL, (unsigned long)m_ulMATRIX_A };
		// mag01[x] = x * MATRIX_A  for x=0,1
		if (m_iMTI >= m_iN) {
			// generate N words at one time
			if (m_iMTI == m_iN+1) {
				/* if init_genrand() has not been called, */
				init_genrand(5489UL); /* a default initial seed is used */
			}
			int iKK = 0;
			{for (; iKK < m_iN - m_iM; iKK++) {
				unsigned long ulY = (m_veculMT[iKK] & m_ulUPPER_MASK) | (m_veculMT[iKK + 1] & m_ulLOWER_MASK);
				m_veculMT[iKK] = m_veculMT[iKK + m_iM] ^ (ulY >> 1) ^ aulMag01[ulY & 0x1UL];
			}}
			{for (; iKK < m_iN - 1; iKK++) {
				unsigned long ulY = (m_veculMT[iKK] & m_ulUPPER_MASK) | (m_veculMT[iKK + 1] & m_ulLOWER_MASK);
				m_veculMT[iKK] = m_veculMT[iKK + (m_iM - m_iN)] ^ (ulY >> 1) ^ aulMag01[ulY & 0x1UL];
			}}
			unsigned long ulY = (m_veculMT[m_iN - 1] & m_ulUPPER_MASK) | (m_veculMT[0] & m_ulLOWER_MASK);
			m_veculMT[m_iN - 1] = m_veculMT[m_iM - 1] ^ (ulY >> 1) ^ aulMag01[ulY & 0x1UL];
			m_iMTI = 0;
		}
		unsigned long ulY = m_veculMT[m_iMTI++];
		// Tempering
		ulY ^= (ulY >> 11)               ;
		ulY ^= (ulY <<  7) & 0x9d2c5680UL;
		ulY ^= (ulY << 15) & 0xefc60000UL;
		ulY ^= (ulY >> 18)               ;
		return ulY;
	}
	// generates a random floating point number on [0,1]
	double genrand_real1() {
		return genrand_uint32() * (1.0 / 4294967295.0);	// divided by 2^32-1
	}
	// generates a random floating point number on [0,1)
	double genrand_real2() {
		return genrand_uint32() * (1.0 / 4294967296.0);	// divided by 2^32
	}
	// generates a random integer number from 0 to N-1
	int genrand_N(int iN) {
		return (int)(genrand_uint32() * (iN / 4294967296.0));
	}
};




#endif

