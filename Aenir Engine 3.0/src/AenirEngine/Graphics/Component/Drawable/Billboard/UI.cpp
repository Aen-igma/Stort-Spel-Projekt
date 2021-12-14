#include "PCH.h"
#include "UI.h"
#include "Core\GlobalSettings.h"

namespace Aen {

    UI::UI(const size_t& id)
        :Drawable(m_id)
    {
        float width = 200.f / GlobalSettings::GetWindow()->GetSize().x * 2.f - 1.f;
        float height = 200.f / GlobalSettings::GetWindow()->GetSize().y * 2.f - 1.f;

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

    void UI::Load(const std::string& dir)
    {

    }

    void Aen::UI::Draw(Renderer& renderer, const uint32_t& layer = 0)
    {
        renderer.m_2DTransform.GetData() = m_transform;
        renderer.m_2DTransform.UpdateBuffer();

        renderer.m_2DTransform.BindBuffer<VShader>(0);
        RenderSystem::BindShader(renderer.m_2DVShader);
        RenderSystem::BindShader(renderer.m_2DPShader);
        RenderSystem::BindRenderTargetView(renderer.m_backBuffer);

        m_vertices.BindBuffer();
        m_vertices.Draw();
    }

    void Aen::UI::DepthDraw(Renderer& renderer, const uint32_t& layer = 0)
    {

    }

    bool Aen::UI::FrustumCull(Renderer& renderer)
    {
        return true;
    }
}