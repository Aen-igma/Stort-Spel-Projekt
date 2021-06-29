#pragma once

namespace Aen {

	namespace Concealed {
		
		#ifdef AEN_SSE2
		#include<immintrin.h>
		// ------------------------------------------ Vector Cross Product --------------------------------------------- //
		// Not much faster than scalar operators.
		
		template<class T>
		inline const T* SSE_Vec_Cross(const T* in1, const T* in2);

		template<>
		inline const float* SSE_Vec_Cross<float>(const float* in1, const float* in2) {
			__m128 v1 = _mm_load_ps(in1);
			__m128 v2 = _mm_load_ps(in2);
			__m128 sv1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
			__m128 sv2 = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(3, 1, 0, 2));
			__m128 a1 = _mm_mul_ps(sv1, v2);
			__m128 a2 = _mm_mul_ps(sv1, sv2);
			__m128 out = _mm_sub_ps(a2, _mm_shuffle_ps(a1, a1, _MM_SHUFFLE(3, 0, 2, 1)));
			return out.m128_f32;
		}

		template<>
		inline const int* SSE_Vec_Cross<int>(const int* in1, const int* in2) {
			__m128i v1 = _mm_loadu_si128((__m128i*)in1);
			__m128i v2 = _mm_loadu_si128((__m128i*)in2);
			__m128i sv1 = _mm_shuffle_epi32(v1, _MM_SHUFFLE(3, 0, 2, 1));
			__m128i sv2 = _mm_shuffle_epi32(v2, _MM_SHUFFLE(3, 1, 0, 2));
			__m128i a1 = _mm_mullo_epi32(sv1, v2);
			__m128i a2 = _mm_mullo_epi32(sv1, sv2);
			__m128i out = _mm_sub_epi32(a2, _mm_shuffle_epi32(a1, _MM_SHUFFLE(3, 0, 2, 1)));
			return out.m128i_i32;
		}

		// ------------------------------------------- Vector Dot Product ---------------------------------------------- //
		// Slightly faster than scalar operators for vec3 and vec4, relatively slow for vec2.

		template<class T, uint32_t N>
		inline const T SSE_Vec_Dot(const T* in1, const T* in2);

		template<>
		inline const double SSE_Vec_Dot<double, 2>(const double* in1, const double* in2) {
			__m128d x1 = _mm_load_pd(&in1[0]);
			__m128d y1 = _mm_load_pd(&in1[1]);
			__m128d x2 = _mm_load_pd(&in2[0]);
			__m128d y2 = _mm_load_pd(&in2[1]);
			__m128d out = _mm_add_pd(_mm_mul_pd(x1, x2), _mm_mul_pd(y1, y2));
			return out.m128d_f64[0];
		}

		template<>
		inline const float SSE_Vec_Dot<float, 2>(const float* in1, const float* in2) {
			__m128 x1 = _mm_load_ps(&in1[0]);
			__m128 y1 = _mm_load_ps(&in1[1]);
			__m128 x2 = _mm_load_ps(&in2[0]);
			__m128 y2 = _mm_load_ps(&in2[1]);
			__m128 out = _mm_add_ss(_mm_mul_ss(x1, x2), _mm_mul_ss(y1, y2));
			return out.m128_f32[0];
		}

		template<>
		inline const int SSE_Vec_Dot<int, 2>(const int* in1, const int* in2) {
			__m128i x1 = _mm_loadu_si32(&in1[0]);
			__m128i y1 = _mm_loadu_si32(&in1[1]);
			__m128i x2 = _mm_loadu_si32(&in2[0]);
			__m128i y2 = _mm_loadu_si32(&in2[1]);
			__m128i out = _mm_add_epi32(_mm_mullo_epi32(x1, x2), _mm_mullo_epi32(y1, y2));
			return out.m128i_i32[0];
		}

		template<>
		inline  const double SSE_Vec_Dot<double, 3>(const double* in1, const double* in2) {
			__m128d x1 = _mm_load_pd(&in1[0]);
			__m128d y1 = _mm_load_pd(&in1[1]);
			__m128d z1 = _mm_load_pd(&in1[2]);
			__m128d x2 = _mm_load_pd(&in2[0]);
			__m128d y2 = _mm_load_pd(&in2[1]);
			__m128d z2 = _mm_load_pd(&in2[2]);
			__m128d out = _mm_add_pd(_mm_mul_pd(x1, x2), _mm_add_pd(_mm_mul_pd(y1, y2), _mm_mul_pd(z1, z2)));
			return out.m128d_f64[0];
		}

		template<>
		inline const float SSE_Vec_Dot<float, 3>(const float* in1, const float* in2) {
			__m128 x1 = _mm_load_ps(&in1[0]);
			__m128 y1 = _mm_load_ps(&in1[1]);
			__m128 z1 = _mm_load_ps(&in1[2]);
			__m128 x2 = _mm_load_ps(&in2[0]);
			__m128 y2 = _mm_load_ps(&in2[1]);
			__m128 z2 = _mm_load_ps(&in2[2]);
			__m128 out = _mm_add_ss(_mm_mul_ss(x1, x2), _mm_add_ss(_mm_mul_ss(y1, y2), _mm_mul_ss(z1, z2)));
			return out.m128_f32[0];
		}

		template<>
		inline const int SSE_Vec_Dot<int, 3>(const int* in1, const int* in2) {
			__m128i x1 = _mm_loadu_si32(&in1[0]);
			__m128i y1 = _mm_loadu_si32(&in1[1]);
			__m128i z1 = _mm_loadu_si32(&in1[2]);
			__m128i x2 = _mm_loadu_si32(&in2[0]);
			__m128i y2 = _mm_loadu_si32(&in2[1]);
			__m128i z2 = _mm_loadu_si32(&in2[2]);
			__m128i out = _mm_add_epi32(_mm_mullo_epi32(x1, x2), _mm_add_epi32(_mm_mullo_epi32(y1, y2), _mm_mullo_epi32(z1, z2)));
			return out.m128i_i32[0];
		}

		template<>
		inline  const double SSE_Vec_Dot<double, 4>(const double* in1, const double* in2) {
			__m128d x1 = _mm_load_pd(&in1[0]);
			__m128d y1 = _mm_load_pd(&in1[1]);
			__m128d z1 = _mm_load_pd(&in1[2]);
			__m128d w1 = _mm_load_pd(&in1[3]);
			__m128d x2 = _mm_load_pd(&in2[0]);
			__m128d y2 = _mm_load_pd(&in2[1]);
			__m128d z2 = _mm_load_pd(&in2[2]);
			__m128d w2 = _mm_load_pd(&in2[3]);
			__m128d out = _mm_add_pd(_mm_mul_pd(x1, x2), _mm_add_pd(_mm_mul_pd(y1, y2), _mm_add_pd(_mm_mul_pd(z1, z2), _mm_mul_pd(w1, w2))));
			return out.m128d_f64[0];
		}

		template<>
		inline const float SSE_Vec_Dot<float, 4>(const float* in1, const float* in2) {
			__m128 x1 = _mm_load_ps(&in1[0]);
			__m128 y1 = _mm_load_ps(&in1[1]);
			__m128 z1 = _mm_load_ps(&in1[2]);
			__m128 w1 = _mm_load_ps(&in1[3]);
			__m128 x2 = _mm_load_ps(&in2[0]);
			__m128 y2 = _mm_load_ps(&in2[1]);
			__m128 z2 = _mm_load_ps(&in2[2]);
			__m128 w2 = _mm_load_ps(&in2[3]);
			__m128 out = _mm_add_ss(_mm_mul_ss(x1, x2), _mm_add_ss(_mm_mul_ss(y1, y2), _mm_add_ss(_mm_mul_ss(z1, z2), _mm_mul_ss(w1, w2))));
			return out.m128_f32[0];
		}

		template<>
		inline const int SSE_Vec_Dot<int, 4>(const int* in1, const int* in2) {
			__m128i x1 = _mm_loadu_si32(&in1[0]);
			__m128i y1 = _mm_loadu_si32(&in1[1]);
			__m128i z1 = _mm_loadu_si32(&in1[2]);
			__m128i w1 = _mm_loadu_si32(&in1[3]);
			__m128i x2 = _mm_loadu_si32(&in2[0]);
			__m128i y2 = _mm_loadu_si32(&in2[1]);
			__m128i z2 = _mm_loadu_si32(&in2[2]);
			__m128i w2 = _mm_loadu_si32(&in2[3]);
			__m128i out = _mm_add_epi32(_mm_mullo_epi32(x1, x2), _mm_add_epi32(_mm_mullo_epi32(y1, y2), _mm_add_epi32(_mm_mullo_epi32(z1, z2), _mm_mullo_epi32(w1, w2))));
			return out.m128i_i32[0];
		}

		// -------------------------------------------- Vector Normalize ----------------------------------------------- //
		// Much faster than scalar operations.
		
		template<uint32_t N>
		inline const float* SSE_Vec_Normalize(const float* in);

		template<>
		inline const float* SSE_Vec_Normalize<2>(const float* in) {
			__m128 x = _mm_load1_ps(&in[0]);
			__m128 y = _mm_load1_ps(&in[1]);
			__m128 isqrt = _mm_rsqrt_ps(_mm_add_ps(_mm_mul_ps(x, x), _mm_mul_ps(y, y)));
			__m128 temp = _mm_loadu_ps(in);
			__m128 out = _mm_mul_ps(temp, isqrt);
			return out.m128_f32;
		}

		template<>
		inline const float* SSE_Vec_Normalize<3>(const float* in) {
			__m128 x = _mm_load1_ps(&in[0]);
			__m128 y = _mm_load1_ps(&in[1]);
			__m128 z = _mm_load1_ps(&in[2]);
			__m128 isqrt = _mm_rsqrt_ps(_mm_add_ps(_mm_mul_ps(x, x), _mm_add_ps(_mm_mul_ps(y, y), _mm_mul_ps(z, z))));
			__m128 temp = _mm_loadu_ps(in);
			__m128 out = _mm_mul_ps(temp, isqrt);
			return out.m128_f32;
		}

		template<>
		inline const float* SSE_Vec_Normalize<4>(const float* in) {
			__m128 x = _mm_load1_ps(&in[0]);
			__m128 y = _mm_load1_ps(&in[1]);
			__m128 z = _mm_load1_ps(&in[2]);
			__m128 w = _mm_load1_ps(&in[3]);
			__m128 isqrt = _mm_rsqrt_ps(_mm_add_ps(_mm_mul_ps(x, x), _mm_add_ps(_mm_mul_ps(y, y), _mm_add_ps(_mm_mul_ps(z, z), _mm_mul_ps(w, w)))));
			__m128 temp = _mm_loadu_ps(in);
			__m128 out = _mm_mul_ps(temp, isqrt);
			return out.m128_f32;
		}
		
		// ------------------------------------------- Matrix Determinant ---------------------------------------------- //
		// Same speed as normal except for 2x2.

		template<class T, uint32_t N>
		inline const T SSE_Mat_Determinant(const T* in);

		template<>
		inline const float SSE_Mat_Determinant<float, 2>(const float* in) {
			__m128 a11 = _mm_load_ss(&in[0]);
			__m128 a12 = _mm_load_ss(&in[1]);
			__m128 a21 = _mm_load_ss(&in[2]);
			__m128 a22 = _mm_load_ps(&in[3]);
			__m128 a = _mm_mul_ss(a11, a22);
			__m128 b = _mm_mul_ss(a12, a21);
			__m128 out = _mm_sub_ss(a, b);
			return out.m128_f32[0];
		}

		template<>
		inline const float SSE_Mat_Determinant<float, 3>(const float* in) {
			__m128 a11 = _mm_load_ss(&in[0]);
			__m128 a12 = _mm_load_ss(&in[1]);
			__m128 a13 = _mm_load_ss(&in[2]);
								  
			__m128 a21 = _mm_load_ss(&in[3]);
			__m128 a22 = _mm_load_ss(&in[4]);
			__m128 a23 = _mm_load_ss(&in[5]);
								  
			__m128 a31 = _mm_load_ss(&in[6]);
			__m128 a32 = _mm_load_ss(&in[7]);
			__m128 a33 = _mm_load_ss(&in[8]);

			__m128 m1 = _mm_mul_ss(a22, a33);
			__m128 m2 = _mm_mul_ss(a23, a32);
			__m128 m3 = _mm_mul_ss(a21, a33);
			__m128 m4 = _mm_mul_ss(a23, a31);
			__m128 m5 = _mm_mul_ss(a21, a32);
			__m128 m6 = _mm_mul_ss(a22, a31);

			__m128 a = _mm_sub_ss(m1, m2);
			__m128 b = _mm_sub_ss(m3, m4);
			__m128 c = _mm_sub_ss(m5, m6);

			__m128 out = _mm_add_ss(_mm_sub_ss(_mm_mul_ss(a11, a), _mm_mul_ss(a12, b)), _mm_mul_ss(a13, c));
			return out.m128_f32[0];
		}

		template<>
		inline const float SSE_Mat_Determinant<float, 4>(const float* in) {
			__m128 a11 = _mm_load_ss(&in[0]);
			__m128 a12 = _mm_load_ss(&in[1]);
			__m128 a13 = _mm_load_ss(&in[2]);
			__m128 a14 = _mm_load_ss(&in[3]);
			
			__m128 a21 = _mm_load_ss(&in[4]);
			__m128 a22 = _mm_load_ss(&in[5]);
			__m128 a23 = _mm_load_ss(&in[6]);
			__m128 a24 = _mm_load_ss(&in[7]);
			
			__m128 a31 = _mm_load_ss(&in[8]);
			__m128 a32 = _mm_load_ss(&in[9]);
			__m128 a33 = _mm_load_ss(&in[10]);
			__m128 a34 = _mm_load_ss(&in[11]);
			
			__m128 a41 = _mm_load_ss(&in[12]);
			__m128 a42 = _mm_load_ss(&in[13]);
			__m128 a43 = _mm_load_ss(&in[14]);
			__m128 a44 = _mm_load_ss(&in[15]);

			__m128 det1 = _mm_sub_ss(_mm_mul_ss(a31, a42), _mm_mul_ss(a41, a32)); // |XX##|
			__m128 det2 = _mm_sub_ss(_mm_mul_ss(a32, a43), _mm_mul_ss(a42, a33)); // |#XX#|
			__m128 det3 = _mm_sub_ss(_mm_mul_ss(a33, a44), _mm_mul_ss(a43, a34)); // |##XX|
			__m128 det4 = _mm_sub_ss(_mm_mul_ss(a31, a43), _mm_mul_ss(a41, a33)); // |X#X#|
			__m128 det5 = _mm_sub_ss(_mm_mul_ss(a32, a44), _mm_mul_ss(a42, a34)); // |#X#X|
			__m128 det6 = _mm_sub_ss(_mm_mul_ss(a31, a44), _mm_mul_ss(a41, a34)); // |X##X|
			
			__m128 a = _mm_add_ss(_mm_sub_ss(_mm_mul_ss(a22, det3), _mm_mul_ss(a23, det5)), _mm_mul_ss(a24, det2));
			__m128 b = _mm_add_ss(_mm_sub_ss(_mm_mul_ss(a21, det3), _mm_mul_ss(a23, det6)), _mm_mul_ss(a24, det4));
			__m128 c = _mm_add_ss(_mm_sub_ss(_mm_mul_ss(a21, det5), _mm_mul_ss(a22, det6)), _mm_mul_ss(a24, det1));
			__m128 d = _mm_add_ss(_mm_sub_ss(_mm_mul_ss(a21, det2), _mm_mul_ss(a22, det4)), _mm_mul_ss(a23, det1));

			__m128 out = _mm_add_ss(_mm_sub_ss(_mm_mul_ss(a11, a), _mm_mul_ss(a12, b)), _mm_sub_ss(_mm_mul_ss(a13, c), _mm_mul_ss(a14, d)));
			return out.m128_f32[0];
		}
		#endif
	}
}