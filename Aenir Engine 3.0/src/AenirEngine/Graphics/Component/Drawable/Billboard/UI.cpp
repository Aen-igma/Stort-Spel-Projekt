#include "PCH.h"
#include "UI.h"

namespace Aen {

    UI::UI():Drawable(m_id)
    {}

    UI::~UI()
    {}

    void UI::Load(const std::string& dir)
    {

    }

    void Aen::UI::Draw(Renderer& renderer, const uint32_t& layer = 0)
    {

    }

    void Aen::UI::DepthDraw(Renderer& renderer, const uint32_t& layer = 0)
    {

    }

    bool Aen::UI::FrustumCull(Renderer& renderer)
    {
        return true;
    }
}