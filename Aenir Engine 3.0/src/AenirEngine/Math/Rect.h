#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//                                            /\               |  __ \         | |  									 //
//                                           /  \   ___ _ __   | |__) |___  ___| |_ 									 //
//                                          / /\ \ / _ \ '_ \  |  _  // _ \/ __| __|									 //
//                                         / ____ \  __/ | | | | | \ \  __/ (__| |_ 									 //
//                                        /_/    \_\___|_| |_| |_|  \_\___|\___|\__|									 //
// --------------------------------------------------------------------------------------------------------------------- //

#include "MathFunctions.h"

namespace Aen {

	namespace Concealed {

		// ------------------------------------------------------------------------------------------------------------- //
		//                                                      Rect	                                                 //
		// ------------------------------------------------------------------------------------------------------------- //

		template<class T>
		struct TRect {

			TRect() = default;
			TRect(const TRect& rhs) = default;
			TRect(const T& x, const T& y, const T& width, const T& height) noexcept;
			TRect(TVec<T, 2>& origin, const T& width, const T& height) noexcept;
			TRect(const T& x, const T& y, const T& size) noexcept;
			TRect(const T& x, const T& y, const TVec<T, 2>& size) noexcept;
			TRect(const TVec<T, 2>& origin, const TVec<T, 2>& size) noexcept;
			TRect(const TVec<T, 2>& origin, const T& size) noexcept;
			TRect(const T& width, const T& height) noexcept;
			TRect(const T& size) noexcept;
			TRect(const TVec<T, 2>& size) noexcept;
			TRect(const TVec<T, 4>& rhs) noexcept;
			
			TRect& operator= (const TRect& rhs) noexcept;

			template<class T1> explicit operator TRect<T1>()const noexcept;

			friend std::ostream& operator<<	(std::ostream& out, const TRect& rhs) noexcept {
				out << "Origin: " << rhs.origin << std::endl << "Size: " << rhs.size;
				return out;
			}

			const TVec<T, 2> GetTL() const noexcept;
			const TVec<T, 2> GetTR() const noexcept;
			const TVec<T, 2> GetBL() const noexcept;
			const TVec<T, 2> GetBR() const noexcept;
			const T	GetL() const noexcept;
			const T	GetR() const noexcept;
			const T	GetT() const noexcept;
			const T	GetB() const noexcept;
			const T Area() const noexcept;

			TVec<T, 2> origin;
			TVec<T, 2> size;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T>
		inline TRect<T>::TRect(const T& x, const T& y, const T& width, const T& height) noexcept
			:origin(x, y), size(width, height) {}

		template<class T>
		inline TRect<T>::TRect(TVec<T, 2>& origin, const T& width, const T& height) noexcept
			:origin(origin), size(width, height) {}

		template<class T>
		inline TRect<T>::TRect(const T& x, const T& y, const T& size) noexcept
			:origin(x, y), size(size, size) {}

		template<class T>
		inline TRect<T>::TRect(const T& x, const T& y, const TVec<T, 2>& size) noexcept
			:origin(x, y), size(size) {}

		template<class T>
		inline TRect<T>::TRect(const TVec<T, 2>& origin, const TVec<T, 2>& size) noexcept
			:origin(origin), size(size) {}

		template<class T>
		inline TRect<T>::TRect(const TVec<T, 2>& origin, const T& size) noexcept
			:origin(origin), size(size, size) {}

		template<class T>
		inline TRect<T>::TRect(const T& width, const T& height) noexcept
			:origin(), size(width, height) {}

		template<class T>
		inline TRect<T>::TRect(const T& size) noexcept
			:origin(), size(size, size) {}

		template<class T>
		inline TRect<T>::TRect(const TVec<T, 2>& size) noexcept
			:origin(), size(size) {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T>
		inline TRect<T>& TRect<T>::operator=(const TRect& rhs) noexcept {
			origin = rhs.origin; 
			size = rhs.size; 
			return *this;
		}

		// -------------------------------------------- Cast Operators ------------------------------------------------- //

		template<class T>
		template<class T1>
		inline TRect<T>::operator TRect<T1>() const noexcept {
			TRect<T1> out;
			out.origin = static_cast<TVec<T1, 2>>(origin);
			out.size = static_cast<TVec<T1, 2>>(size);
			return out;
		}

		// -------------------------------------------- Get Operations ------------------------------------------------- //

		template<class T>
		inline const TVec<T, 2> TRect<T>::GetTL() const noexcept {
			return origin + TVec<T, 2>(Min((T)0, size.x), Max((T)0, size.y));
		}

		template<class T>
		inline const TVec<T, 2> TRect<T>::GetTR() const noexcept {
			return origin + TVec<T, 2>(Max((T)0, size.x), Max((T)0, size.y));
		}

		template<class T>
		inline const TVec<T, 2> TRect<T>::GetBL() const noexcept {
			return origin + TVec<T, 2>(Min((T)0, size.x), Min((T)0, size.y));
		}

		template<class T>
		inline const TVec<T, 2> TRect<T>::GetBR() const noexcept {
			return origin + TVec<T, 2>(Max((T)0, size.x), Min((T)0, size.y));
		}

		template<class T>
		inline const T TRect<T>::GetL() const noexcept {
			return origin.x + Min((T)0, size.x);
		}

		template<class T>
		inline const T TRect<T>::GetR() const noexcept {
			return origin.x + Max((T)0, size.x);
		}

		template<class T>
		inline const T TRect<T>::GetT() const noexcept {
			return origin.y + Max((T)0, size.y);
		}

		template<class T>
		inline const T TRect<T>::GetB() const noexcept {
			return origin.y + Min((T)0, size.y);
		}

		template<class T>
		inline const T TRect<T>::Area() const noexcept {
			return size.x * size.y;
		}
	}

	// ------------------------------------------------------- Type Alias ---------------------------------------------- //

	using Rectd = Concealed::TRect<double>;
	using Rectf = Concealed::TRect<float>;
	using Recti = Concealed::TRect<int>;

	// ----------------------------------------------------------------------------------------------------------------- //
}