#pragma once
#include"CameraHandler.h"

namespace Aen {

	class Camera {
		public:
		~Camera();
		Camera() = delete;
		Camera(const uint32_t& id);

		void SetCameraPerspective(const float& fov, const float& aRatio, const float& minZ, const float& maxZ);
		void SetCameraOrthographic(const float& width, const float& height, const float& minZ, const float& maxZ);

		const Mat4f GetVPMatrix() const;
		const Mat4f& GetView() const;
		const Mat4f& GetProjecton() const;

		private:
		Mat4f m_view;
		Mat4f m_projection;
		uint32_t m_id;
	};
}
