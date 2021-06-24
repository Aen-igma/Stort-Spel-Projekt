#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//                                       /\               |  \/  |     | |      (_)                                      //
//                                      /  \   ___ _ __   | \  / | __ _| |_ _ __ ___  __                                 //
//                                     / /\ \ / _ \ '_ \  | |\/| |/ _` | __| '__| \ \/ /                                 //
//                                    / ____ \  __/ | | | | |  | | (_| | |_| |  | |>  <                                  //
//                                   /_/    \_\___|_| |_| |_|  |_|\__,_|\__|_|  |_/_/\_\                                 //
// --------------------------------------------------------------------------------------------------------------------- //

#include"MathFunctions.h"

namespace Aen {

	namespace Concealed {

		// ------------------------------------------------ Determinant ------------------------------------------------ //

		template<class T>
		const T Determinant(const TMat<T, 2, 2>& m) noexcept {
			return m.a11 * m.a22 - m.a12 * m.a21;
		}

		template<class T>
		const T Determinant(const TMat<T, 3, 3>& m) noexcept {
			T a = m.a22 * m.a33 - m.a23 * m.a32;
			T b = m.a21 * m.a33 - m.a23 * m.a31;
			T c = m.a21 * m.a32 - m.a22 * m.a31;
			return m.a11 * a - m.a12 * b + m.a13 * c;
		}

		template<class T>
		const T Determinant(const TMat<T, 4, 4>& m) noexcept {
			T a = TMat<T, 3, 3>(m.a22, m.a23, m.a24, m.a32, m.a33, m.a34, m.a42, m.a43, m.a44).Determinant();
			T b = TMat<T, 3, 3>(m.a21, m.a23, m.a24, m.a31, m.a33, m.a34, m.a41, m.a43, m.a44).Determinant();
			T c = TMat<T, 3, 3>(m.a21, m.a22, m.a24, m.a31, m.a32, m.a34, m.a41, m.a42, m.a44).Determinant();
			T d = TMat<T, 3, 3>(m.a21, m.a22, m.a23, m.a31, m.a32, m.a33, m.a41, m.a42, m.a43).Determinant();
			return m.a11 * a - m.a12 * b + m.a13 * c - m.a14 * d;
		}

		// ------------------------------------------------- Adjugate -------------------------------------------------- //

		template<class T>
		const TMat<T, 2, 2> Adjugate(const TMat<T, 2, 2>& m) noexcept {
			TMat<T, 2, 2> out(m.a22, -m.a12, -m.a21, m.a11);
			return out;
		}

		template<class T>
		const TMat<T, 3, 3> Adjugate(const TMat<T, 3, 3>& m) noexcept {
			TMat<T, 3, 3> out;
			out.a11 = TMat<T, 2, 2>(m.a22, m.a23, m.a32, m.a33).Determinant();
			out.a12 = -TMat<T, 2, 2>(m.a21, m.a23, m.a31, m.a33).Determinant();
			out.a13 = TMat<T, 2, 2>(m.a21, m.a22, m.a31, m.a32).Determinant();
			out.a21 = -TMat<T, 2, 2>(m.a12, m.a13, m.a32, m.a33).Determinant();
			out.a22 = TMat<T, 2, 2>(m.a11, m.a13, m.a31, m.a33).Determinant();
			out.a23 = -TMat<T, 2, 2>(m.a11, m.a12, m.a31, m.a32).Determinant();
			out.a31 = TMat<T, 2, 2>(m.a12, m.a13, m.a22, m.a23).Determinant();
			out.a32 = -TMat<T, 2, 2>(m.a11, m.a13, m.a21, m.a23).Determinant();
			out.a33 = TMat<T, 2, 2>(m.a11, m.a12, m.a21, m.a22).Determinant();
			return out.Transposed();
		}

		template<class T>
		const TMat<T, 4, 4> Adjugate(const TMat<T, 4, 4>& m) noexcept {
			TMat<T, 4, 4> out;
			out.a11 = TMat<T, 3, 3>(m.a22, m.a23, m.a24, m.a32, m.a33, m.a34, m.a42, m.a43, m.a44).Determinant();
			out.a12 = -TMat<T, 3, 3>(m.a21, m.a23, m.a24, m.a31, m.a33, m.a34, m.a41, m.a43, m.a44).Determinant();
			out.a13 = TMat<T, 3, 3>(m.a21, m.a22, m.a24, m.a31, m.a32, m.a34, m.a41, m.a42, m.a44).Determinant();
			out.a14 = -TMat<T, 3, 3>(m.a21, m.a22, m.a23, m.a31, m.a32, m.a33, m.a41, m.a42, m.a43).Determinant();
			out.a21 = -TMat<T, 3, 3>(m.a12, m.a13, m.a14, m.a32, m.a33, m.a34, m.a42, m.a43, m.a44).Determinant();
			out.a22 = TMat<T, 3, 3>(m.a11, m.a13, m.a14, m.a31, m.a33, m.a34, m.a41, m.a43, m.a44).Determinant();
			out.a23 = -TMat<T, 3, 3>(m.a11, m.a12, m.a14, m.a31, m.a32, m.a34, m.a41, m.a42, m.a44).Determinant();
			out.a24 = TMat<T, 3, 3>(m.a11, m.a12, m.a13, m.a31, m.a32, m.a33, m.a41, m.a42, m.a43).Determinant();
			out.a31 = TMat<T, 3, 3>(m.a12, m.a13, m.a14, m.a22, m.a23, m.a24, m.a42, m.a43, m.a44).Determinant();
			out.a32 = -TMat<T, 3, 3>(m.a11, m.a13, m.a14, m.a21, m.a23, m.a24, m.a41, m.a43, m.a44).Determinant();
			out.a33 = TMat<T, 3, 3>(m.a11, m.a12, m.a14, m.a21, m.a22, m.a24, m.a41, m.a42, m.a44).Determinant();
			out.a34 = -TMat<T, 3, 3>(m.a11, m.a12, m.a13, m.a21, m.a22, m.a23, m.a41, m.a42, m.a43).Determinant();
			out.a41 = -TMat<T, 3, 3>(m.a12, m.a13, m.a14, m.a22, m.a23, m.a24, m.a32, m.a33, m.a34).Determinant();
			out.a42 = TMat<T, 3, 3>(m.a11, m.a13, m.a14, m.a21, m.a23, m.a24, m.a31, m.a33, m.a34).Determinant();
			out.a43 = -TMat<T, 3, 3>(m.a11, m.a12, m.a14, m.a21, m.a22, m.a24, m.a31, m.a32, m.a34).Determinant();
			out.a44 = TMat<T, 3, 3>(m.a11, m.a12, m.a13, m.a21, m.a22, m.a23, m.a31, m.a32, m.a33).Determinant();
			return out.Transposed();
		}

		// ---------------------------------------------- Matrix Container --------------------------------------------- //

		template<class T>
		struct TMatN<T, 2, 2> {
			TMatN<T, 2, 2>() :x(), y() {}
			TMatN<T, 2, 2>(const TMatN<T, 2, 2>& rhs) = default;
			TMatN<T, 2, 2>(const TVec<T, 2>& x, const TVec<T, 2>& y) 
				:x(x), y(y) {}
			TMatN<T, 2, 2>(const TVec<T, 4>& rhs) 
				:a11(rhs.x), a12(rhs.y), a21(rhs.z), a22(rhs.w) {}
			TMatN<T, 2, 2>(const T& a11, const T& a12, const TVec<T, 2>& y) 
				:a11(a11), a12(a12), a21(y[0]), a22(y[1]) {}
			TMatN<T, 2, 2>(const TVec<T, 2>& x, const T& a21, const T& a22) 
				:a11(y[0]), a12(y[1]), a21(a21), a22(a22) {}
			TMatN<T, 2, 2>(const T& a11, const T& a12, const T& a21, const T& a22) 
				:a11(a11), a12(a12), a21(a21), a22(a22) {}
			union {
				struct {T a11, a12, a21, a22;};
				struct {TVec<T, 2> x, y;};
				struct {TVec<T, 2> r, g;};
			};
		};

		template<class T>
		struct TMatN<T, 2, 3> {
			TMatN<T, 2, 3>() :x(), y() {}
			TMatN<T, 2, 3>(const TMatN<T, 2, 3>& rhs) = default;
			TMatN<T, 2, 3>(const TVec<T, 3>& x, const TVec<T, 3>& y) 
				:x(x), y(y) {}
			TMatN<T, 2, 3>(const TMatN<T, 2, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(0),
				a21(rhs.a21), a22(rhs.a22), a23(0) {}
			TMatN<T, 2, 3>(	const T& a11, const T& a12, const T& a13,
							const T& a21, const T& a22, const T& a23) :
					a11(a11), a12(a12), a13(a13), 
					a21(a21), a22(a22), a23(a23) {}
			union {
				struct {T a11, a12, a13, a21, a22, a23;};
				struct {TVec<T, 3> x, y;};
				struct {TVec<T, 3> r, g;};
			};
		};

		template<class T>
		struct TMatN<T, 2, 4> {
			TMatN<T, 2, 4>() :x(), y() {}
			TMatN<T, 2, 4>(const TMatN<T, 2, 4>& rhs) = default;
			TMatN<T, 2, 4>(const TVec<T, 4>& x, const TVec<T, 4>& y) 
				:x(x), y(y) {}
			TMatN<T, 2, 4>(const TMatN<T, 2, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(0), a14(0),
				a21(rhs.a21), a22(rhs.a22), a23(0), a24(0) {}
			TMatN<T, 2, 4>(	const T& a11, const T& a12, const T& a13, const T& a14,
							const T& a21, const T& a22, const T& a23, const T& a24) :
					a11(a11), a12(a12), a13(a13), a14(a14), 
					a21(a21), a22(a22), a23(a23), a24(a24) {}
			union {
				struct {T a11, a12, a13, a14, a21, a22, a23, a24;};
				struct {TVec<T, 4> x, y;};
				struct {TVec<T, 4> r, g;};
			};
		};

		template<class T>
		struct TMatN<T, 3, 2> {
			TMatN<T, 3, 2>() :x(), y(), z() {}
			TMatN<T, 3, 2>(const TMatN<T, 3, 2>& rhs) = default;
			TMatN<T, 3, 2>(const TVec<T, 2>& x, const TVec<T, 2>& y, const TVec<T, 2>& z) 
				:x(x), y(y), z(z) {}
			TMatN<T, 3, 2>(const TMatN<T, 2, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12),
				a21(rhs.a21), a22(rhs.a22),
				a31(0), a32(0) {}
			TMatN<T, 3, 2>(	const T& a11, const T& a12,
							const T& a21, const T& a22, 
							const T& a31, const T& a32) :
					a11(a11), a12(a12), 
					a21(a21), a22(a22), 
					a31(a31), a32(a32) {}
			union {
				struct {T a11, a12, a21, a22, a31, a32;};
				struct {TVec<T, 2> x, y, z;};
				struct {TVec<T, 2> r, g, b;};
			};
		};

		template<class T>
		struct TMatN<T, 3, 3> {
			TMatN<T, 3, 3>() :x(), y(), z() {}
			TMatN<T, 3, 3>(const TMatN<T, 3, 3>& rhs) = default;
			TMatN<T, 3, 3>(const TVec<T, 3>& x, const TVec<T, 3>& y, const TVec<T, 3>& z) 
				:x(x), y(y), z(z) {}
			TMatN<T, 3, 3>(const TMatN<T, 2, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(0),
				a21(rhs.a21), a22(rhs.a22), a23(0),
				a31(0), a32(0), a33(0) {}
			TMatN<T, 3, 3>(const TMatN<T, 2, 3>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23),
				a31(0), a32(0), a33(0) {}
			TMatN<T, 3, 3>(const TMatN<T, 3, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(0),
				a21(rhs.a21), a22(rhs.a22), a23(0),
				a31(rhs.a31), a32(rhs.a32), a33(0) {}
			TMatN<T, 3, 3>(	const T& a11, const T& a12, const T& a13,
							const T& a21, const T& a22, const T& a23, 
							const T& a31, const T& a32, const T& a33) :
					a11(a11), a12(a12), a13(a13), 
					a21(a21), a22(a22), a23(a23), 
					a31(a31), a32(a32), a33(a33) {}
			union {
				struct {T a11, a12, a13, a21, a22, a23, a31, a32, a33;};
				struct {TVec<T, 3> x, y, z;};
				struct {TVec<T, 3> r, g, b;};
			};
		};

		template<class T>
		struct TMatN<T, 3, 4> {
			TMatN<T, 3, 4>() :x(), y(), z() {}
			TMatN<T, 3, 4>(const TMatN<T, 3, 4>& rhs) = default;
			TMatN<T, 3, 4>(const TVec<T, 2>& x, const TVec<T, 2>& y, const TVec<T, 2>& z)
				:x(x), y(y), z(z) {}
			TMatN<T, 3, 4>(const TMatN<T, 3, 3>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13), a14(0),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23), a24(0),
				a31(rhs.a31), a32(rhs.a32), a33(rhs.a33), a34(0) {}
			TMatN<T, 3, 4>(	const T& a11, const T& a12, const T& a13, const T& a14,
							const T& a21, const T& a22, const T& a23, const T& a24, 
							const T& a31, const T& a32, const T& a33, const T& a34) :
					a11(a11), a12(a12), a13(a13), a14(a14), 
					a21(a21), a22(a22), a23(a23), a24(a24), 
					a31(a31), a32(a32), a33(a33), a34(a34) {}
			union {
				struct {T a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34;};
				struct {TVec<T, 4> x, y, z;};
				struct {TVec<T, 4> r, g, b;};
			};
		};

		template<class T>
		struct TMatN<T, 4, 2> {
			TMatN<T, 4, 2>() :x(), y(), z(), w() {}
			TMatN<T, 4, 2>(const TMatN<T, 4, 2>& rhs) = default;
			TMatN<T, 4, 2>(const TVec<T, 2>& x, const TVec<T, 2>& y, const TVec<T, 2>& z, const TVec<T, 2>& w)
				: x(x), y(y), z(z), w(w) {}
			TMatN<T, 4, 2>(const TMatN<T, 2, 2>& rhs) :
				a11(rhs.a11), a12(rhs.a12),
				a21(rhs.a21), a22(rhs.a22),
				a31(0), a32(0),
				a41(0), a42(0) {}
			TMatN<T, 4, 2>(	const T& a11, const T& a12,
							const T& a21, const T& a22,
							const T& a31, const T& a32,
							const T& a41, const T& a42) :
				a11(a11), a12(a12),
				a21(a21), a22(a22),
				a31(a31), a32(a32),
				a41(a41), a42(a42) {}
			union {
				struct {T a11, a12, a21, a22, a31, a32, a41, a42;};
				struct {TVec<T, 2> x, y, z, w;};
				struct {TVec<T, 2> r, g, b, a;};
			};
		};

		template<class T>
		struct TMatN<T, 4, 3> {
			TMatN<T, 4, 3>() :x(), y(), z(), w() {}
			TMatN<T, 4, 3>(const TMatN<T, 4, 3>& rhs) = default;
			TMatN<T, 4, 3>(const TVec<T, 3>& x, const TVec<T, 3>& y, const TVec<T, 3>& z, const TVec<T, 3>& w) 
				:x(x), y(y), z(z), w(w) {}
			TMatN<T, 4, 3>(const TMatN<T, 3, 3>& rhs) :
				a11(rhs.a11), a12(rhs.a12), a13(rhs.a13),
				a21(rhs.a21), a22(rhs.a22), a23(rhs.a23),
				a31(rhs.a31), a32(rhs.a32), a33(rhs.a33),
				a41(0), a42(0), a43(0) {}
			TMatN<T, 4, 3>(	const T& a11, const T& a12, const T& a13,
							const T& a21, const T& a22, const T& a23,
							const T& a31, const T& a32, const T& a33,
							const T& a41, const T& a42, const T& a43) :
				a11(a11), a12(a12), a13(a13),
				a21(a21), a22(a22), a23(a23),
				a31(a31), a32(a32), a33(a33),
				a41(a41), a42(a42), a43(a43) {}
			union {
				struct {T a11, a12, a13, a21, a22, a23, a31, a32, a33, a41, a42, a43;};
				struct {TVec<T, 3> x, y, z, w;};
				struct {TVec<T, 3> r, g, b, a;};
			};
		};

		template<class T>
		struct TMatN<T, 4, 4> {
			TMatN<T, 4, 4>() :x(), y(), z(), w() {}
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
				struct {T a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44;};
				struct {TVec<T, 4> x, y, z, w;};
				struct {TVec<T, 4> r, g, b, a;};
			};
		};

		// ------------------------------------------------------------------------------------------------------------- //
		//                                                     Matrix                                                    //
		// ------------------------------------------------------------------------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		struct TMat : public TMatN<T, R, C> {

			TMat() noexcept;
			TMat(const TMat& rhs) noexcept = default;
			template <typename ...Ts>
			TMat(const T& t, const Ts&... ts) noexcept;
			template <typename ...Ts, uint32_t Ns>
			TMat(const TVec<T, Ns>& t, const Ts&... ts) noexcept;
			template <typename ...Ts, uint32_t Rs, uint32_t Cs>
			TMat(const TMat<T, Rs, Cs>& t, const Ts&... ts) noexcept;

			TMat& operator*= (const TMat& rhs) noexcept;
			TMat& operator= (const TMat& rhs) noexcept;
			
			const TMat& operator+ () const noexcept;
			const TMat operator- () const noexcept;

			TMat operator* (const TMat& rhs) const noexcept;
			TMat operator* (const T& rhs)  const noexcept;
			TMat operator+ (const TMat& rhs) const noexcept;
			TMat operator- (const TMat& rhs) const noexcept;

			TVec<T, C>& operator[] (const uint32_t& i) throw();
			const TVec<T, C>& operator[] (const uint32_t& i) const throw();

			template<class T1, uint32_t R1, uint32_t C1> explicit operator TMat<T1, R1, C1>() const noexcept;

			friend std::ostream& operator<<	(std::ostream& out, const TMat& rhs) noexcept {
				for(uint32_t i = 0; i < R; i++) out << rhs[i] << std::endl;
				return out;
			}

			const TMat<T, C, R> Transposed () const noexcept;

			template<class = std::enable_if_t<R == C>>
			const T Determinant() const noexcept {return Concealed::Determinant<T>(*this);}
			template<class = std::enable_if_t<R == C>>
			const TMat Adjugate () const noexcept {return Concealed::Adjugate<T>(*this);}
			template<class = std::enable_if_t<R == C>>
			const TMat Inverse () const noexcept {
				float d = Determinant();
				float scale = (d != 0) ? 1.f / d : 0.f;
				return Adjugate() * scale;
			}

			static const TMat identity;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		inline TMat<T, R, C>::TMat() noexcept {
			for(uint32_t i = 0; i < R; i++) (*this)[i] = TVec<T, C>::zero;
			uint32_t size = (R < C) ? R : C;
			for(uint32_t i = 0; i < size; i++) (*this)[i][i] = 1;
		}

		template<class T, uint32_t R, uint32_t C>
		template<typename ...Ts>
		inline TMat<T, R, C>::TMat(const T& t, const Ts & ...ts) noexcept
			:TMatN<T, R, C> {t, ts...} {}

		template<class T, uint32_t R, uint32_t C>
		template<typename ...Ts, uint32_t Ns>
		inline TMat<T, R, C>::TMat(const TVec<T, Ns>& t, const Ts& ...ts) noexcept
			:TMatN<T, R, C> {t, ts...} {}

		template<class T, uint32_t R, uint32_t C>
		template<typename ...Ts, uint32_t Rs, uint32_t Cs>
		inline TMat<T, R, C>::TMat(const TMat<T, Rs, Cs>& t, const Ts & ...ts) noexcept
			:TMatN<T, R, C>{t, ts...} {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		inline TMat<T, R, C>& TMat<T, R, C>::operator*= (const TMat& rhs) noexcept {
			*this = *this * rhs;
			return *this;
		}

		template<class T, uint32_t R, uint32_t C>
		inline TMat<T, R, C>& TMat<T, R, C>::operator= (const TMat& rhs) noexcept {
			for(uint32_t i = 0; i < R; i++) (*this)[i] = rhs[i];
			return *this;
		}

		// -------------------------------------------- Unary Operators ------------------------------------------------ //

		template<class T, uint32_t R, uint32_t C>
		inline const TMat<T, R, C>& TMat<T, R, C>::operator+ () const noexcept {
			return *this;
		}

		template<class T, uint32_t R, uint32_t C>
		inline const TMat<T, R, C> TMat<T, R, C>::operator- () const noexcept {
			return *this * -1;
		}

		// ------------------------------------------ Arithmetic Operators --------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		inline TMat<T, R, C> TMat<T, R, C>::operator* (const TMat& rhs) const noexcept {
			TMat<T, R, C> out;
			TMat<T, R, C> rhsT = rhs.Transposed();
			for(uint32_t i = 0; i < R; i++) for(uint32_t j = 0; j < C; j++) out[i][j] = (*this)[i] * rhsT[j];
			return out;
		}

		template<class T, uint32_t R, uint32_t C>
		inline TMat<T, R, C> TMat<T, R, C>::operator* (const T& rhs) const noexcept {
			TMat<T, R, C> out;
			for(uint32_t i = 0; i < R; i++) out[i] = (*this)[i] * rhs;
			return out;
		}

		template<class T, uint32_t R, uint32_t C>
		inline TMat<T, R, C> TMat<T, R, C>::operator+ (const TMat& rhs) const noexcept {
			TMat<T, R, C> out;
			for(uint32_t i = 0; i < R; i++) out[i] = (*this)[i] + rhs[i];
			return out;
		}

		template<class T, uint32_t R, uint32_t C>
		inline TMat<T, R, C> TMat<T, R, C>::operator- (const TMat& rhs) const noexcept {
			TMat<T, R, C> out;
			for(uint32_t i = 0; i < R; i++) out[i] = (*this)[i] - rhs[i];
			return out;
		}

		// ----------------------------------------- Member Access Operators ------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		inline TVec<T, C>& TMat<T, R, C>::operator[] (const uint32_t& i) throw() {
			if(i < R) return reinterpret_cast<TVec<T, C>*>(this)[i];
		}

		template<class T, uint32_t R, uint32_t C>
		inline const TVec<T, C>& TMat<T, R, C>::operator[] (const uint32_t& i) const throw() {
			if(i < R) return reinterpret_cast<const TVec<T, C>*>(this)[i];
		}

		// -------------------------------------------- Cast Operators ------------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		template<class T1, uint32_t R1, uint32_t C1>
		inline TMat<T, R, C>::operator TMat<T1, R1, C1>() const noexcept {
			TMat<T1, R1, C1> out;
			uint32_t c = (R1 < R) ? R1 : R;
			for(uint32_t i = 0; i < c; i++) out[i] = static_cast<TVec<T1, C1>>((*this)[i]);
			return out;
		}

		// ------------------------------------------- Matrix Operations ----------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		inline const TMat<T, C, R> TMat<T, R, C>::Transposed() const noexcept {
			TMat<T, C, R> out;
			for(uint32_t i = 0; i < R; i++) for(uint32_t j = 0; j < C; j++) out[j][i] = (*this)[i][j];
			return out;
		}

		// ------------------------------------------- Constant Variables ---------------------------------------------- //

		template<class T, uint32_t R, uint32_t C>
		const TMat<T, R, C> TMat<T, R, C>::identity = TMat<T, R, C>();
	}

	// --------------------------------------------------- Type Alias -------------------------------------------------- //

	using Mat2d = Concealed::TMat<double, 2, 2>;
	using Mat2x3d = Concealed::TMat<double, 2, 3>;
	using Mat2x4d = Concealed::TMat<double, 2, 4>;
	using Mat3x2d = Concealed::TMat<double, 3, 2>;
	using Mat3d = Concealed::TMat<double, 3, 3>;
	using Mat3x4d = Concealed::TMat<double, 3, 4>;
	using Mat4x2d = Concealed::TMat<double, 4, 2>;
	using Mat4x3d = Concealed::TMat<double, 4, 3>;
	using Mat4d = Concealed::TMat<double, 4, 4>;
		  				  
	using Mat2f = Concealed::TMat<float, 2, 2>;
	using Mat2x3f = Concealed::TMat<float, 2, 3>;
	using Mat2x4f = Concealed::TMat<float, 2, 4>;
	using Mat3x2f = Concealed::TMat<float, 3, 2>;
	using Mat3f = Concealed::TMat<float, 3, 3>;
	using Mat3x4f = Concealed::TMat<float, 3, 4>;
	using Mat4x2f = Concealed::TMat<float, 4, 2>;
	using Mat4x3f = Concealed::TMat<float, 4, 3>;
	using Mat4f = Concealed::TMat<float, 4, 4>;

	using Mat2i = Concealed::TMat<int, 2, 2>;
	using Mat2x3i = Concealed::TMat<int, 2, 3>;
	using Mat2x4i = Concealed::TMat<int, 2, 4>;
	using Mat3x2i = Concealed::TMat<int, 3, 2>;
	using Mat3i = Concealed::TMat<int, 3, 3>;
	using Mat3x4i = Concealed::TMat<int, 3, 4>;
	using Mat4x2i = Concealed::TMat<int, 4, 2>;
	using Mat4x3i = Concealed::TMat<int, 4, 3>;
	using Mat4i = Concealed::TMat<int, 4, 4>;

	// ----------------------------------------------------------------------------------------------------------------- //
}