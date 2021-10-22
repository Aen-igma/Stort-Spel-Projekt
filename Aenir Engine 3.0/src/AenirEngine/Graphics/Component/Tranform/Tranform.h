#pragma once
#include"../../Graphics.h"

namespace Aen {

	class AEN_DECLSPEC Translation {
		public:
		Translation() = default;

		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);
		void Move(const Vec3f& pos);
		void Move(const float& x, const float& y, const float& z);

		const Vec3f& GetPos();
		const Mat4f GetTranform();

		private:
		~Translation() = default;

		Vec3f m_pos;

		friend class ComponentHandler;
	};

	class AEN_DECLSPEC Rotation {
		public:
		Rotation() = default;

		void SetRot(const Vec3f& rot);
		void SetRot(const float& p, const float& y, const float& r);
		void Rotate(const Vec3f& ang);
		void Rotate(const float& p, const float& y, const float& r);

		const Vec3f& GetRot();
		const Mat4f GetTranform();

		private:
		~Rotation() = default;

		Vec3f m_rot;

		friend class ComponentHandler;
	};

	class AEN_DECLSPEC Scale {
		public:
		Scale();

		void SetScale(const Vec3f& scale);
		void SetScale(const float& x, const float& y, const float& z);

		const Vec3f& GetScale();
		const Mat4f GetTranform();

		private:
		~Scale() = default;

		Vec3f m_scale;

		friend class ComponentHandler;
	};
}