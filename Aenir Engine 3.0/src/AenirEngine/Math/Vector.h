#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//                                       /\               \ \    / /      | |                                            //
//                                      /  \   ___ _ __    \ \  / /__  ___| |_ ___  _ __                                 //
//                                     / /\ \ / _ \ '_ \    \ \/ / _ \/ __| __/ _ \| '__|                                //
//                                    / ____ \  __/ | | |    \  /  __/ (__| || (_) | |                                   //
//                                   /_/    \_\___|_| |_|     \/ \___|\___|\__\___/|_|                                   //
// --------------------------------------------------------------------------------------------------------------------- //

#include"MathFunctions.h"

namespace Aen {

	namespace Concealed {

		// ---------------------------------------------- Vector Container --------------------------------------------- //

		template<class T>
		struct TVecN<T, 2> {
			TVecN<T, 2>() :x(0), y(0) {}
			TVecN<T, 2>(const T& x, const T& y) :x(x), y(y) {}
			TVecN<T, 2>(const TVecN<T, 3>& rhs) :x(rhs.x), y(rhs.y) {}
			union {
				struct {T x, y;};
				struct {T r, g;};
			};
		};

		template<class T>
		struct TVecN<T, 3> {
			TVecN<T, 3>() :x(0), y(0), z(0) {}
			TVecN<T, 3>(const T& x, const T& y, const T& z) :x(x), y(y), z(z) {}
			TVecN<T, 3>(const TVecN<T, 2>& rhs, const T& z) :x(rhs.x), y(rhs.y), z(z) {}
			TVecN<T, 3>(const T& x, const TVecN<T, 2>& rhs) :x(x), y(rhs.x), z(rhs.y) {}
			TVecN<T, 3>(const TVecN<T, 4>& rhs) :x(rhs.x), y(rhs.y), z(rhs.z) {}
			union {
				struct {T x, y, z;};
				struct {T r, g, b;};
			};
		};

		template<class T>
		struct TVecN<T, 4> {
			TVecN<T, 4>() :x(0), y(0), z(0), w(0) {}
			TVecN<T, 4>(const T& x, const T& y, const T& z, const T& w) :x(x), y(y), z(z), w(w) {}
			TVecN<T, 4>(const TVecN<T, 2>& rhs1, const TVecN<T, 2>& rhs2) :x(rhs1.x), y(rhs1.y), z(rhs2.x), w(rhs2.y) {}
			TVecN<T, 4>(const TVecN<T, 2>& rhs, const T& z, const T& w) :x(rhs.x), y(rhs.y), z(z), w(w) {}
			TVecN<T, 4>(const T& x, const TVecN<T, 2>& rhs, const T& w) :x(x), y(rhs.x), z(rhs.y), w(w) {}
			TVecN<T, 4>(const T& x, const T& y,const TVecN<T, 2>& rhs) :x(x), y(y), z(rhs.x), w(rhs.y) {}
			TVecN<T, 4>(const TVecN<T, 3>& rhs, const T& w) :x(rhs.x), y(rhs.y), z(rhs.z), w(w) {}
			TVecN<T, 4>(const T& x, const TVecN<T, 3>& rhs) :x(x), y(rhs.x), z(rhs.y), w(rhs.z) {}
			union {
				struct {T x, y, z, w;};
				struct {T r, g, b, a;};
				struct {T h, s, v, a;};
			};
		};

		// ------------------------------------------------------------------------------------------------------------- //
		//                                                     Vector                                                    //
		// ------------------------------------------------------------------------------------------------------------- //

		template<class T, uint32_t N>
		struct TVec : public TVecN<T, N> {

			TVec() noexcept;
			TVec(const TVec& rhs) noexcept = default;
			TVec(const T& t) noexcept;
			template <class ...Ts>
			TVec(const T& t, const Ts& ...ts) noexcept;
			template <class ...Ts, uint32_t Ns>
			TVec(const TVecN<T, Ns>& t, const Ts& ...ts) noexcept;

			TVec& operator= (const TVec& rhs) noexcept;
			TVec& operator= (const T rhs[N]) noexcept;
			template<class = std::enable_if_t<N == 4>>
			TVec& operator= (const TQuat<T>& rhs) noexcept {
				using TVecN<T, 4>::x;
				using TVecN<T, 4>::y;
				using TVecN<T, 4>::z;
				using TVecN<T, 4>::w;
				x = rhs.x;
				y = rhs.y;
				z = rhs.z;
				w = rhs.w;
				return *this;
			}

			TVec& operator+= (const TVec& rhs) noexcept;
			TVec& operator-= (const TVec& rhs) noexcept;
			TVec& operator*= (const T& rhs) noexcept;

			const TVec& operator+ () const noexcept;
			const TVec operator- () const noexcept;

			TVec operator+ (const TVec& rhs) const noexcept;
			TVec operator- (const TVec& rhs) const noexcept;
			TVec operator* (const T& rhs) const noexcept;
			T operator* (const TVec& rhs) const noexcept;

			template<class = std::enable_if_t<N == 3>>
			TVec<T, 3> operator% (const TVec<T, 3>& rhs) const noexcept{
			return TVec<T, 3>
				((*this)[1] * rhs.z - (*this)[2] * rhs.y, 
				-((*this)[0] * rhs.z - (*this)[2] * rhs.x), 
				(*this)[0] * rhs.y - (*this)[1] * rhs.x);
			}

			T& operator[] (const uint32_t& i) throw();
			const T& operator[] (const uint32_t& i) const throw();

			template<class T1, uint32_t N1> explicit operator TVec<T1, N1>() const noexcept;

			friend std::ostream& operator<<	(std::ostream& out, const TVec& rhs) noexcept {
				out << "[";
				for(uint32_t i = 0; i < N - 1; i++) out << std::to_string(rhs[i]) + ", ";
				out << std::to_string(rhs[N - 1]) + "]";
				return out;
			}

			const float Magnitude() const noexcept;
			const TVec<float, N> Normalized() const noexcept;
			const TVec Sign() const noexcept;
			const TVec Abs() const noexcept;

			static const TVec zero;
			static const TVec one;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T, uint32_t N>
		inline TVec<T, N>::TVec() noexcept {
			for(uint32_t i = 0; i < N; i++) (*this)[i] = 0;
		}

		template<class T, uint32_t N>
		inline TVec<T, N>::TVec(const T& t) noexcept {
			for(uint32_t i = 0; i < N; i++) (*this)[i] = t;
		}

		template<class T, uint32_t N>
		template<class ...Ts>
		inline TVec<T, N>::TVec(const T& t, const Ts & ...ts) noexcept
			:TVecN<T, N> {t, ts...} {}

		template<class T, uint32_t N>
		template<class ...Ts, uint32_t Ns>
		inline TVec<T, N>::TVec(const TVecN<T, Ns>& t, const Ts & ...ts) noexcept
			:TVecN<T, N> {t, ts...} {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T, uint32_t N>
		inline TVec<T, N>& TVec<T, N>::operator= (const TVec& rhs) noexcept {
			for(uint32_t i = 0; i < N; i++) (*this)[i] = rhs[i];
			return *this;
		}

		template<class T, uint32_t N>
		inline TVec<T, N>& TVec<T, N>::operator= (const T rhs[N]) noexcept {
			for(uint32_t i = 0; i < N; i++) (*this)[i] = rhs[i];
			return *this;
		}

		template<class T, uint32_t N>
		inline TVec<T, N>& TVec<T, N>::operator+= (const TVec& rhs) noexcept {
			for(uint32_t i = 0; i < N; i++) (*this)[i] += rhs[i];
			return *this;
		}

		template<class T, uint32_t N>
		inline TVec<T, N>& TVec<T, N>::operator-= (const TVec& rhs) noexcept {
			for(uint32_t i = 0; i < N; i++) (*this)[i] -= rhs[i];
			return *this;
		}

		template<class T, uint32_t N>
		inline TVec<T, N>& TVec<T, N>::operator*= (const T& rhs) noexcept {
			*this = *this * rhs;
			return *this;
		}

		// -------------------------------------------- Unary Operators ------------------------------------------------ //
		
		template<class T, uint32_t N>
		inline const TVec<T, N>& TVec<T, N>::operator+ () const noexcept {
			return *this;
		}

		template<class T, uint32_t N>
		inline const TVec<T, N> TVec<T, N>::operator- () const noexcept {
			return *this * -1;
		}
		
		// ------------------------------------------ Arithmetic Operators --------------------------------------------- //

		template<class T, uint32_t N>
		inline TVec<T, N> TVec<T, N>::operator+ (const TVec& rhs) const noexcept {
			TVec<T, N> out;
			for(uint32_t i = 0; i < N; i++) out[i] = (*this)[i] + rhs[i];
			return out;
		}

		template<class T, uint32_t N>
		inline TVec<T, N> TVec<T, N>::operator- (const TVec& rhs) const noexcept {
			TVec<T, N> out;
			for(uint32_t i = 0; i < N; i++) out[i] = (*this)[i] - rhs[i];
			return out;
		}

		template<class T, uint32_t N>
		inline TVec<T, N> TVec<T, N>::operator* (const T& rhs) const noexcept {
			TVec<T, N> out;
			for(uint32_t i = 0; i < N; i++) out[i] = (*this)[i] * rhs;
			return out;
		}

		template<class T, uint32_t N>
		inline T TVec<T, N>::operator* (const TVec& rhs) const noexcept {
			T out(0);
			for(uint32_t i = 0; i < N; i++) out += (*this)[i] * rhs[i];
			return out;
		}

		// ----------------------------------------- Member Access Operators ------------------------------------------- //

		template<class T, uint32_t N>
		inline T& TVec<T, N>::operator[] (const uint32_t& i) throw() {
			if(i < N) return reinterpret_cast<T*>(this)[i];
		}

		template<class T, uint32_t N>
		inline const T& TVec<T, N>::operator[] (const uint32_t& i) const throw() {
			if(i < N) return reinterpret_cast<const T*>(this)[i];
		}

		// -------------------------------------------- Cast Operators ------------------------------------------------- //

		template<class T, uint32_t N>
		template<class T1, uint32_t N1>
		inline TVec<T, N>::operator TVec<T1, N1> () const noexcept {
			TVec<T1, N1> out;
			uint32_t c = (N1 < N) ? N1 : N;
			for(uint32_t i = 0; i < c; i++) out[i] = static_cast<T1>((*this)[i]);
			return out;
		}

		// ------------------------------------------- Vector Operations ----------------------------------------------- //

		template<class T, uint32_t N>
		inline const float TVec<T, N>::Magnitude() const noexcept {
			float m = 0.f;
			for(uint32_t i = 0; i < N; i++) m += (*this)[i] * (*this)[i];
			return std::sqrt(m);
		}

		template<class T, uint32_t N>
		inline const float* Normalize(const TVec<float, N>* vec, std::true_type) {
			return SSE_Vec_Normalize<N>((float*)vec);
		}

		template<class T, uint32_t N>
		inline const float* Normalize(const TVec<T, N>* vec, std::false_type) {
			TVec<float, N> temp(*vec);
			return SSE_Vec_Normalize<N>((float*)&temp);
		}

		template<class T, uint32_t N>
		inline const TVec<float, N> TVec<T, N>::Normalized() const noexcept {
			TVec<float, N> out;
			#ifdef AEN_SSE2
			out = Normalize<T, N>(this, std::conditional_t<std::is_same<float, T>::value, std::true_type, std::false_type>());
			#else

			float mag = Magnitude();
			float iSqrt = (mag != 0.f) ? (1.f / mag) : 0.f;
			for(uint32_t i = 0; i < N; i++) out[i] = (*this)[i] * iSqrt;
			#endif

			return out;
		}

		template<class T, uint32_t N>
		inline const TVec<T, N> TVec<T, N>::Sign() const noexcept {
			TVec<T, N> out;
			for(uint32_t i = 0; i < N; i++) out[i] = Aen::Sign((*this)[i]);
			return out;
		}

		template<class T, uint32_t N>
		inline const TVec<T, N> TVec<T, N>::Abs() const noexcept {
			TVec<T, N> out;
			for(uint32_t i = 0; i < N; i++) out[i] = std::abs((*this)[i]);
			return out;
		}

		// ------------------------------------------- Constant Variables ---------------------------------------------- //

		template<class T, uint32_t N>
		const TVec<T, N> TVec<T, N>::zero(0);

		template<class T, uint32_t N>
		const TVec<T, N> TVec<T, N>::one(1);
	}

	// --------------------------------------------------- Type Alias -------------------------------------------------- //

	using Vec2d = Concealed::TVec<double, 2>;
	using Vec3d = Concealed::TVec<double, 3>;
	using Vec4d = Concealed::TVec<double, 4>;

	using Vec2f = Concealed::TVec<float, 2>;
	using Vec3f = Concealed::TVec<float, 3>;
	using Vec4f = Concealed::TVec<float, 4>;

	using Vec2i = Concealed::TVec<int, 2>;
	using Vec3i = Concealed::TVec<int, 3>;
	using Vec4i = Concealed::TVec<int, 4>;

	// ----------------------------------------------------------------------------------------------------------------- //
}