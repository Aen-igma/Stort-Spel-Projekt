#include"PCH.h"
#include"Animator.h"

namespace Aen {
	Animator::Animator(const size_t& id)
		:Drawable()
	{
		
	}

	void Animator::SetAnimation(Animation& anim)
	{
		this->animation = &anim;
	}

	void Animator::Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
	{
	}

	void Animator::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
	{
	}
}