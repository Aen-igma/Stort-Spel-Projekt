#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//									      /\               |__   __|                     								 //
//									     /  \   ___ _ __      | |_ __ __ _  ___ ___ _ __ 								 //
//									    / /\ \ / _ \ '_ \     | | '__/ _` |/ __/ _ \ '__|								 //
//									   / ____ \  __/ | | |    | | | | (_| | (_|  __/ |   								 //
//									  /_/    \_\___|_| |_|    |_|_|  \__,_|\___\___|_|   								 //
// --------------------------------------------------------------------------------------------------------------------- //

#include"MathFunctions.h"

namespace Aen {
	
	namespace Concealed {

		// ------------------------------------------------------------------------------------------------------------- //
		//                                                      Tracer                                                   //
		// ------------------------------------------------------------------------------------------------------------- //

		template<class T> 
		class TTracer {

			TTracer() noexcept;
			TTracer(const TTracer& rhs) = default;
			TTracer(const TVec<T, 3>& origin, const TVec<T, 3>& direction) noexcept;

			TTracer& operator= (const TTracer<T>& rhs) noexcept;

			template<class T1> explicit operator TTracer<T1>() const noexcept;

			void SetOrigin(const TVec<T, 3>& pos) noexcept;
			void SetOrigin(const T& x, const T& y, const T& z) noexcept;
			void SetDirection(const TVec<T, 3>& dir) noexcept;
			void SetDirection(const T& x, const T& y, const T& z) noexcept;

			const TVec<T, 3>& GetOrigin() const noexcept;
			const TVec<T, 3>& GetDirection() const noexcept;
			const T& GetDistance() const noexcept;
			const TVec<T, 3> GetHitPos() const noexcept;

			private:
			T distance;
			TVec<T, 3> origin;
			TVec<T, 3> direction;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T>
		inline TTracer<T>::TTracer() noexcept
			:distance(0), origin(TVec<T, 3>::zero), direction(TVec<T, 3>::zero) {}

		template<class T>
		inline TTracer<T>::TTracer(const TVec<T, 3>& origin, const TVec<T, 3>& direction) noexcept
			:distance(0), origin(origin), direction(direction) {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T>
		inline TTracer<T>& TTracer<T>::operator=(const TTracer<T>& rhs) noexcept {
			distance = rhs.distance;
			origin = rhs.origin;
			direction = rhs.direction;
			return *this;
		}

		// -------------------------------------------- Cast Operators ------------------------------------------------- //

		template<class T>
		template<class T1>
		inline TTracer<T>::operator TTracer<T1>() const noexcept {
			TTracer<T1> out;
			out.distance = static_cast<T1>(distance);
			out.origin = static_cast<TVec<T1, 3>>(origin);
			out.direction = static_cast<TVec<T1, 3>>(direction);
			return out;
		}

		// -------------------------------------------- Set Operations ------------------------------------------------- //

		template<class T>
		inline void TTracer<T>::SetOrigin(const TVec<T, 3>& pos) noexcept {
			origin = pos;
		}

		template<class T>
		inline void TTracer<T>::SetOrigin(const T& x, const T& y, const T& z) noexcept {
			origin = {x, y, z};
		}

		template<class T>
		inline void TTracer<T>::SetDirection(const TVec<T, 3>& dir) noexcept {
			direction = dir.Normalized();
		}

		template<class T>
		inline void TTracer<T>::SetDirection(const T& x, const T& y, const T& z) noexcept {
			direction = TVec<T, 3>(x, y, z).Normalized();
		}

		// -------------------------------------------- Get Operations ------------------------------------------------- //

		template<class T>
		inline const TVec<T, 3>& TTracer<T>::GetOrigin() const noexcept {
			return origin;
		}

		template<class T>
		inline const TVec<T, 3>& TTracer<T>::GetDirection() const noexcept {
			return direction;
		}

		template<class T>
		inline const T& TTracer<T>::GetDistance() const noexcept {
			return distance;
		}

		template<class T>
		inline const TVec<T, 3> TTracer<T>::GetHitPos() const noexcept {
			return origin + direction * distance;
		}
	}

	// ------------------------------------------------------- Type Alias ---------------------------------------------- //

	using Tracerd = Concealed::TTracer<double>;
	using Tracerf = Concealed::TTracer<float>;

	// ----------------------------------------------------------------------------------------------------------------- //
}