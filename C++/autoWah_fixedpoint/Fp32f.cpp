//!
//! @file 				Fp32f.cpp
//! @author 			Markus Trenkwalder
//! @edited 			Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @created			2012-10-23
//! @last-modified		2017-06-26
//! @brief 				Fast 32-bit fixed point library.
//! @details
//!		See README.rst in root dir for more info.


//===============================================================================================//
//========================================= INCLUDES ============================================//
//===============================================================================================//

#include "Fp32f.hpp"

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace Fp 
{

	//===============================================================================================//
	//==================================== PRIVATE VARIABLES ========================================//
	//===============================================================================================//

	//static const int32_t FIX16_2PI	= FloatToFix32<16>(6.28318530717958647692f);
	//static const int32_t FIX16_R2PI = FloatToFix32<16>(1.0f/6.28318530717958647692f);

	//===============================================================================================//
	//===================================== PUBLIC FUNCTIONS ========================================//
	//===============================================================================================//

	/*
	int32_t fixcos16(int32_t a) 
	{
		int32_t v;
		// reduce to [0,1]
		while (a < 0) a += FIX16_2PI;
		a = FixMul<16>(a, FIX16_R2PI);
		a += 0x4000;

		// now in the range [0, 0xffff], reduce to [0, 0xfff] 
		a >>= 4;

		v = (a & 0x400) ? sin_tab[0x3ff - (a & 0x3ff)] : sin_tab[a & 0x3ff];
		v = FixMul<16>(v, 1 << 16);
		return (a & 0x800) ? -v : v;
	}

	int32_t fixsin16(int32_t a)
	{
		int32_t v;

		// reduce to [0,1) 
		while (a < 0) a += FIX16_2PI;
		a = FixMul<16>(a, FIX16_R2PI);

		// now in the range [0, 0xffff], reduce to [0, 0xfff] 
		a >>= 4;

		v = (a & 0x400) ? sin_tab[0x3ff - (a & 0x3ff)] : sin_tab[a & 0x3ff];
		v = FixMul<16>(v, 1 << 16);
		return (a & 0x800) ? -v : v;
	}
	*/

	template <uint8_t q>
	Fp32f<q> sin(Fp32f<q> a)
	{
		static Fp32f<q> factor3 = -1.0 / 6.0;
		return a * (Fp32f<q>(1) + factor3*a*a);
	}

	int32_t fixrsqrt16(int32_t a)
	{
		int32_t x;

		static const uint16_t rsq_tab[] = { // domain 0.5 .. 1.0-1/16 
			0xb504, 0xaaaa, 0xa1e8, 0x9a5f, 0x93cd, 0x8e00, 0x88d6, 0x8432,
		};

		int32_t i, exp;
		if (a == 0) return 0x7fffffff;
		if (a == (1<<16)) return a;

		exp = detail::CountLeadingZeros(a);
		x = rsq_tab[(a>>(28-exp))&0x7]<<1;

		exp -= 16;
		if (exp <= 0)
			x >>= -exp>>1;
		else
			x <<= (exp>>1)+(exp&1);
		if (exp&1) x = FixMul<16>(x, rsq_tab[0]);


		// newton-raphson
		// x = x/2*(3-(a*x)*x)
		i = 0;
		do {

			x = FixMul<16>((x>>1),((1<<16)*3 - FixMul<16>(FixMul<16>(a,x),x)));
		} while(++i < 3);

		return x;
	}

	static inline int32_t fast_div16(int32_t a, int32_t b)
	{
		if ((b >> 24) && (b >> 24) + 1) {
			return FixMul<16>(a >> 8, fixinv<16>(b >> 8));
		} else {
			return FixMul<16>(a, fixinv<16>(b));
		}
	}

	//! @brief		Calculates squareroot
	int32_t fixsqrt16(int32_t a) 
	{
		int32_t s;
		int32_t i;
		s = (a + (1<<16)) >> 1;
		// 6 iterations to converge
		for (i = 0; i < 6; i++)
			s = (s + fast_div16(a, s)) >> 1;
		return s;
	}

} // namespace Fp

// EOF
