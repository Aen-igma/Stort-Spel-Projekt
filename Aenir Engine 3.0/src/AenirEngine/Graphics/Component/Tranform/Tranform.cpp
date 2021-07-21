#include "PCH.h"
#include "Tranform.h"

namespace Aen {

    void Translation::SetPos(const Vec3f& pos) {
        m_pos = pos;
    }

    void Translation::SetPos(const float& x, const float& y, const float& z) {
        m_pos = {x, y, z};
    }

    void Translation::Move(const Vec3f& pos) {
        m_pos += pos;
    }

    void Translation::Move(const float& x, const float& y, const float& z) {
        m_pos += {x, y, z};
    }

    const Vec3f& Translation::GetPos() {
        return m_pos;
    }

    const Mat4f Translation::GetTranform() {
        return MatTranslate(m_pos);
    }
    


    void Rotation::SetRot(const Vec3f& rot) {
        m_rot = rot;
    }

    void Rotation::SetRot(const float& p, const float& y, const float& r) {
        m_rot = {p, y, r};
    }

    void Rotation::Rotate(const Vec3f& ang) {
        m_rot += ang;
    }

    void Rotation::Rotate(const float& p, const float& y, const float& r) {
        m_rot += {p, y, r};
    }

    const Vec3f& Rotation::GetRot() {
        return m_rot;
    }

    const Mat4f Rotation::GetTranform() {
        return MatRotate(m_rot);
    }
    


    Scale::Scale()
        :m_scale(Vec3f::one) {}

    void Scale::SetScale(const Vec3f& scale) {
        m_scale = scale;
    }

    void Scale::SetScale(const float& x, const float& y, const float& z) {
        m_scale = {x, y, z};
    }

    const Vec3f& Scale::GetScale() {
        return m_scale;
    }

    const Mat4f Scale::GetTranform() {
        return MatScale(m_scale);
    }

}