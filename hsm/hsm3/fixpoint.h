//============================================================================
//
// $URL: svn+ssh://ristna.unjo.local/var/svn/220_06_005_COMMON/trunk/include/fixpoint.h $
// $Revision: 11802 $
// $Date: 2016-11-28 11:46:05 +0100 (mån, 28 nov 2016) $
// $Author: andreas.fritiofson $
//
// Description :
//
// Copyright   : UNJO AB
//
//============================================================================


#ifndef FIXPOINT_H_
#define FIXPOINT_H_

#include <stdint.h>

typedef int32_t s16b16;

static inline int32_t clip_s64_to_s32(int64_t x) __attribute__ ((const,always_inline));
static inline int s16b16_int(s16b16 x) __attribute__ ((const,always_inline));
static inline s16b16 int_s16b16(int x) __attribute__ ((const,always_inline));
static inline float s16b16_float(s16b16 x) __attribute__ ((const,always_inline));
static inline s16b16 float_s16b16(float x) __attribute__ ((const,always_inline));
static inline s16b16 mul(s16b16 a, s16b16 b) __attribute__ ((const,always_inline));
s16b16 divf(s16b16 a, s16b16 b) __attribute__ ((const));
static inline s16b16 absf(s16b16 a) __attribute__ ((const,always_inline));
s16b16 isqrt(s16b16 x) __attribute__ ((const));
s16b16 atos16b16(const char *s);

static inline int32_t clip_s64_to_s32(int64_t x) {
	return (int32_t)(x >> 32) != (int32_t)x >> 31 ?
			0x7FFFFFFF ^ (int32_t)(x >> 63) : (int32_t)x;
}

static inline int s16b16_int(s16b16 x) {
	return clip_s64_to_s32((int64_t)x + 32768) >> 16;
}

static inline s16b16 int_s16b16(int x) {
	return x*65536;
}

static inline float s16b16_float(s16b16 x) {
	return (float)x/65536.0f;
}

static inline s16b16 float_s16b16(float x) {
	return (s16b16)(x*65536.0f+0.5f);
}

static inline s16b16 mul(s16b16 a, s16b16 b) {
	return clip_s64_to_s32(((int64_t)a * b) >> 16);
}

static inline s16b16 absf(s16b16 a) {
	return (a < 0) ? -a : a;
}

/* Macro versions of conversion functions for static initialization */
#define INT_S16B16(x) ((s16b16)(x)*65536)
#define FLOAT_S16B16(x) ((s16b16)((float)(x)*65536.0f+0.5f))
#define S16B16_FLOAT(x) ((float)(s16b16)(x)/65536.0f)

#define SQRT3_DIV_3 FLOAT_S16B16(0.577350269189626f)
#define SQRT3_DIV_2 FLOAT_S16B16(0.8660254038f)

static inline void clarke(s16b16 ab[2], const s16b16 uvw[3])
{
	/* [2/3         -1/3         -1/3]
	 * [  0  1/3*sqrt(3) -1/3*sqrt(3)]
	 */
	s16b16 ab_0, ab_1;
	ab_0 = (uvw[0] * 2 - uvw[1] - uvw[2]) / 3;
	ab_1 = mul(SQRT3_DIV_3, uvw[1] - uvw[2]);
	ab[0] = ab_0;
	ab[1] = ab_1;
}

static inline void clarke_zero_sum(s16b16 ab[2], const s16b16 uvw[2])
{
	/* [2/3         -1/3         -1/3]
	 * [  0  1/3*sqrt(3) -1/3*sqrt(3)]
	 * simplified under the restriction that u + v + w = 0
	 */
	ab[0] = uvw[0];
	ab[1] = mul(SQRT3_DIV_3, uvw[0] + 2 * uvw[1]);
}

static inline void clarke_mul_3_div_2(s16b16 ab[2], const s16b16 uvw[3])
{
	/* [2/3         -1/3         -1/3]
	 * [  0  1/3*sqrt(3) -1/3*sqrt(3)]
	 * but multiplied by 3/2 to get clarke(svm(v)) == v
	 */
	s16b16 ab_0, ab_1;
	ab_0 = uvw[0] - (uvw[1] + uvw[2]) / 2;
	ab_1 = mul(SQRT3_DIV_2, uvw[1] - uvw[2]);
	ab[0] = ab_0;
	ab[1] = ab_1;
}

static inline void vector_rot(s16b16 v[2], const s16b16 a[2], const s16b16 b[2]) {
	s16b16 v_0, v_1;
	v_0 = clip_s64_to_s32(((int64_t)a[0] * b[0] - (int64_t)a[1] * b[1]) >> 16);
	v_1 = clip_s64_to_s32(((int64_t)a[1] * b[0] + (int64_t)a[0] * b[1]) >> 16);
	v[0] = v_0;
	v[1] = v_1;
}

static inline void vector_rotinv(s16b16 v[2], const s16b16 a[2], const s16b16 b[2]) {
	s16b16 v_0, v_1;
	v_0 = clip_s64_to_s32(((int64_t)a[0] * b[0] + (int64_t)a[1] * b[1]) >> 16);
	v_1 = clip_s64_to_s32(((int64_t)a[1] * b[0] - (int64_t)a[0] * b[1]) >> 16);
	v[0] = v_0;
	v[1] = v_1;
}

#endif /* FIXPOINT_H_ */
