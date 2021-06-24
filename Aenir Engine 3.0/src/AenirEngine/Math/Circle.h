#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//                                          /\                / ____(_)        | |     									 //
//                                         /  \   ___ _ __   | |     _ _ __ ___| | ___ 									 //
//                                        / /\ \ / _ \ '_ \  | |    | | '__/ __| |/ _ \									 //
//                                       / ____ \  __/ | | | | |____| | | | (__| |  __/									 //
//                                      /_/    \_\___|_| |_|  \_____|_|_|  \___|_|\___|									 //
// --------------------------------------------------------------------------------------------------------------------- //

#include"MathFunctions.h"

namespace Aen {

	namespace Concealed {

		// ------------------------------------------------------------------------------------------------------------- //
		//                                                       Circle                                                  //
		// ------------------------------------------------------------------------------------------------------------- //

		template<class T>
		struct TCircle {

			TCircle() = default;
			TCircle(const TCircle& rhs) = default;
			TCircle(const TVec<T, 2>& origin, const T& radius) noexcept;
			TCircle(const T& x, const T& y, const T& radius) noexcept;
			TCircle(const T& radius) noexcept;

			TCircle& operator= (const TCircle& rhs) noexcept;

			template<class T1> operator TCircle<T1>()const noexcept;

			friend std::ostream& operator<<	(std::ostream& out, const TCircle& rhs) noexcept {
				out << "Origin: " << rhs.origin << std::endl << "Radius: " << rhs.radius;
				return out;
			}

			const T Diameter() const noexcept;
			const T Circumference() const noexcept;
			const T Area() const noexcept;

			TVec<T, 2> origin;
			T radius;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T>
		inline TCircle<T>::TCircle(const TVec<T, 2>& origin, const T& radius) noexcept
			:origin(origin), radius(radius) {}

		template<class T>
		inline TCircle<T>::TCircle(const T& x, const T& y, const T& radius) noexcept
			:origin(x, y), radius(radius) {}

		template<class T>
		inline TCircle<T>::TCircle(const T& radius) noexcept
			:origin(), radius(radius) {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T>
		inline TCircle<T>& TCircle<T>::operator=(const TCircle& rhs) noexcept {
			origin = rhs.origin;
			radius = rhs.radius;
			return *this;
		}

		// -------------------------------------------- Cast Operators ------------------------------------------------- //

		template<class T>
		template<class T1>
		inline TCircle<T>::operator TCircle<T1>() const noexcept {
			TCircle<T> out;
			out.origin = static_cast<TVec<T1, 2>>(origin);
			out.radius = static_cast<T1>(radius);
			return out;
		}

		// ------------------------------------------- Circle Operations ----------------------------------------------- //

		template<class T>
		inline const T TCircle<T>::Diameter() const noexcept {
			return 2 * radius;
		}

		template<class T>
		inline const T TCircle<T>::Circumference() const noexcept {
			return (T)C_2PI * radius;
		}

		template<class T>
		inline const T TCircle<T>::Area() const noexcept {
			return (T)C_PI * radius * radius;
		}
	}

	// ------------------------------------------------------- Type Alias ---------------------------------------------- //

	using Circled = Concealed::TCircle<double>;
	using Circlef = Concealed::TCircle<float>;
	using Circlei = Concealed::TCircle<int>;

	// ----------------------------------------------------------------------------------------------------------------- //

}