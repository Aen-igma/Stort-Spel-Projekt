#pragma once
#include"MathFunctions.h"

namespace Aen {

	namespace Concealed {

		template<class T>
		struct TMatN<T, 4, 4> {
			TMatN<T, 4, 4>() :smMat() {}
			TMatN<T, 4, 4>(const TMatN<T, 4, 4>& rhs) = default;
			TMatN<T, 4, 4>(const TVec<T, 4>& x, const TVec<T, 4>& y, const TVec<T, 4>& z, const TVec<T, 4>& w) 
				:x(x), y(y), z(z), w(w) {}
			TMatN<T, 4, 4>(const TMatN<T, 2, 4>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13), a14(rhs.a14),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23), a24(rhs.a24),
				a31(0), a32(0), a33(0), a34(0),
				a41(0), a42(0), a43(0), a44(0) {}
			TMatN<T, 4, 4>(const TMatN<T, 2, 3>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13), a14(0),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23), a24(0),
				a31(0), a32(0), a33(0), a34(0),
				a41(0), a42(0), a43(0), a44(0) {}
			TMatN<T, 4, 4>(const TMatN<T, 3, 3>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13), a14(0),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23), a24(0),
				a31(rhs.a31), a32(rhs.a32), a33(rhs.a33), a34(0),
				a41(0), a42(0), a43(0), a44(0) {}
			TMatN<T, 4, 4>(const TMatN<T, 3, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(0), a14(0),
				a21(rhs.a21), a22(rhs.a22), a23(0), a24(0),
				a31(rhs.a31), a32(rhs.a32), a33(0), a34(0),
				a41(0), a42(0), a43(0), a44(0) {}
			TMatN<T, 4, 4>(const TMatN<T, 3, 4>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13), a14(rhs.a14),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23), a24(rhs.a24),
				a31(rhs.a31), a32(rhs.a32), a33(rhs.a33), a34(rhs.a34),
				a41(0), a42(0), a43(0), a44(0) {}
			TMatN<T, 4, 4>(const TMatN<T, 4, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(0), a14(0),
				a21(rhs.a21), a22(rhs.a22), a23(0), a24(0),
				a31(rhs.a31), a32(rhs.a32), a33(0), a34(0),
				a41(rhs.a41), a42(rhs.a42), a43(0), a44(0) {}
			TMatN<T, 4, 4>(const TMatN<T, 4, 3>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13), a14(0),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23), a24(0),
				a31(rhs.a31), a32(rhs.a32), a33(rhs.a33), a34(0),
				a41(rhs.a41), a42(rhs.a42), a43(rhs.a43), a44(0) {}
			TMatN<T, 4, 4>(	const T& a11, const T& a12, const T& a13, const T& a14,
							const T& a21, const T& a22, const T& a23, const T& a24, 
							const T& a31, const T& a32, const T& a33, const T& a34, 
							const T& a41, const T& a42, const T& a43, const T& a44) :
					a11(a11), a12(a12), a13(a13), a14(a14), 
					a21(a21), a22(a22), a23(a23), a24(a24), 
					a31(a31), a32(a32), a33(a33), a34(a34), 
					a41(a41), a42(a42), a43(a43), a44(a44) {}
			union {
				sm::Matrix smMat;
				struct {T a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44;};
				struct {TVec<T, 4> x, y, z, w;};
				struct {TVec<T, 4> r, g, b, a;};
				TVec<T, 4> arr[4];
			};
		};

		// ------------------------------------------------------------------------------------------------------------- //
		//                                                     Matrix                                                    //
		// ------------------------------------------------------------------------------------------------------------- //

		template<class T>
		struct TMat : public TMatN<T, 4, 4> {

			using TMatN<T, 4, 4>::smMat;
			using TMatN<T, 4, 4>::arr;

			TMat() noexcept;
			TMat(const TMat& rhs) noexcept = default;
			template <typename ...Ts>
			TMat(const T& t, const Ts&... ts) noexcept;
			template <typename ...Ts, uint32_t Ns>
			TMat(const TVec<T, Ns>& t, const Ts&... ts) noexcept;

			TMat& operator*= (const TMat& rhs) noexcept;
			TMat& operator= (const TMat& rhs) noexcept;
			
			const TMat& operator+ () const noexcept;
			const TMat operator- () const noexcept;

			TMat operator* (const TMat& rhs) const noexcept;
			TMat operator* (const T& rhs)  const noexcept;
			TMat operator+ (const TMat& rhs) const noexcept;
			TMat operator- (const TMat& rhs) const noexcept;

			TVec<T, 4>& operator[] (const uint32_t& i) throw();
			const TVec<T, 4>& operator[] (const uint32_t& i) const throw();

			friend std::ostream& operator<<	(std::ostream& out, const TMat& rhs) noexcept {
				for(uint32_t i = 0; i < 4; i++) out << rhs[i] << std::endl;
				return out;
			}

			const TMat Transposed () const noexcept;
			const T Determinant() const noexcept;
			const TMat Inverse() const noexcept;

			static const TMat identity;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T>
		inline TMat<T>::TMat() noexcept :TMatN<T, 4, 4>() {}

		template<class T>
		template<typename ...Ts>
		inline TMat<T>::TMat(const T& t, const Ts & ...ts) noexcept
			:TMatN<T, 4, 4> {t, ts...} {}

		template<class T>
		template<typename ...Ts, uint32_t Ns>
		inline TMat<T>::TMat(const TVec<T, Ns>& t, const Ts& ...ts) noexcept
			:TMatN<T, 4, 4> {t, ts...} {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T>
		inline TMat<T>& TMat<T>::operator*= (const TMat& rhs) noexcept {
			*this = *this * rhs;
			return *this;
		}

		template<class T>
		inline TMat<T>& TMat<T>::operator= (const TMat& rhs) noexcept {
			smMat = rhs.smMat;
			return *this;
		}

		// -------------------------------------------- Unary Operators ------------------------------------------------ //

		template<class T>
		inline const TMat<T>& TMat<T>::operator+ () const noexcept {
			return *this;
		}

		template<class T>
		inline const TMat<T> TMat<T>::operator- () const noexcept {
			return *this * -1;
		}

		// ------------------------------------------ Arithmetic Operators --------------------------------------------- //

		template<class T>
		inline TMat<T> TMat<T>::operator* (const TMat& rhs) const noexcept {
			TMat<T> out;
			out.smMat = smMat * rhs.smMat;
			return out;
		}

		template<class T>
		inline TMat<T> TMat<T>::operator* (const T& rhs) const noexcept {
			TMat<T> out;
			out.smMat = smMat * rhs;
			return out;
		}

		template<class T>
		inline TMat<T> TMat<T>::operator+ (const TMat& rhs) const noexcept {
			TMat<T> out;
			out.smMat = smMat + rhs.smMat;
			return out;
		}

		template<class T>
		inline TMat<T> TMat<T>::operator- (const TMat& rhs) const noexcept {
			TMat<T> out;
			out.smMat = smMat - rhs.smMat;
			return out;
		}

		// ----------------------------------------- Member Access Operators ------------------------------------------- //

		template<class T>
		inline TVec<T, 4>& TMat<T>::operator[] (const uint32_t& i) throw() {
			return arr[i];
		}

		template<class T>
		inline const TVec<T, 4>& TMat<T>::operator[] (const uint32_t& i) const throw() {
			return arr[i];
		}

		// ------------------------------------------- Matrix Operations ----------------------------------------------- //

		template<class T>
		inline const TMat<T> TMat<T>::Transposed() const noexcept {
			TMat<T> out(*this);
			out.smMat = out.smMat.Transpose();
			return out;
		}

		template<class T>
		inline const T TMat<T>::Determinant() const noexcept {
			T out = smMat.Determinant();
			return out;
		}

		template<class T>
		inline const TMat<T> TMat<T>::Inverse() const noexcept {
			TMat<T> out(*this);
			out.smMat = out.smMat.Invert();
			return out;
		}

		// ------------------------------------------- Constant Variables ---------------------------------------------- //

		template<class T>
		const TMat<T> TMat<T>::identity = TMat<T>();
	}

	// --------------------------------------------------- Type Alias -------------------------------------------------- //

	using Mat4f = Concealed::TMat<float>;
	using Mat4i = Concealed::TMat<int>;

	// ----------------------------------------------------------------------------------------------------------------- //
}