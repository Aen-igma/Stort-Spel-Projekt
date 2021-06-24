#pragma once

// --------------------------------------------------------------------------------------------------------------------- //
//                                          /\                / ____|    | |           								     //
//                                         /  \   ___ _ __   | |     ___ | | ___  _ __ 								     //
//                                        / /\ \ / _ \ '_ \  | |    / _ \| |/ _ \| '__|								     //
//                                       / ____ \  __/ | | | | |___| (_) | | (_) | |   								     //
//                                      /_/    \_\___|_| |_|  \_____\___/|_|\___/|_|   								     //
// --------------------------------------------------------------------------------------------------------------------- //

#include"MathFunctions.h"

namespace Aen {
	
	namespace Concealed {

		template<class T>
		struct TColor : public TVecN<T, 4> {

			using TVecN<T, 4>::r;
			using TVecN<T, 4>::g;
			using TVecN<T, 4>::b;
			using TVecN<T, 4>::a;
			using TVecN<T, 4>::h;
			using TVecN<T, 4>::s;
			using TVecN<T, 4>::v;

			TColor() noexcept;
			TColor(const TColor<T>& rhs) = default;
			template <class ...Ts>
			TColor(const T& t, const Ts& ...ts) noexcept;
			template <class ...Ts, uint32_t Ns>
			TColor(const TVec<T, Ns>& t, const Ts& ...ts) noexcept;

			TColor& operator= (const TColor& rhs) noexcept;
			TColor& operator= (const TVec<T, 4>& rhs) noexcept;
			TColor& operator= (const TVec<T, 3>& rhs) noexcept;

			T& operator[] (const uint32_t& i) throw();
			const T& operator[] (const uint32_t& i) const throw();

			template<class T1, uint32_t N1> explicit operator TVec<T1, N1>() const noexcept;

			friend std::ostream& operator<<	(std::ostream& out, const TColor& rhs) noexcept {
				out << "[";
				for(uint32_t i = 0; i < 3; i++) out << std::to_string(rhs[i]) + ", ";
				out << std::to_string(rhs[3]) + "]";
				return out;
			}

			void SetColorRGB(const TVec<T, 4>& clr) noexcept;
			void SetColorRGB(const TVec<T, 3>& clr) noexcept;
			void SetColorRGB(const T& r, const T& g, const T& b, const T& a) noexcept;
			void SetColorRGB(const T& r, const T& g, const T& b) noexcept;
			void SetColorHSV(const TVec<T, 4>& clr) noexcept;
			void SetColorHSV(const TVec<T, 3>& clr) noexcept;
			void SetColorHSV(const T& h, const T& s, const T& v, const T& a) noexcept;
			void SetColorHSV(const T& h, const T& s, const T& v) noexcept;

			const TColor& GetRGB() const noexcept;
			const TColor GetHSV() const noexcept;

			static const TColor White;
			static const TColor Gray;
			static const TColor Black;
			static const TColor Red;
			static const TColor Green;
			static const TColor Blue;
			static const TColor Magenta;
			static const TColor Yellow;
			static const TColor Cyan;
			static const TColor Orange;
			static const TColor Brown;
			static const TColor Lime;
			static const TColor Violet;
			static const TColor Purple;
			static const TColor Pink;
		};

		// ---------------------------------------------- Constructor -------------------------------------------------- //

		template<class T>
		template<class ...Ts>
		inline TColor<T>::TColor(const T& t, const Ts & ...ts) noexcept
			:TVecN<T, 4>{t, ts...} {}

		template<class T>
		template<class ...Ts, uint32_t Ns>
		inline TColor<T>::TColor(const TVec<T, Ns>& t, const Ts & ...ts) noexcept 
			:TVecN<T, 4>{t, ts...} {}

		// ------------------------------------------ Assignment Operators --------------------------------------------- //

		template<class T>
		inline TColor<T>::TColor() noexcept {
			for(uint32_t i = 0; i < 4; i++) (*this)[i] = 0;
		}

		template<class T>
		inline TColor<T>& TColor<T>::operator= (const TColor& rhs) noexcept {
			r = rhs.r;
			g = rhs.g;
			b = rhs.b;
			a = rhs.a;
			return *this;
		}

		template<class T>
		inline TColor<T>& TColor<T>::operator= (const TVec<T, 4>& rhs) noexcept {
			r = rhs.r;
			g = rhs.g;
			b = rhs.b;
			a = rhs.a;
			return *this;
		}

		template<class T>
		inline TColor<T>& TColor<T>::operator= (const TVec<T, 3>& rhs) noexcept {
			r = rhs.r;
			g = rhs.g;
			b = rhs.b;
			return *this;
		}

		// ----------------------------------------- Member Access Operators ------------------------------------------- //

		template<class T>
		inline T& TColor<T>::operator[](const uint32_t& i) throw() {
			if(i < 4) return reinterpret_cast<T*>(this)[i];
		}

		template<class T>
		inline const T& TColor<T>::operator[](const uint32_t& i) const throw() {
			if(i < 4) return reinterpret_cast<const T*>(this)[i];
		}

		// -------------------------------------------- Cast Operators ------------------------------------------------- //

		template<class T>
		template<class T1, uint32_t N1>
		inline TColor<T>::operator TVec<T1, N1>() const noexcept {
			TColor<T1> out;
			out.r = static_cast<T1>(r);
			out.g = static_cast<T1>(g);
			out.b = static_cast<T1>(b);
			out.a = static_cast<T1>(a);
			return out;
		}
		
		// -------------------------------------------- Set Operations ------------------------------------------------- //

		template<class T>
		inline void TColor<T>::SetColorRGB(const TVec<T, 4>& clr) noexcept {
			r = clr.r;
			g = clr.g;
			b = clr.b;
			a = clr.a;
		}

		template<class T>
		inline void TColor<T>::SetColorRGB(const TVec<T, 3>& clr) noexcept {
			r = clr.r;
			g = clr.g;
			b = clr.b;
		}

		template<class T>
		inline void TColor<T>::SetColorRGB(const T& r, const T& g, const T& b, const T& a) noexcept {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		template<class T>
		inline void TColor<T>::SetColorRGB(const T& r, const T& g, const T& b) noexcept {
			this->r = r;
			this->g = g;
			this->b = b;
		}

		template<class T>
		inline void TColor<T>::SetColorHSV(const TVec<T, 4>& clr) noexcept {
			T C = clr.g * clr.b;
			T X = C * (1 - Abs(std::fmod(clr.r / (T)60, 2) - 1));
			T m = clr.g - C;

			if(clr.g >= 0 && clr.g < 60) {r = C, g = X, b = 0;} 
			else if(clr.g >= 60 && clr.g < 120) {r = X, g = C, b = 0;} 
			else if(clr.g >= 120 && clr.g < 180) {r = 0, g = C, b = X;} 
			else if(clr.g >= 180 && clr.g < 240) {r = 0, g = X, b = C;} 
			else if(clr.g >= 240 && clr.g < 300) {r = X, g = 0, b = C;} 
			else {r = C, g = 0, b = X;}
			a = clr.a;
		}

		template<class T>
		inline void TColor<T>::SetColorHSV(const TVec<T, 3>& clr) noexcept {
			T C = clr.g * clr.b;
			T X = C * (1 - Abs(std::fmod(clr.r / (T)60, 2) - 1));
			T m = clr.g - C;

			if(clr.g >= 0 && clr.g < 60) {r = C, g = X, b = 0;} 
			else if(clr.g >= 60 && clr.g < 120) {r = X, g = C, b = 0;} 
			else if(clr.g >= 120 && clr.g < 180) {r = 0, g = C, b = X;} 
			else if(clr.g >= 180 && clr.g < 240) {r = 0, g = X, b = C;} 
			else if(clr.g >= 240 && clr.g < 300) {r = X, g = 0, b = C;} 
			else {r = C, g = 0, b = X;}
		}

		template<class T>
		inline void TColor<T>::SetColorHSV(const T& h, const T& s, const T& v, const T& a) noexcept {
			T C = s * v;
			T X = C * ((T)1 - (T)Abs(std::fmod(h / (T)60, 2) - (T)1));
			T m = s - C;

			if(s >= 0 && s < 60) {r = C, g = X, b = 0;} 
			else if(s >= 60 && s < 120) {r = X, g = C, b = 0;} 
			else if(s >= 120 && s < 180) {r = 0, g = C, b = X;} 
			else if(s >= 180 && s < 240) {r = 0, g = X, b = C;} 
			else if(s >= 240 && s < 300) {r = X, g = 0, b = C;} 
			else {r = C, g = 0, b = X;}
			this->a = a;
		}

		template<class T>
		inline void TColor<T>::SetColorHSV(const T& h, const T& s, const T& v) noexcept {
			T C = s * v;
			T X = C * (1 - Abs(std::fmod(h / (T)60, 2) - 1));
			T m = s - C;

			if(s >= 0 && s < 60) {r = C, g = X, b = 0;} 
			else if(s >= 60 && s < 120) {r = X, g = C, b = 0;} 
			else if(s >= 120 && s < 180) {r = 0, g = C, b = X;} 
			else if(s >= 180 && s < 240) {r = 0, g = X, b = C;} 
			else if(s >= 240 && s < 300) {r = X, g = 0, b = C;} 
			else {r = C, g = 0, b = X;}
		}

		// -------------------------------------------- Get Operations ------------------------------------------------- //

		template<class T>
		inline const TColor<T>& TColor<T>::GetRGB() const noexcept {
			return *this;
		}

		template<class T>
		inline const TColor<T> TColor<T>::GetHSV() const noexcept {
			TColor<T> out;
			T min, max, delta;

			min = (r < g) ? r : g;
			min = (min < b) ? min : b;

			max = (r > g) ? r : g;
			max = (max > b) ? max : b;

			out.v = max;
			delta = max - min;
			if(delta < 0) {
				out.h = 0;
				out.s = 0;
				return out;
			}

			if(max > 0) {
				out.s = (delta / max);
			} else {
				out.h = 0;
				out.s = 0;
				return out;
			}

			if(r >= max)
				out.h = (g - b) / delta;
			else if(g >= max)
				out.h = 2 + (b - r) / delta;
			else
				out.h = 4 + (r - g) / delta;

			out.h *= 60;

			if(out.h < 0)
				out.h += 360;

			out.a = a;
			return out;
		}

		// ------------------------------------------- Constant Variables ---------------------------------------------- //

		template<class T> const TColor<T> TColor<T>::White(1, 1, 1, 1);
		template<class T> const TColor<T> TColor<T>::Gray(0.5, 0.5, 0.5, 1);
		template<class T> const TColor<T> TColor<T>::Black(0, 0, 0, 1);
		template<class T> const TColor<T> TColor<T>::Red(1, 0, 0, 1);
		template<class T> const TColor<T> TColor<T>::Green(0, 1, 0, 1);
		template<class T> const TColor<T> TColor<T>::Blue(0, 0, 1, 1);
		template<class T> const TColor<T> TColor<T>::Magenta(1, 0, 1, 1);
		template<class T> const TColor<T> TColor<T>::Yellow(1, 1, 0, 1);
		template<class T> const TColor<T> TColor<T>::Cyan(0, 1, 1, 1);
		template<class T> const TColor<T> TColor<T>::Orange(1, 1.55, 0, 1);
		template<class T> const TColor<T> TColor<T>::Brown(1.55, 0.16, 0.16, 1);
		template<class T> const TColor<T> TColor<T>::Lime(0.25, 1, 0, 1);
		template<class T> const TColor<T> TColor<T>::Violet(0.5, 0, 1, 1);
		template<class T> const TColor<T> TColor<T>::Purple(0.5, 0, 0.5, 1);
		template<class T> const TColor<T> TColor<T>::Pink(1, 0.75, 0.79, 1);
}

	// --------------------------------------------------- Type Alias -------------------------------------------------- //

	using Color = Concealed::TColor<float>;

	// ----------------------------------------------------------------------------------------------------------------- //
}
