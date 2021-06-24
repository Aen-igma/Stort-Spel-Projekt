#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//                             /\                / __ \            | |                (_)            					 //
//                            /  \   ___ _ __   | |  | |_   _  __ _| |_ ___ _ __ _ __  _  ___  _ __  					 //
//                           / /\ \ / _ \ '_ \  | |  | | | | |/ _` | __/ _ \ '__| '_ \| |/ _ \| '_ \ 					 //
//                          / ____ \  __/ | | | | |__| | |_| | (_| | ||  __/ |  | | | | | (_) | | | |					 //
//                         /_/    \_\___|_| |_|  \___\_\\__,_|\__,_|\__\___|_|  |_| |_|_|\___/|_| |_|					 //
// --------------------------------------------------------------------------------------------------------------------- //

#include"MathFunctions.h"

namespace Aen {

	namespace Concealed {

		// ------------------------------------------------------------------------------------------------------------- //
		//                                                   Quaternion                                                  //
		// ------------------------------------------------------------------------------------------------------------- //

		template<class T>
		struct TQuat : public TVecN<T, 4> {

			using TVecN<T, 4>::x;
			using TVecN<T, 4>::y;
			using TVecN<T, 4>::z;
			using TVecN<T, 4>::w;

			TQuat() noexcept;
			TQuat(const TQuat& rhs) noexcept = default;
			TQuat(const T & t) noexcept;
			template <class ...Ts>
			TQuat(const T & t, const Ts& ...ts) noexcept;
			template <class ...Ts, uint32_t Ns>
			TQuat(const TVec<T, Ns>&t, const Ts& ...ts) noexcept;

			TQuat& operator= (const TQuat& rhs) noexcept;
			TQuat& operator= (const TVec<T, 4>& rhs) noexcept;
			TQuat& operator= (const T rhs[4]) noexcept;
			TQuat& operator+= (const TQuat& rhs) noexcept;
			TQuat& operator+= (const TVec<T, 4>& rhs) noexcept;
			TQuat& operator-= (const TQuat& rhs) noexcept;
			TQuat& operator-= (const TVec<T, 4>& rhs) noexcept;

			const TQuat& operator+ () const noexcept;
			const TQuat operator- () const noexcept;

			TQuat operator+ (const TQuat& rhs) const noexcept;
			TQuat operator+ (const TVec<T, 4>& rhs) const noexcept;
			TQuat operator- (const TQuat& rhs) const noexcept;
			TQuat operator- (const TVec<T, 4>& rhs) const noexcept;
			TQuat operator* (const T& rhs) const noexcept;
			TQuat operator* (const TQuat& rhs) const noexcept;
			TQuat operator* (const TVec<T, 4>& rhs) const noexcept;
			TVec<T, 3> operator* (const TVec<T, 3>& rhs) const noexcept;

			T& operator[] (const uint32_t& i) throw();
			const T& operator[] (const uint32_t& i) const throw();

			template<class T1> explicit operator TQuat<T1>() const noexcept;
			template<class T1> explicit operator TVec<T1, 4>() const noexcept;

			friend std::ostream& operator<<	(std::ostream& out, const TQuat& rhs) noexcept {
				out << "[";
				for(uint32_t i = 0; i < 3; i++) out << std::to_string(rhs[i]) + ", ";
				out << std::to_string(rhs[3]) + "]";
				return out;
			}

			const TVec<T, 3> GetVec() const noexcept;
			const T Magnitude() const noexcept;
			const TQuat Normalized() const noexcept;
			const TQuat Conjugate() const noexcept;
			const TQuat Inverse() const noexcept;

			static const TQuat<T> zero;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T>
		inline TQuat<T>::TQuat() noexcept {
			for(uint32_t i = 0; i < 4; i++) (*this)[i] = 0;
		}

		template<class T>
		inline TQuat<T>::TQuat(const T& t) noexcept {
			for(uint32_t i = 0; i < 4; i++) (*this)[i] = t;
		}

		template<class T>
		template<class ...Ts>
		inline TQuat<T>::TQuat(const T& t, const Ts & ...ts) noexcept
			:TVecN<T, 4>{t, ts...} {}

		template<class T>
		template<class ...Ts, uint32_t Ns>
		inline TQuat<T>::TQuat(const TVec<T, Ns>& t, const Ts & ...ts) noexcept
			:TVecN<T, 4>{t, ts...} {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T>
		inline TQuat<T>& TQuat<T>::operator=(const TQuat& rhs) noexcept {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		template<class T>
		inline TQuat<T>& TQuat<T>::operator=(const TVec<T, 4>& rhs) noexcept {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		template<class T>
		inline TQuat<T>& TQuat<T>::operator=(const T rhs[4]) noexcept {
			x = rhs[0];
			y = rhs[1];
			z = rhs[2];
			w = rhs[3];
			return *this;
		}

		template<class T>
		inline TQuat<T>& TQuat<T>::operator+=(const TQuat& rhs) noexcept {
			*this = *this + rhs;
			return *this;
		}

		template<class T>
		inline TQuat<T>& TQuat<T>::operator+=(const TVec<T, 4>& rhs) noexcept {
			*this = *this + rhs;
			return *this;
		}

		template<class T>
		inline TQuat<T>& TQuat<T>::operator-=(const TQuat& rhs) noexcept {
			*this = *this - rhs;
			return *this;
		}

		template<class T>
		inline TQuat<T>& TQuat<T>::operator-=(const TVec<T, 4>& rhs) noexcept {
			*this = *this - rhs;
			return *this;
		}

		// -------------------------------------------- Unary Operators ------------------------------------------------ //
		
		template<class T>
		inline const TQuat<T>& TQuat<T>::operator+() const noexcept {
			return *this;
		}

		template<class T>
		inline const TQuat<T> TQuat<T>::operator-() const noexcept {
			return TQuat(-x, -y, -z, -w);
		}

		// ------------------------------------------ Arithmetic Operators --------------------------------------------- //

		template<class T>
		inline TQuat<T> TQuat<T>::operator+(const TQuat& rhs) const noexcept {
			TQuat out;
			out.x = x + rhs.x;
			out.y = y + rhs.y;
			out.z = z + rhs.z;
			out.w = w + rhs.w;
			return out;
		}

		template<class T>
		inline TQuat<T> TQuat<T>::operator+(const TVec<T, 4>& rhs) const noexcept {
			TQuat out;
			out.x = x + rhs.x;
			out.y = y + rhs.y;
			out.z = z + rhs.z;
			out.w = w + rhs.w;
			return out;
		}

		template<class T>
		inline TQuat<T> TQuat<T>::operator-(const TQuat& rhs) const noexcept {
			TQuat out;
			out.x = x - rhs.x;
			out.y = y - rhs.y;
			out.z = z - rhs.z;
			out.w = w - rhs.w;
			return out;
		}

		template<class T>
		inline TQuat<T> TQuat<T>::operator-(const TVec<T, 4>& rhs) const noexcept {
			TQuat out;
			out.x = x - rhs.x;
			out.y = y - rhs.y;
			out.z = z - rhs.z;
			out.w = w - rhs.w;
			return out;
		}

		template<class T>
		inline TQuat<T> TQuat<T>::operator*(const T& rhs) const noexcept {
			TQuat out;
			out.x = x * rhs;
			out.y = y * rhs;
			out.z = z * rhs;
			out.w = w * rhs;
			return out;
		}

		template<class T>
		inline TQuat<T> TQuat<T>::operator*(const TQuat& rhs) const noexcept {
			const TVec<T, 3> rv(rhs.GetVec()); 
			const TVec<T, 3> v(GetVec()); 
			return TQuat(rv * w + v * rhs.w + v % rv, w * rhs.w - v * rv);
		}

		template<class T>
		inline TQuat<T> TQuat<T>::operator*(const TVec<T, 4>& rhs) const noexcept {
			TVec<T, 3> rv(static_cast<TVec<T, 3>>(rhs));
			TVec<T, 3> v(GetVec());
			return TQuat(rv * w + v * rhs.w + v % rv, w * rhs.w - v * rv);
		}

		template<class T>
		inline TVec<T, 3> TQuat<T>::operator*(const TVec<T, 3>& rhs) const noexcept {
			TQuat p(rhs, 0);
			return (*this * p * Inverse()).GetVec();
		}

		// ----------------------------------------- Member Access Operators ------------------------------------------- //

		template<class T>
		inline T& TQuat<T>::operator[](const uint32_t& i) throw() {
			if(i < 4) return reinterpret_cast<T*>(this)[i];
		}

		template<class T>
		inline const T& TQuat<T>::operator[](const uint32_t& i) const throw() {
			if(i < 4) return reinterpret_cast<const T*>(this)[i];
		}

		// ------------------------------------------- Vector Operations ----------------------------------------------- //

		template<class T>
		inline const TVec<T, 3> TQuat<T>::GetVec() const noexcept {
			return TVec<T, 3>(x, y, z);
		}

		template<class T>
		inline const T TQuat<T>::Magnitude() const noexcept {
			return std::sqrt(x * x + y * y + z * z + w * w);
		}

		template<class T>
		inline const TQuat<T> TQuat<T>::Normalized() const noexcept {
			T mag = Magnitude();
			T iSqrt = (mag != 0) ? ((T)1 / mag) : 0.f;
			return TQuat(x * iSqrt, y * iSqrt, z * iSqrt, w * iSqrt);
		}

		template<class T>
		inline const TQuat<T> TQuat<T>::Conjugate() const noexcept {
			return TQuat(-x, -y, -z, w);
		}

		template<class T>
		inline const TQuat<T> TQuat<T>::Inverse() const noexcept {
			T mag = Magnitude();
			T iSqMag = (mag != 0) ? (T)1 / Sqrd(Magnitude()) : 0;
			return Conjugate() * mag;
		}

		// -------------------------------------------- Cast Operators ------------------------------------------------- //

		template<class T>
		template<class T1>
		inline TQuat<T>::operator TVec<T1, 4>() const noexcept {
			TVec<T1, 4> out;
			for(uint32_t i = 0; i < 4; i++) out[i] = static_cast<T1>((*this)[i]);
			return out;
		}

		template<class T>
		template<class T1>
		inline TQuat<T>::operator TQuat<T1>() const noexcept {
			TQuat<T1> out;
			for(uint32_t i = 0; i < 4; i++) out[i] = static_cast<T1>((*this)[i]);
			return out;
		}

		// ------------------------------------------- Constant Variables ---------------------------------------------- //

		template<class T>
		const TQuat<T> TQuat<T>::zero(0);
	}

	// ------------------------------------------------------- Type Alias ---------------------------------------------- //

	using Quatd = Concealed::TQuat<double>;
	using Quatf = Concealed::TQuat<float>;

	// ----------------------------------------------------------------------------------------------------------------- //
}