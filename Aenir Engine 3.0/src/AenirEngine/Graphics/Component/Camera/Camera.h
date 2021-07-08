#pragma once
#include"../../Graphics.h"

namespace Aen {

	class Camera {
		public:
		Camera();

		void SetCameraPerspective(const float& fov, const float& aRatio, const float& minZ, const float& maxZ);
		void SetCameraOrthographic(const float& width, const float& height, const float& minZ, const float& maxZ);

		const Mat4f GetVPMatrix() const;
		const Mat4f& GetView() const;
		const Mat4f& GetProjecton() const;

		private:
		~Camera() = default;

		Mat4f m_view;
		Mat4f m_projection;

		friend class CameraHandler;
	};
}
