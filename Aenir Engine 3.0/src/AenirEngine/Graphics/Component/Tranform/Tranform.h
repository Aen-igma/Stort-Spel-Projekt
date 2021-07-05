#pragma once
#include"TranformHandler.h"

namespace Aen {

	class Translation {
		public:
		~Translation();
		Translation() = delete;
		Translation(const uint32_t& id);

		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);
		void Move(const Vec3f& pos);
		void Move(const float& x, const float& y, const float& z);

		const Vec3f& GetPos();
		const Mat4f GetTranform();

		private:
		Vec3f m_pos;
		uint32_t m_id;
	};

	class Rotation {
		public:
		~Rotation();
		Rotation() = delete;
		Rotation(const uint32_t& id);

		void SetRot(const Vec3f& rot);
		void SetRot(const float& p, const float& y, const float& r);
		void Rotate(const Vec3f& ang);
		void Rotate(const float& p, const float& y, const float& r);

		const Vec3f& GetRot();
		const Mat4f GetTranform();

		private:
		Vec3f m_rot;
		uint32_t m_id;
	};

	class Scale {
		public:
		~Scale();
		Scale() = delete;
		Scale(const uint32_t& id);

		void SetScale(const Vec3f& scale);
		void SetScale(const float& x, const float& y, const float& z);

		const Vec3f& GetScale();
		const Mat4f GetTranform();

		private:
		Vec3f m_scale;
		uint32_t m_id;
	};
}