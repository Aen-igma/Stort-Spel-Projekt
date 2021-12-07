#pragma once

#include"AenDefines.h"
#include"../../ThirdParty/SimpleMath/SimpleMath.h"
#include<DirectXMath.h>
#include<type_traits>
#include<iostream>
#include<sstream>
#include<cmath>

namespace Aen {

	namespace sm = DirectX::SimpleMath;

	// ----------------------------------------------- Forward Declarations -------------------------------------------- //

	namespace Concealed {

		template<class T, uint32_t N> struct TVec;
		template<class T, uint32_t N> struct TVecN;
		template<class T> struct TMat;
		template<class T, uint32_t R, uint32_t C> struct TMatN;
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
	inline const T Clamp(const T& value, const T& min, const T& max) noexcept {
		return (value < min) ? min : (value > max) ? max : value;
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
		out.smVec = out.smVec.Lerp(a.smVec, b.smVec, t);
		return out;
	}

	template<class T, int32_t N> 
	inline const Concealed::TVec<T, N> Clamp(Concealed::TVec<T, N>& value, const Concealed::TVec<T, N>& min, const Concealed::TVec<T, N>& max) noexcept {
		Concealed::TVec<T, N> out(value);
		out.smVec.Clamp(min.smVec, max.smVec);
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

	template<class T>
	inline const Concealed::TMat<T> Lerp(const Concealed::TMat<T>& a, const Concealed::TMat<T>& b, const float& t) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::Lerp(a.smMat, b.smMat, t);
		return out;
	}

	template<class T, uint32_t N>
	inline const Concealed::TVec<T, N> Transform(const Concealed::TMat<T>& m, const Concealed::TVec<T, 2>& vec) noexcept;

	template<class T, uint32_t N = 2>
	inline const Concealed::TVec<T, 2> Transform(const Concealed::TMat<T>& m, const Concealed::TVec<T, 2>& vec) noexcept {
		Concealed::TVec<T, 2> out;
		out.smVec = sm::Vector2::Transform(vec.smVec, m.smMat);
		return out;
	}

	template<class T, uint32_t N = 3>
	inline const Concealed::TVec<T, 3> Transform(const Concealed::TMat<T>& m, const Concealed::TVec<T, 3>& vec) noexcept {
		Concealed::TVec<T, 3> out;
		out.smVec = sm::Vector3::Transform(vec.smVec, m.smMat);
		return out;
	}

	template<class T, uint32_t N = 4>
	inline const Concealed::TVec<T, 4> Transform(const Concealed::TMat<T>& m, const Concealed::TVec<T, 4>& vec) noexcept {
		Concealed::TVec<T, 4> out;
		out.smVec = sm::Vector4::Transform(vec.smVec, m.smMat);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatTranslate(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateTranslation(t.smVec);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatTranslate(const T& x, const T& y, const T& z) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateTranslation(x, y, z);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatRotate(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TVec<T, 3> ang(DegToRad(t));
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateFromYawPitchRoll(ang.y, ang.x, ang.z);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatRotate(const T& x, const T& y, const T& z) noexcept {
		Concealed::TVec<T, 3> ang(x, y, z);
		ang = DegToRad(ang);
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateFromYawPitchRoll(ang.y, ang.x, ang.z);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatRotateRad(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateFromYawPitchRoll(t.y, t.x, t.z);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatRotateRad(const T& x, const T& y, const T& z) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateFromYawPitchRoll(y, x, z);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatScale(const Concealed::TVec<T, 3>& t) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateScale(t.smVec);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatScale(const T& x, const T& y, const T& z) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateScale(x, y, z);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> 
		MatPerspective(const float& fov, const float& aspect, const float& min, const float& max) noexcept {
		float ang = DegToRad(fov);
		Concealed::TMat<T> out;
		out.smMat = DirectX::XMMatrixPerspectiveFovLH(ang, aspect, min, max);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> 
		MatOrthographic(const float& l, const float& r, const float& t, const float& b, const float& n, const float& f) noexcept {
		Concealed::TMat<T> out;
		out.smMat = sm::Matrix::CreateOrthographicOffCenter(l, r, b, t, n, f);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> 
		MatViewLH(const Concealed::TVec<T, 3>& pos, const Concealed::TVec<T, 3>& target, const Concealed::TVec<T, 3>& up) noexcept {
		Concealed::TMat<T> out;
		out.smMat = DirectX::XMMatrixLookAtLH(pos.smVec, target.smVec, up.smVec);
		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> 
		MatViewRH(const Concealed::TVec<T, 3>& pos, const Concealed::TVec<T, 3>& target, const Concealed::TVec<T, 3>& up) noexcept {
		Concealed::TMat<T> out;
		out.smMat = DirectX::XMMatrixLookAtRH(pos.smVec, target.smVec, up.smVec);
		return out;
	}

	template<class T>
	inline const Concealed::TVec<T, 3> MatRotationToEuler(const Concealed::TMat<T>& mat) noexcept {
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
	inline const Concealed::TMat<T> MatQuaternion(const Concealed::TVec<T, 4>& quat) noexcept {

		Concealed::TMat<T> out;
		sm::Quaternion q(quat.smVec);
		out.smMat = sm::Matrix::CreateFromQuaternion(q);

		return out;
	}

	template<class T>
	inline const Concealed::TMat<T> MatQuaternion(const T& x, const T& y, const T& z, const T& w) noexcept {

		Concealed::TMat<T> out;
		sm::Quaternion q(x, y, z, w);
		out.smMat = sm::Matrix::CreateFromQuaternion(q);

		return out;
	}

	template<class T>
	inline const Concealed::TVec<T, 4> EulerToQuat(const T& x, const T& y, const T& z) noexcept {
		Concealed::TVec<T, 4> out;
		out.smVec = DirectX::XMQuaternionRotationRollPitchYaw(DegToRad(x), DegToRad(y), DegToRad(z));

		return out;
	}

	template<class T>
	inline const Concealed::TVec<T, 4> EulerToQuat(const Concealed::TVec<T, 3> rot) noexcept {
		Concealed::TVec<T, 4> out;
		Concealed::TVec<T, 3> tempRot = DegToRad(rot);
		out.smVec = DirectX::XMQuaternionRotationRollPitchYaw(tempRot.x, tempRot.y, tempRot.z);

		return out;
	}

	// --------------------------------------------------------------------------------------------------------------------- //
}