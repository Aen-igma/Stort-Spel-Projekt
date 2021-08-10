#pragma once
#include"../../Graphics.h"

namespace Aen {

	class AEN_DECLSPEC Camera {
		public:
		Camera();

		void SetCameraPerspective(const float& fov, const float& aRatio, const float& minZ, const float& maxZ);
		void SetCameraOrthographic(const float& width, const float& height, const float& minZ, const float& maxZ);

		const Vec3f GetForward();
		const Vec3f GetUp();
		const Vec3f GetRight();
		const Mat4f GetVPMatrix() const;
		const Mat4f& GetView() const;
		const Mat4f& GetProjecton() const;

		private:
		~Camera() = default;

		void UpdateView(const Vec3f& pos, const Vec3f& rot);

		Mat4f m_view;
		Mat4f m_projection;
		Vec3f m_forwardVec;
		Vec3f m_upVec;

		friend class ComponentHandler;
		friend class Renderer;
	};
}