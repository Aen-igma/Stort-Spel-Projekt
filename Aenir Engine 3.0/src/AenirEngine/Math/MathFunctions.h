#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//                               /\               |  ____|              | | (_)                                          //
//                              /  \   ___ _ __   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___                           //
//                             / /\ \ / _ \ '_ \  |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|                          //
//                            / ____ \  __/ | | | | |  | |_| | | | | (__| |_| | (_) | | | \__ \                          //
//                           /_/    \_\___|_| |_| |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/                          //
// --------------------------------------------------------------------------------------------------------------------- //

#include<type_traits>
#include<iostream>
#include<sstream>
#include<cmath>
#include"Simd.h"

namespace Aen {

	// ----------------------------------------------- Forward Declarations -------------------------------------------- //

	namespace Concealed {

		template<class T, uint32_t N> struct TVec;
		template<class T, uint32_t N> struct TVecN;
		template<class T, uint32_t R, uint32_t C> struct TMat;
		template<class T, uint32_t R, uint32_t C> struct TMatN;
		template<class T> struct TQuat;
		template<class T> struct TRect;
		template<class T> struct TCircle;
		template<class T> class TTracer;
	}

	// ----------------------------------------------------- Constants ------------------------------------------------- //

	constexpr double C_PI(3.14159265358979323846);
	constexpr double C_2xPI(6.28318530717958647692);
	constexpr double C_PI2(1.57079632679489661923);
	constexpr double C_PI4(0.785398163397448309616);
	constexpr double C_1PI(0.318309886183790671538);
	constexpr double C_2PI(0.636619772367581343076);
	constexpr double C_2SqrtPI(1.12837916709551257390);
	constexpr double C_Sqrt2(1.41421356237309504880);
	constexpr double C_1Sqrt2(0.707106781186547524401);
	constexpr double C_E(2.71828182845904523536);
	constexpr double C_Log2E(1.44269504088896340736);
	constexpr double C_Log10E(0.434294481903251827651);
	constexpr double C_LogE2(0.693147180559945309417);
	constexpr double C_LogE10(2.30258509299404568402);
	constexpr double C_180dPI(57.2957795130823208767);
	constexpr double C_1d180xPI(0.01745329251994329576);

	// ----------------------------------------------- General Math Functions ------------------------------------------ //

	template<class T> 
	inline const T Sqrd(const T& n) noexcept {
		return n * n;
	}

	template<class T> 
	inline const int Sign(const T& val) noexcept {
		return static_cast<T>((0 < val) - (val < 0));
	}

	template<class T> 
	inline const T Lerp(const T& a, const T& b, const float& t) noexcept {
		return a * (1.f - t) + b * t;
	}

	template<class T> 
	inline const T RadToDeg(const T& value) noexcept {
		return value * (T)C_180dPI;
	}

	template<class T> 
	inline const T DegToRad(const T& value) noexcept {
		return value * (T)C_1d180xPI;
	}

	template<class T> 
	inline void Clamp(T& value, const T& min, const T& max) noexcept {
		value = (value < min) ? min : (value > max) ? max : value;
	}

	template<class T> 
	inline const T Min(const T& a, const T& b) noexcept {
		return (a < b) ? a : b;
	}

	template<class T> 
	inline const T Max(const T& a, const T& b) noexcept {
		return (a > b) ? a : b;
	}

	template<class T> 
	inline const T Abs(const T& num) noexcept {
		return std::abs(num);
	}

	template<class T> 
	inline void Swap(T& a, T& b) noexcept {
		T temp = std::move(a); 
		a = std::move(b);
		b = std::move(temp);
	}

	// ----------------------------------------------- Vector Math Functions ------------------------------------------ //
	
	template<class T> 
	inline const Concealed::TVec<T, 3> Cross(const Concealed::TVec<T, 3>& u, const Concealed::TVec<T, 3>& v) noexcept {
		return u % v;
	}

	template<class T, int32_t N> 
	inline const Concealed::TVec<T, N> Dot(const Concealed::TVec<T, N>& u, const Concealed::TVec<T, N>& v) noexcept {
		return u * v;
	}

	template<class T, int32_t N> 
	inline const Concealed::TVec<T, N> Lerp(const Concealed::TVec<T, N>& a, const Concealed::TVec<T, N>& b, const float& t) noexcept {
		Concealed::TVec<T, N> out;
		for(int32_t i = 0; i < N; i++) out[i] = Lerp(a[i], b[i], t);
		return out;
	}

	template<class T, int32_t N> 
	inline void Clamp(Concealed::TVec<T, N>& value, const Concealed::TVec<T, N>& min, const Concealed::TVec<T, N>& max) noexcept {
		Concealed::TVec<T, N> out;
		for(int32_t i = 0; i < N; i++) out[i] = Clamp(value[i], min[i], max[i]);
		return out;
	}

	template<class T, int32_t N> 
	inline const Concealed::TVec<T, N> RadToDeg(const Concealed::TVec<T, N>& value) noexcept {
		Concealed::TVec<T, N> out;
		for(int32_t i = 0; i < N; i++) out[i] = RadToDeg(value[i]);
		return out;
	}

	template<class T, int32_t N> 
	inline const Concealed::TVec<T, N> DegToRad(const Concealed::TVec<T, N>& value) noexcept {
		Concealed::TVec<T, N> out;
		for(int32_t i = 0; i < N; i++) out[i] = DegToRad(value[i]);
		return out;
	}

	// ----------------------------------------------- Matrix Math Functions ------------------------------------------ //

	namespace Concealed {

		template<class T, uint32_t R, uint32_t C, uint32_t N>
		inline const Concealed::TVec<T, N> Transform(const Concealed::TMat<T, R, C>& m, const Concealed::TVec<T, N>& vec, std::true_type) noexcept {
			Concealed::TVec<T, N> out;
			for(uint32_t i = 0; i < R; i++) for(uint32_t j = 0; j < C; j++) out[i] += m[i][j] * vec[j];
			return out;
		}

		template<class T, uint32_t R, uint32_t C, uint32_t N>
		inline const Concealed::TVec<T, N> Transform(const Concealed::TMat<T, R, C>& m, const Concealed::TVec<T, N>& vec, std::false_type) noexcept {
			Concealed::TVec<T, N> out;
			Concealed::TVec<T, C> temp;
			Concealed::TVec<T, C> vecT(vec, (T)1);
			for(uint32_t i = 0; i < R; i++) for(uint32_t j = 0; j < C; j++) temp[i] += m[i][j] * vecT[j];
			for(uint32_t i = 0; i < N; i++) out[i] = temp[i] / temp[C - 1];
			return out;
		}
	}

	template<class T, uint32_t R, uint32_t C, uint32_t N>
	inline const Concealed::TVec<T, N> Transform(const Concealed::TMat<T, R, C>& m, const Concealed::TVec<T, N>& vec) noexcept {
		return Concealed::Transform(m, vec, std::conditional_t <N == C, std::true_type, std::false_type>());
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatTranslate(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TMat<T, 4, 4> out;
		out.a41 = t.x;
		out.a42 = t.y;
		out.a43 = t.z;
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatTranslate(const T& x, const T& y, const T& z) noexcept {
		Concealed::TMat<T, 4, 4> out;
		out.a41 = x;
		out.a42 = y;
		out.a43 = z;
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatRotate(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TVec<T, 3> ang(DegToRad(t));
		Concealed::TMat<T, 4, 4> out;
		out.a11 = std::cos(ang.z) * std::cos(ang.y);
		out.a12 = std::cos(ang.z) * std::sin(ang.y) * std::sin(ang.x) - std::sin(ang.z) * std::cos(ang.x);
		out.a13 = std::cos(ang.z) * std::sin(ang.y) * std::cos(ang.x) + std::sin(ang.z) * std::sin(ang.x);
		out.a21 = std::sin(ang.z) * std::cos(ang.y);
		out.a22 = std::sin(ang.z) * std::sin(ang.y) * std::sin(ang.x) + std::cos(ang.z) * std::cos(ang.x);
		out.a23 = std::sin(ang.z) * std::sin(ang.y) * std::cos(ang.x) - std::cos(ang.z) * std::sin(ang.x);
		out.a31 = -std::sin(ang.y);
		out.a32 = std::cos(ang.y) * std::sin(ang.x);
		out.a33 = std::cos(ang.y) * std::cos(ang.x);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatRotate(const T& x, const T& y, const T& z) noexcept {
		Concealed::TVec<T, 3> ang(x, y, z);
		ang = DegToRad(ang);
		Concealed::TMat<T, 4, 4> out;
		out.a11 = std::cos(ang.z) * std::cos(ang.y);
		out.a12 = std::cos(ang.z) * std::sin(ang.y) * std::sin(ang.x) - std::sin(ang.z) * std::cos(ang.x);
		out.a13 = std::cos(ang.z) * std::sin(ang.y) * std::cos(ang.x) + std::sin(ang.z) * std::sin(ang.x);
		out.a21 = std::sin(ang.z) * std::cos(ang.y);
		out.a22 = std::sin(ang.z) * std::sin(ang.y) * std::sin(ang.x) + std::cos(ang.z) * std::cos(ang.x);
		out.a23 = std::sin(ang.z) * std::sin(ang.y) * std::cos(ang.x) - std::cos(ang.z) * std::sin(ang.x);
		out.a31 = -std::sin(ang.y);
		out.a32 = std::cos(ang.y) * std::sin(ang.x);
		out.a33 = std::cos(ang.y) * std::cos(ang.x);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatRotateRad(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TMat<T, 4, 4> out;
		out.a11 = std::cos(t.z) * std::cos(t.y);
		out.a12 = std::cos(t.z) * std::sin(t.y) * std::sin(t.x) - std::sin(t.z) * std::cos(t.x);
		out.a13 = std::cos(t.z) * std::sin(t.y) * std::cos(t.x) + std::sin(t.z) * std::sin(t.x);
		out.a21 = std::sin(t.z) * std::cos(t.y);
		out.a22 = std::sin(t.z) * std::sin(t.y) * std::sin(t.x) + std::cos(t.z) * std::cos(t.x);
		out.a23 = std::sin(t.z) * std::sin(t.y) * std::cos(t.x) - std::cos(t.z) * std::sin(t.x);
		out.a31 = -std::sin(t.y);
		out.a32 = std::cos(t.y) * std::sin(t.x);
		out.a33 = std::cos(t.y) * std::cos(t.x);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatRotateRad(const T& x, const T& y, const T& z) noexcept {
		Concealed::TMat<T, 4, 4> out;
		out.a11 = std::cos(z) * std::cos(y);
		out.a12 = std::cos(z) * std::sin(y) * std::sin(x) - std::sin(z) * std::cos(x);
		out.a13 = std::cos(z) * std::sin(y) * std::cos(x) + std::sin(z) * std::sin(x);
		out.a21 = std::sin(z) * std::cos(y);
		out.a22 = std::sin(z) * std::sin(y) * std::sin(x) + std::cos(z) * std::cos(x);
		out.a23 = std::sin(z) * std::sin(y) * std::cos(x) - std::cos(z) * std::sin(x);
		out.a31 = -std::sin(y);
		out.a32 = std::cos(y) * std::sin(x);
		out.a33 = std::cos(y) * std::cos(x);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatScale(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TMat<T, 4, 4> out;
		out.a11 = t.x;
		out.a22 = t.y;
		out.a33 = t.z;
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatScale(const T& x, const T& y, const T& z) noexcept {
		Concealed::TMat<T, 4, 4> out;
		out.a11 = x;
		out.a22 = y;
		out.a33 = z;
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> 
		MatPerspective(const float& fov, const float& aspect, const float& min, const float& max) noexcept {
		float ang = DegToRad(fov);
		float yScale = 1.f / tan(ang * 0.5f);
		float xScale = yScale / aspect;
		Concealed::TMat<T, 4, 4> out;
		out.a11 = xScale;
		out.a22 = yScale;
		out.a33 = max / (max - min);
		out.a34 = 1;
		out.a43 = -min * max / (max - min);
		out.a44 = 0;
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> 
		MatOrthographic(const float& l, const float& r, const float& t, const float& b, const float& n, const float& f) noexcept {
		Concealed::TMat<T, 4, 4> out;
		out.a11 = (T)2 / (r - l);
		out.a22 = (T)2 / (t - b);
		out.a33 = (T)-1 / (f - n);
		out.a43 = n / (n - f);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> 
		MatViewLH(const Concealed::TVec<T, 3>& pos, const Concealed::TVec<T, 3>& target, const Concealed::TVec<T, 3>& up) noexcept {
		Concealed::TMat<T, 3, 3> dir;
		dir.z = (target - pos).Normalized();
		dir.x = Cross(up, dir.z).Normalized();
		dir.y = Cross(dir.z, dir.x).Normalized();
		Concealed::TMat<T, 4, 4> out;
		out = dir.Transposed();
		out.a41 = -(dir.x * pos);
		out.a42 = -(dir.y * pos);
		out.a43 = -(dir.z * pos);
		return out;
	}

	template<class T>
	inline const Concealed::TVec<T, 3> MatRotationToEuler(const Concealed::TMat<T, 4, 4>& mat) noexcept {
		T p, y, r;
		T sy = std::sqrt(Sqrd(mat.a32) + Sqrd(mat.a33));
		if(sy > 0) {
			p = std::atan2f(mat.a32, mat.a33);
			y = std::atan2f(-mat.a31, sy);
			r = std::atan2f(mat.a21, mat.a11);
		} else {
			p = std::atan2f(-mat.a23, mat.a22);
			y = std::atan2f(-mat.a31, sy);
			r = 0;
		}
		return Concealed::TVec<T, 3>(Aen::RadToDeg(p), Aen::RadToDeg(y), Aen::RadToDeg(r));
	}

	template<class T>
	inline const Concealed::TVec<T, 3> MatRotationToEuler(const Concealed::TMat<T, 3, 3>& mat) noexcept {
		T p, y, r;
		T sy = std::sqrt(Sqrd(mat.a32) + Sqrd(mat.a33));
		if(sy > 0) {
			p = std::atan2f(mat.a32, mat.a33);
			y = std::atan2f(-mat.a31, sy);
			r = std::atan2f(mat.a21, mat.a11);
		} else {
			p = std::atan2f(-mat.a23, mat.a22);
			y = std::atan2f(-mat.a31, sy);
			r = 0;
		}
		return Concealed::TVec<T, 3>(Aen::RadToDeg(p), Aen::RadToDeg(y), Aen::RadToDeg(r));
	}

	// ----------------------------------------------- Quaternion Math Functions ------------------------------------------- //

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatQuaternion(const Concealed::TVec<T, 4>& quat) noexcept {
		Concealed::TQuat<T> q;
		q = quat.Normalized();
		Concealed::TMat<T, 4, 4> out;
		out.a11 = (T)2 * (q.x * q.x + q.y * q.y) - (T)1;
		out.a12 = (T)2 * (q.y * q.z - q.x * q.w);
		out.a13 = (T)2 * (q.y * q.w + q.x * q.z);
		out.a21 = (T)2 * (q.y * q.z + q.x * q.w);
		out.a22 = (T)2 * (q.x * q.x + q.z * q.z) - (T)1;
		out.a23 = (T)2 * (q.z * q.w - q.x * q.y);
		out.a31 = (T)2 * (q.y * q.w - q.x * q.z);
		out.a32 = (T)2 * (q.z * q.w + q.x * q.y);
		out.a33 = (T)2 * (q.x * q.x + q.w * q.w) - (T)1;
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T, 4, 4> MatQuaternion(const Concealed::TQuat<T>& quat) noexcept {
		Concealed::TQuat<T> q;
		q = quat.Normalized();
		Concealed::TMat<T, 4, 4> out;
		out.a11 = (T)2 * (q.x * q.x + q.y * q.y) - (T)1;
		out.a12 = (T)2 * (q.y * q.z - q.x * q.w);
		out.a13 = (T)2 * (q.y * q.w + q.x * q.z);
		out.a21 = (T)2 * (q.y * q.z + q.x * q.w);
		out.a22 = (T)2 * (q.x * q.x + q.z * q.z) - (T)1;
		out.a23 = (T)2 * (q.z * q.w - q.x * q.y);
		out.a31 = (T)2 * (q.y * q.w - q.x * q.z);
		out.a32 = (T)2 * (q.z * q.w + q.x * q.y);
		out.a33 = (T)2 * (q.x * q.x + q.w * q.w) - (T)1;
		return out;
	}

	template<class T>
	inline const Concealed::TQuat<T> AxisAngToQuat(const Concealed::TVec<T, 4>& axisAng) noexcept {
		T a(DegToRad(axisAng.w));
		Concealed::TVec<T, 3> v(axisAng);
		return Concealed::TQuat<T>(v.Normalized() * std::sin(a * 0.5), std::cos(a * 0.5));
	}

	template<class T>
	inline const Concealed::TQuat<T> AxisAngToQuat(const T& x, const T& y, const T& z, const T& w) noexcept {
		T a(DegToRad(w));
		Concealed::TVec<T, 3> v(x, y, z);
		return Concealed::TQuat<T>(v.Normalized() * (T)std::sin(a * 0.5), (T)std::cos(a * 0.5));
	}

	template<class T>
	inline const Concealed::TQuat<T> AxisAngToQuat(const Concealed::TVec<T, 3>& axis, const T& ang) noexcept {
		T a(DegToRad(ang));
		return Concealed::TQuat<T>(axis.Normalized() * std::sin(a * 0.5), std::cos(a * 0.5));
	}

	template<class T> 
	inline const Concealed::TQuat<T> Slerp(const Concealed::TQuat<T>& q1, const Concealed::TQuat<T>& q2, const float& t) noexcept {
		
		T theta = std::abs(std::acos(static_cast<Concealed::TVec<T, 4>>(q1) * static_cast<Concealed::TVec<T, 4>>(q2)));

		T st = (T)std::sin(theta);
		T sut = (T)std::sin(t * theta);
		T sout = (T)std::sin(((T)1 - t) * theta);
		T coeff1 = sout / st;
		T coeff2 = sut / st;

		Concealed::TQuat<T> q;
		for(uint32_t i = 0; i < 4; i++) q[i] = coeff1 * q1[i] + coeff2 * q2[i];

		return q;
	}

	template<class T> 
	inline const Concealed::TQuat<T> Slerp(const Concealed::TVec<T, 4>& q1, const Concealed::TVec<T, 4>& q2, const float& t) noexcept {

		T theta = std::abs(std::acos(q1 * q2));

		T st = (T)std::sin(theta);
		T sut = (T)std::sin(t * theta);
		T sout = (T)std::sin(((T)1 - t) * theta);
		T coeff1 = sout / st;
		T coeff2 = sut / st;

		Concealed::TQuat<T> q;
		for(uint32_t i = 0; i < 4; i++) q[i] = coeff1 * q1[i] + coeff2 * q2[i];

		return q;
	}

	// ------------------------------------------------- Intersect Functions ----------------------------------------------- //

	template<class T> 
	inline const bool RangeOverlap(const T& a1, const T& a2, const T& b1, const T& b2) noexcept {
		return (a2 > b1 && b2 > a1);
	}

	template<class T> 
	inline const bool Intersect(Concealed::TTracer<T>& t, const Concealed::TRect<T>& r) noexcept {

		T tMin = -100000.f;
		T tMax = 100000.f;
		Concealed::TVec<T, 2> tOrigin = t.GetOrigin();
		Concealed::TVec<T, 2> tDir = t.GetDirection();
		Concealed::TVec<T, 2> bl = r.GetBL();
		Concealed::TVec<T, 2> tr = r.GetTR();

		for(int i = 0; i < 2; i++) {
			T t1 = (bl[i] - tOrigin[i]) / tDir[i];
			T t2 = (tr[i] - tOrigin[i]) / tDir[i];
			if(t1 > t2) Aen::Swap(t1, t2);
			tMin = Aen::Max(t1, tMin);
			tMax = Aen::Min(t2, tMax);
			if(tMin >= tMax || tMax < 0.f)
				return false;
		}

		if(tMin > 0.f) {
			*(T*)&t = tMin;
			return true;
		}

		return false;
	}

	template<class T> 
	inline const bool Intersect(const Concealed::TRect<T>& r1, const Concealed::TRect<T>& r2) noexcept {
		return RangeOverlap(r1.GetL(), r1.GetR(), r2.GetL(), r2.GetR()) && RangeOverlap(r1.GetB(), r1.GetT(), r2.GetB(), r1.GetT());
	}

	template<class T> 
	inline const bool Intersect(const Concealed::TVec<T, 2>& p, const Concealed::TRect<T>& r) noexcept {
		return p.x > r.GetL() && p.x < r.GetR() && p.y > r.GetB() && p.y < r.GetT();
	}

	template<class T> 
	inline const bool Intersect(Concealed::TTracer<T>& t, const Concealed::TCircle<T>& c) noexcept {
		
		T tDist = 100000.f;

		Concealed::TVec<T, 2> tOrigin = t.GetOrigin();
		Concealed::TVec<T, 2> tDir = t.GetDirection();

		T b = tDir * (tOrigin - c.origin);
		T r = Abs(c.radius);
		T a = (tOrigin - c.origin) * (tOrigin - c.origin) - (r * r);
		T q = (b * b) - a;
		
		if(q > 0) {
			tDist = -b - std::sqrt(q);
			if(tDist < 0) return false;
			*(T*)&t = tDist;
			return true;
		}

		return false;
	}

	template<class T> 
	inline const bool Intersect(const Concealed::TCircle<T>& c1, const Concealed::TCircle<T>& c2) noexcept {
		Concealed::TVec<T, 2> dist = c2.origin - c1.origin;
		return dist.Magnitude() < Abs(c1.radius) + Abs(c2.radius);
	}

	template<class T> 
	inline const bool Intersect(const Concealed::TVec<T, 2>& p, const Concealed::TCircle<T>& c) noexcept {
		Concealed::TVec<T, 2> dist = c.origin - p;
		return dist.Magnitude() < Abs(c.radius);
	}

	template<class T> 
	inline const bool Intersect(const Concealed::TRect<T>& r, const Concealed::TCircle<T>& c) noexcept {
		Concealed::TVec<T, 2> n;
		n.x = Max(r.GetL(), Min(c.origin.x, r.GetR()));
		n.y = Max(r.GetB(), Min(c.origin.y, r.GetT()));
		Concealed::TVec<T, 2> dist = c.origin - n;
		return dist.Magnitude() < Abs(c.radius);
	}

	// --------------------------------------------------------------------------------------------------------------------- //
}