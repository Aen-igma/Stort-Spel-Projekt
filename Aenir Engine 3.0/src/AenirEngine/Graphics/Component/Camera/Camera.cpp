#include "PCH.h"
#include "Camera.h"

namespace Aen {

    Camera::Camera()
        :m_view(Mat4f::identity), m_projection(Mat4f::identity) {}

    void Camera::SetCameraPerspective(const float& fov, const float& aRatio, const float& minZ, const float& maxZ) {
        m_projection = MatPerspective<float>(fov, aRatio, minZ, maxZ);
    }

    void Camera::SetCameraOrthographic(const float& width, const float& height, const float& minZ, const float& maxZ) {
        m_projection = MatOrthographic<float>(-width * 0.5f, width * 0.5f, height * 0.5f, -height * 0.5f, minZ, maxZ);
    }

    const Mat4f Camera::GetVPMatrix() const {
        return m_view * m_projection;
    }

    const Mat4f& Camera::GetView() const {
        return m_view;
    }

    const Mat4f& Camera::GetProjecton() const {
        return m_projection;
    }
}