#include "PCH.h"
#include "UI.h"
#include "Core\GlobalSettings.h"

namespace Aen {

    UI::UI(const size_t& id)
        :Drawable(m_id)
    {
        width = 700.f / GlobalSettings::GetWindow()->GetSize().x * 2.f - 1.f;
        height = 300.f / GlobalSettings::GetWindow()->GetSize().y * 2.f - 1.f;

        Vertex2D DARR[6]
        { 
            Vertex2D(Vec2f(-width, -height), Vec2f(0.f, 1.f)),
            Vertex2D(Vec2f(width, height), Vec2f(1.f, 0.f)),
            Vertex2D(Vec2f(-width, height), Vec2f(0.f, 0.f)),

            Vertex2D(Vec2f(-width, -height), Vec2f(0.f, 1.f)),
            Vertex2D(Vec2f(width, -height), Vec2f(1.f, 1.f)),
            Vertex2D(Vec2f(width, height), Vec2f(1.f, 0.f))
        };
        
        if (!m_vertices.Create(DARR, 6))
            throw;
    }

    UI::~UI()
    {}

    void UI::SetPos(const Vec2f & pos)
    {

    }

    void UI::SetPos(const float& x, const float& y)
    {

    }

    const Vec2f UI::GetPos() const
    {
        return Vec2f();
    }

    const Vec2f UI::GetScale() const
    {
        return Vec2f();
    }

    void Aen::UI::Draw(Renderer& renderer, const uint32_t& layer)
    {
        renderer.m_2DTransform.GetData() = m_transform;
        renderer.m_2DTransform.UpdateBuffer();

        renderer.m_2DTransform.BindBuffer<VShader>(0);
        RenderSystem::BindShader(renderer.m_2DVShader);
        RenderSystem::BindShader(renderer.m_2DPShader);
        RenderSystem::BindRenderTargetView(renderer.m_backBuffer);
        RenderSystem::SetRasteriserState(renderer.m_rasterizerState);
        RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);

        m_vertices.BindBuffer();
        m_vertices.Draw();
    }

    void Aen::UI::DepthDraw(Renderer& renderer, const uint32_t& layer)
    {

    }

    bool Aen::UI::FrustumCull(Renderer& renderer)
    {
        return true;
    }
}