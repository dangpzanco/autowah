/* Original copyright notice
Copyright (c) 2007, Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
  contributors may be used to endorse or promote products derived from this 
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef FIX32_H
#define FIX32_H

#include <cstdint>
#include <cmath>

namespace fp
{
	// The template argument q in all of the following functions refers to the 
	// fixed point precision (e.g. q = 8 gives 24.8 fixed point functions).

	//! @brief		Perform a fixed point multiplication without a 64-bit intermediate result.
	//!	@note 		This is fast but beware of intermediary overflow!
	template <uint8_t q>
	inline int32_t FixMulF(int32_t a, int32_t b)
	{
		return (a * b) >> q;
	}

	//! @brief		Perform a fixed point multiplication using a 64-bit intermediate result to
	//! 			prevent intermediary overflow problems.
	//! @note 		Slower than fix32::FixMulF()
	template <uint8_t q>
	inline int32_t FixMul(int32_t a, int32_t b)
	{
		return (int32_t)(((int64_t)a * b) >> q);
	}

	// Fixed point division
	template <uint8_t q>
	inline int32_t fixdiv(int32_t a, int32_t b)
	{
		#if 0
		return (int32_t)((((int64_t)a) << q) / b);
		#else	
		// The following produces the same results as the above but gcc 4.0.3 
		// generates fewer instructions (at least on the ARM processor).
		union {
			int64_t a;
			struct {
				int32_t l;
				int32_t h;
			};
		} x;

		x.l = a << q;
		x.h = a >> (sizeof(int32_t) * 8 - q);
		return (int32_t)(x.a / b);
		#endif
	}

	namespace detail {
		inline uint32_t CountLeadingZeros(uint32_t x)
		{
			uint32_t exp = 31;

			if (x & 0xffff0000) {
				exp -= 16;
				x >>= 16;
			}

			if (x & 0xff00) {
				exp -= 8;
				x >>= 8;
			}

			if (x & 0xf0) {
				exp -= 4;
				x >>= 4;
			}

			if (x & 0xc) {
				exp -= 2;
				x >>= 2;
			}

			if (x & 0x2) {
				exp -= 1;
			}

			return exp;
		}
	}

	// q is the precision of the input
	// output has 32-q bits of fraction
	template <uint8_t q>
	inline int32_t fixinv(int32_t a)
	{
		int32_t x;

		bool sign = false;

		if (a < 0) {
			sign = true;
			a = -a;
		}

		static const uint16_t rcp_tab[] = {
			0x8000, 0x71c7, 0x6666, 0x5d17, 0x5555, 0x4ec4, 0x4924, 0x4444
		};

		int32_t exp = detail::CountLeadingZeros(a);
		x = ((int32_t)rcp_tab[(a >> (28 - exp)) & 0x7]) << 2;
		exp -= 16;

		if (exp <= 0)
			x >>= -exp;
		else
			x <<= exp;

		// Two iterations of newton-raphson  x = x(2-ax)
		x = FixMul<(32 - q)>(x, ((2 << (32 - q)) - FixMul<q>(a, x)));
		x = FixMul<(32 - q)>(x, ((2 << (32 - q)) - FixMul<q>(a, x)));

		if (sign)
			return -x;
		else
			return x;
	}

	//! @brief		Converts from float to a raw fixed-point number.
	//! @details	Do not write "myFpNum = FloatToRawFix32()"! This function outputs a raw
	//!				number, so you would have to use the syntax "myFpNum.rawVal = FloatToRawFix32()".
	//! @warning	Slow!
	template <uint8_t q>
	int32_t FloatToRawFix32(float f)
	{
		return (int32_t)(f * (1 << q));
	}

	//! @brief		Converts from double to a raw fixed-point number.
	//! @details	Do not write "myFpNum = FloatToRawFix32()"! This function outputs a raw
	//!				number, so you would have to use the syntax "myFpNum.rawVal = FloatToRawFix32()".
	//! @warning	Slow!
	template <uint8_t q>
	int32_t DoubleToRawFix32(double f)
	{
		return (int32_t)(f * (double)(1 << q));
	}

	//! The template argument q in all of the following functions refers to the 
	//! fixed point precision (e.g. q = 8 gives 24.8 fixed point functions).
	//! Contains mathematical operator overloading. Doesn't have modulus (%) overloading
	template <uint8_t q>
	class fix32 {

	public:

		//! @brief		The fixed-point number is stored in this basic data type.
		int32_t rawVal;

		fix32()
		{
		}

		fix32(int8_t i) :
			rawVal((int32_t)i << q)
		{

		}

		// int16_t is assumed to be Q1.15: keep the best precision shifting right or left
		fix32(int16_t i) :
			rawVal(q <= 15 ? (int32_t)i >> (15 - q) : (int32_t)i << (q - 15))
		{

		}

		fix32(int32_t i) :
			rawVal(i << q)
		{

		}

		fix32(float f) :
			rawVal(FloatToRawFix32<q>(f))
		{

		}

		fix32(double f) :
			rawVal(FloatToRawFix32<q>((float)f))
		{

		}

		// Compound Arithmetic Overloads

		fix32& operator += (fix32 r)
		{
			rawVal += r.rawVal;
			return *this;
		}

		fix32& operator -= (fix32 r)
		{
			rawVal -= r.rawVal;
			return *this;
		}

		//! @brief		Overlaod for '*=' operator.
		//! @details	Uses intermediatary casting to int64_t to prevent overflows.
		fix32& operator *= (fix32 r)
		{
			rawVal = FixMul<q>(rawVal, r.rawVal);
			return *this;
		}

		//! @brief		Overlaod for '/=' operator.
		//! @details	Uses intermediatary casting to int64_t to prevent overflows.
		fix32& operator /= (fix32 r)
		{
			rawVal = fixdiv<q>(rawVal, r.rawVal);
			return *this;
		}

		//! @brief		Overlaod for '%=' operator.
		fix32& operator %= (fix32 r)
		{
			rawVal %= r.rawVal;
			return *this;
		}


		fix32& operator *= (int32_t r)
		{
			rawVal *= r;
			return *this;
		}

		fix32& operator /= (int32_t r)
		{
			rawVal /= r;
			return *this;
		}

		// Simple Arithmetic Overloads

		//! @brief		Overload for '-itself' operator.
		fix32 operator - () const
		{
			fix32 x;
			x.rawVal = -rawVal;
			return x;
		}

		//! @brief		Overload for '+' operator.
		//! @details	Uses '+=' operator.
		fix32 operator + (fix32 r) const
		{
			fix32 x = *this;
			x += r;
			return x;
		}

		//! @brief		Overload for '-' operator.
		//! @details	Uses '-=' operator.
		fix32 operator - (fix32 r) const
		{
			fix32 x = *this;
			x -= r;
			return x;
		}

		//! @brief		Overload for '*' operator.
		//! @details	Uses '*=' operator.
		fix32 operator * (fix32 r) const
		{
			fix32 x = *this;
			x *= r;
			return x;
		}

		//! @brief		Overload for '/' operator.
		//! @details	Uses '/=' operator.
		fix32 operator / (fix32 r) const
		{
			fix32 x = *this;
			x /= r;
			return x;
		}

		//! @brief		Overload for '%' operator.
		//! @details	Uses '%=' operator.
		fix32 operator % (fix32 r) const
		{
			fix32 x = *this;
			x %= r;
			return x;
		}

		// fix32-fix32 Binary Operator Overloads

		bool operator == (fix32 r) const
		{
			return rawVal == r.rawVal;
		}

		bool operator != (const fix32 &r)
		{
			return !(*this == r);
		}

		bool operator <  (const fix32 &r)
		{
			return rawVal < r.rawVal;
		}

		bool operator >  (const fix32 &r)
		{
			return rawVal > r.rawVal;
		}

		bool operator <= (fix32 r) const
		{
			return rawVal <= r.rawVal;
		}

		bool operator >= (fix32 r) const
		{
			return rawVal >= r.rawVal;
		}

		//! @defgroup Explicit "From fix32" Conversion Overloads (casts)
		//! @{

		//! @brief		Conversion operator from fixed-point to int16_t in Q15.1 format.
		//! @warning	Possible loss of accuracy from conversion from int32_t to int16_t.
		operator int16_t()
		{
			// Right-shift to get rid of all the extra decimal bits (truncate)
			//return (int16_t)(rawVal >> (q-15));

			if (q <= 15)
				return (int32_t)rawVal << (15 - q);
			else
				return (int32_t)rawVal >> (q - 15);
		}

		//! @brief		Conversion operator from fixed-point to int32_t.
		operator int32_t()
		{
			// Right-shift to get rid of all the decimal bits (truncate)
			return (rawVal >> q);
		}

		//! @brief		Conversion operator from fixed-point to int64_t.
		operator int64_t()
		{
			// Right-shift to get rid of all the decimal bits (truncate)
			return (int64_t)(rawVal >> q);
		}

		//! @brief		Conversion operator from fixed-point to float.
		operator float()
		{
			return (float)rawVal / (float)(1 << q);
		}

		//! @brief		Conversion operator from fixed-point to double.
		//! @note		Similar to float conversion.
		operator double()
		{
			return (double)rawVal / (double)(1 << q);
		}

		//! @}

		// Overloads Between fix32 And int32_t

		fix32 operator + (int32_t r) const
		{
			fix32 x = *this;
			x += r;
			return x;
		}

		fix32 operator - (int32_t r) const
		{
			fix32 x = *this;
			x -= r;
			return x;
		}

		fix32 operator * (int32_t r) const
		{
			fix32 x = *this;
			x *= r;
			return x;
		}

		fix32 operator / (int32_t r) const
		{
			fix32 x = *this;
			x /= r;
			return x;
		}

		bool operator >  (int32_t r) const
		{
			return rawVal > (r << q);
		}

		bool operator >=  (int32_t r) const
		{
			return rawVal >= (r << q);
		}

		bool operator <  (int32_t r) const
		{
			return rawVal < (r << q);
		}

		bool operator <=  (int32_t r) const
		{
			return rawVal < (r << q);
		}

		bool operator ==  (int32_t r) const
		{
			return rawVal == (r << q);
		}

		bool operator !=  (int32_t r) const
		{
			return rawVal != (r << q);
		}

	};

	// Specializations for use with plain integers

	//! @note 		Assumes integer has the same precision as fix32
	template <uint8_t q>
	inline fix32<q> operator + (int32_t a, fix32<q> b)
	{
		return b + a;
	}

	//! @note 		Assumes integer has the same precision as fix32
	template <uint8_t q>
	inline fix32<q> operator - (int32_t a, fix32<q> b)
	{
		return -b + a;
	}

	//! @note 		Assumes integer has the same precision as fix32
	template <uint8_t q>
	inline fix32<q> operator * (int32_t a, fix32<q> b)
	{
		return b * a;
	}

	template <uint8_t q>
	inline fix32<q> operator / (int32_t a, fix32<q> b)
	{
		fix32<q> r(a);
		r /= b;
		return r;
	}

	// math functions
	// no default implementation

	template <uint8_t q>
	inline fix32<q> exp(fix32<q> a) {
		return fix32<q>(expf((float)a));
	}

	template <uint8_t q>
	inline fix32<q> sin(fix32<q> a) {
		static fix32<q> factor3 = -1.0 / 6.0;
		return a * (1 + factor3*a*a);
	}

	template <uint8_t q>
	inline fix32<q> psin(fix32<q> a) {
		static fix32<q> factor3 = -1.0 / 6.0;
		static fix32<q> factor5 = 1.0 / 120.0;
		fix32<q> a2 = a*a;
		fix32<q> a4 = a2*a2;
		return a*(1 + factor3*a2 + factor5*a4);
	}

	template <uint8_t q>
	inline fix32<q> cos(fix32<q> a);

	template <uint8_t q>
	inline fix32<q> sqrt(fix32<q> a);

	template <uint8_t q>
	inline fix32<q> rsqrt(fix32<q> a);

	template <uint8_t q>
	inline fix32<q> inv(fix32<q> a);

	template <uint8_t q>
	inline fix32<q> abs(fix32<q> a)
	{
		fix32<q> r;
		r.rawVal = a.rawVal > 0 ? a.rawVal : -a.rawVal;
		return r;
	}

	// The multiply accumulate case can be optimized.
	template <uint8_t q>
	inline fix32<q> multiply_accumulate(
		int32_t count,
		const fix32<q> *a,
		const fix32<q> *b)
	{
		long long result = 0;
		for (int32_t i = 0; i < count; ++i)
			result += static_cast<long long>(a[i].rawVal) * b[i].rawVal;
		fix32<q> r;
		r.rawVal = static_cast<int32_t>(result >> q);
		return r;
	}
} // namespace fixpoint

#endif // #ifndef FIX32_H

// EOF
