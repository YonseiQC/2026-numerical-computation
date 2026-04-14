#include <immintrin.h>
#include <span>

# define ALIGN32_BEG
# define ALIGN32_END __attribute__((aligned(32)))

#define _PS256_CONST(Name, Val)                                            \
  static const ALIGN32_BEG float _ps256_##Name[8] ALIGN32_END = { Val, Val, Val, Val, Val, Val, Val, Val }
#define _PI32_CONST256(Name, Val)                                            \
  static const ALIGN32_BEG int _pi32_256_##Name[8] ALIGN32_END = { Val, Val, Val, Val, Val, Val, Val, Val }

_PS256_CONST(1  , 1.0f);
_PS256_CONST(0p5, 0.5f);

_PI32_CONST256(0, 0);
_PI32_CONST256(1, 1);
_PI32_CONST256(inv1, ~1);
_PI32_CONST256(2, 2);
_PI32_CONST256(4, 4);
_PI32_CONST256(0x7f, 0x7f);

_PS256_CONST(exp_hi,	88.3762626647949f);
_PS256_CONST(exp_lo,	-88.3762626647949f);

_PS256_CONST(cephes_LOG2EF, 1.44269504088896341);
_PS256_CONST(cephes_exp_C1, 0.693359375);
_PS256_CONST(cephes_exp_C2, -2.12194440e-4);

_PS256_CONST(cephes_exp_p0, 1.9875691500E-4);
_PS256_CONST(cephes_exp_p1, 1.3981999507E-3);
_PS256_CONST(cephes_exp_p2, 8.3334519073E-3);
_PS256_CONST(cephes_exp_p3, 4.1665795894E-2);
_PS256_CONST(cephes_exp_p4, 1.6666665459E-1);
_PS256_CONST(cephes_exp_p5, 5.0000001201E-1);

#define _PD256_CONST(Name, Val)                                            \
  static const ALIGN32_BEG double _pd256_##Name[4] ALIGN32_END = { Val, Val, Val, Val }

_PD256_CONST(exp_hi, 709.437);
_PD256_CONST(exp_lo, -709.437);
_PD256_CONST(LOG2EF, 1.4426950408889634073599);
_PD256_CONST(ln2_C1, 6.93147180369123816490e-1);   /* high part of ln(2) */
_PD256_CONST(ln2_C2, 1.90821492927058770002e-10);  /* low  part of ln(2) */
_PD256_CONST(1, 1.0);
_PD256_CONST(0p5, 0.5);

/* Minimax polynomial coefficients for exp(r)-approximation on [-ln2/2, ln2/2].
 * The evaluation uses exp(x) = 1 + x + x^2 * P(x), so p9=1/2!, p8=1/3!, ...
 * (coefficients from SLEEF / Cephes double-precision exp) */
_PD256_CONST(cephes_exp_p0, 2.51086977355939492552e-8);
_PD256_CONST(cephes_exp_p1, 2.76897204145097766853e-7);
_PD256_CONST(cephes_exp_p2, 2.75572514993676078894e-6);
_PD256_CONST(cephes_exp_p3, 2.48015872896003956019e-5);
_PD256_CONST(cephes_exp_p4, 1.98412698088018840403e-4);
_PD256_CONST(cephes_exp_p5, 1.38888888919030047796e-3);
_PD256_CONST(cephes_exp_p6, 8.33333333333331107820e-3);
_PD256_CONST(cephes_exp_p7, 4.16666666666666579923e-2);
_PD256_CONST(cephes_exp_p8, 1.66666666666666657415e-1);
_PD256_CONST(cephes_exp_p9, 5.00000000000000009337e-1);

__m256d exp256_pd(__m256d x) {
  __m256d tmp, fx;
  __m256d one = *(__m256d*)_pd256_1;

  x = _mm256_min_pd(x, *(__m256d*)_pd256_exp_hi);
  x = _mm256_max_pd(x, *(__m256d*)_pd256_exp_lo);

  /* express exp(x) as exp(g + n*log(2)) */
  fx = _mm256_mul_pd(x, *(__m256d*)_pd256_LOG2EF);
  fx = _mm256_add_pd(fx, *(__m256d*)_pd256_0p5);
  tmp = _mm256_floor_pd(fx);

  /* if greater, subtract 1 */
  __m256d mask = _mm256_cmp_pd(tmp, fx, _CMP_GT_OS);
  mask = _mm256_and_pd(mask, one);
  fx = _mm256_sub_pd(tmp, mask);

  /* x = x - fx * ln(2), split in two for cancellation-free subtraction */
  tmp = _mm256_mul_pd(fx, *(__m256d*)_pd256_ln2_C1);
  __m256d z = _mm256_mul_pd(fx, *(__m256d*)_pd256_ln2_C2);
  x = _mm256_sub_pd(x, tmp);
  x = _mm256_sub_pd(x, z);

  /* degree-10 polynomial: exp(x) = 1 + x + x^2 * P(x) */
  __m256d y = *(__m256d*)_pd256_cephes_exp_p0;
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p1);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p2);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p3);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p4);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p5);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p6);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p7);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p8);
  y = _mm256_mul_pd(y, x);
  y = _mm256_add_pd(y, *(__m256d*)_pd256_cephes_exp_p9);

  z = _mm256_mul_pd(x, x);   /* x^2 */
  y = _mm256_mul_pd(y, z);   /* P(x) * x^2 */
  y = _mm256_add_pd(y, x);   /* + x */
  y = _mm256_add_pd(y, one); /* + 1 */

  /* build 2^n:
   * doubles have 11-bit exponent (bias 1023) and 52-bit mantissa,
   * so we work in 64-bit integers throughout */
  __m128i imm0    = _mm256_cvttpd_epi32(fx);           /* 4×double → 4×int32 in 128b */
  __m256i imm0_64 = _mm256_cvtepi32_epi64(imm0);       /* sign-extend to 4×int64     */
  imm0_64 = _mm256_add_epi64(imm0_64, _mm256_set1_epi64x(1023LL));
  imm0_64 = _mm256_slli_epi64(imm0_64, 52);
  __m256d pow2n = _mm256_castsi256_pd(imm0_64);
  y = _mm256_mul_pd(y, pow2n);
  return y;
}

__m256 exp256_ps(__m256 x) {
  __m256 tmp = _mm256_setzero_ps(), fx;
  __m256i imm0;
  __m256 one = *(__m256*)_ps256_1;

  x = _mm256_min_ps(x, *(__m256*)_ps256_exp_hi);
  x = _mm256_max_ps(x, *(__m256*)_ps256_exp_lo);

  /* express exp(x) as exp(g + n*log(2)) */
  fx = _mm256_mul_ps(x, *(__m256*)_ps256_cephes_LOG2EF);
  fx = _mm256_add_ps(fx, *(__m256*)_ps256_0p5);

  /* how to perform a floorf with SSE: just below */
  //imm0 = _mm256_cvttps_epi32(fx);
  //tmp  = _mm256_cvtepi32_ps(imm0);
  
  tmp = _mm256_floor_ps(fx);

  /* if greater, substract 1 */
  //__m256 mask = _mm256_cmpgt_ps(tmp, fx);    
  __m256 mask = _mm256_cmp_ps(tmp, fx, _CMP_GT_OS);    
  mask = _mm256_and_ps(mask, one);
  fx = _mm256_sub_ps(tmp, mask);

  tmp = _mm256_mul_ps(fx, *(__m256*)_ps256_cephes_exp_C1);
  __m256 z = _mm256_mul_ps(fx, *(__m256*)_ps256_cephes_exp_C2);
  x = _mm256_sub_ps(x, tmp);
  x = _mm256_sub_ps(x, z);

  z = _mm256_mul_ps(x,x);
  
  __m256 y = *(__m256*)_ps256_cephes_exp_p0;
  y = _mm256_mul_ps(y, x);
  y = _mm256_add_ps(y, *(__m256*)_ps256_cephes_exp_p1);
  y = _mm256_mul_ps(y, x);
  y = _mm256_add_ps(y, *(__m256*)_ps256_cephes_exp_p2);
  y = _mm256_mul_ps(y, x);
  y = _mm256_add_ps(y, *(__m256*)_ps256_cephes_exp_p3);
  y = _mm256_mul_ps(y, x);
  y = _mm256_add_ps(y, *(__m256*)_ps256_cephes_exp_p4);
  y = _mm256_mul_ps(y, x);
  y = _mm256_add_ps(y, *(__m256*)_ps256_cephes_exp_p5);
  y = _mm256_mul_ps(y, z);
  y = _mm256_add_ps(y, x);
  y = _mm256_add_ps(y, one);

  /* build 2^n */
  imm0 = _mm256_cvttps_epi32(fx);
  // another two AVX2 instructions
  imm0 = _mm256_add_epi32(imm0, *(__m256i*)_pi32_256_0x7f);
  imm0 = _mm256_slli_epi32(imm0, 23);
  __m256 pow2n = _mm256_castsi256_ps(imm0);
  y = _mm256_mul_ps(y, pow2n);
  return y;
}

/* Assume that vec is 256bit aligned and the size is multiple of 4 */
void sigmoid_avx2(std::span<double> vec) {
	const __m256d sign_mask = _mm256_set1_pd(-0.0);
	const __m256d ones = _mm256_set1_pd(1.0);
	for(size_t i = 0; i < vec.size(); i+=4) {
		__m256d val = _mm256_load_pd(&vec[i]);
		val = _mm256_xor_pd(val, sign_mask);
		val = exp256_pd(val);
		val = _mm256_add_pd(val, ones);
		val = _mm256_div_pd(ones, val);
		_mm256_store_pd(&vec[i], val);
	}
}
