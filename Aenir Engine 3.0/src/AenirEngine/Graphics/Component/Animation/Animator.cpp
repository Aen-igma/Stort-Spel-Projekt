#include"PCH.h"
#include"Animator.h"
#include"Core/Renderer.h"

namespace Aen {
	Animator::Animator(const size_t& id)
		:Drawable(id), animation(nullptr)
	{
	}

	void Animator::SetAnimation(Animation& anim)
	{
		this->animation = &anim;
	}

	void Animator::Draw(Renderer& renderer, const uint32_t& layer)
	{
		if (animation) {
			Mat4f m = EntityHandler::GetEntity(m_id).GetTransformation();
			renderer.m_cbTransform.GetData().m_mdlMat = m.Transposed();
			renderer.m_cbTransform.UpdateBuffer();

			renderer.m_collisionBuffer.BindBuffer<PShader>(0);
			renderer.m_collisionBuffer.GetData().color = { 1.0f, 0.f, 0.f };
			renderer.m_collisionBuffer.UpdateBuffer();
			
			renderer.m_cbTransform.BindBuffer<VShader>(0);
			RenderSystem::BindShader(renderer.m_collisionPS);
			RenderSystem::BindShader(renderer.m_opaqueVS);
			RenderSystem::SetPrimitiveTopology(Topology::LINELIST);
			RenderSystem::BindRenderTargetView(renderer.m_backBuffer);

			animation->m_finalMatrix.BindSRV<VShader>(0);
			animation->vBuff.BindBuffer();
			//animation->vBuff.Draw();
			animation->m_indexBuffer.BindBuffer();
			animation->m_indexBuffer.DrawIndexed();
			RenderSystem::UnBindShaderResources<VShader>(0, 1);
		}
	}

	void Animator::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{
	}
}