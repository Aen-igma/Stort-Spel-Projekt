#include"PCH.h"
#include"Animator.h"
#include"Core/Renderer.h"

namespace Aen {
	void Animator::Update()
	{
		if (animationIndex < m_animationList.size() && pause) {
			Animation* animation = m_animationList[animationIndex].second;
			m_end = ResClock::now();
			while (std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start) > frameRate) {

				m_start = ResClock::now();
				std::vector<Mat4f> anim;
				GetAnimation(anim);

				std::vector<Mat4f> localTran(animation->m_boneArray.size());
				std::vector<Mat4f> modelTran(animation->m_boneArray.size());

				for (int i = 0; i < animation->m_boneArray.size(); i++) {
					localTran[i] = animation->m_boneArray[i].localMatrix * anim[i].Transposed();
				}

				modelTran[0] = localTran[0];

				for (int i = 1; i < animation->m_boneArray.size(); i++) {
					int parent = animation->m_boneArray[i].parentID;
					modelTran[i] = modelTran[parent] * localTran[i];
				}

				for (int i = 0; i < animation->m_boneArray.size(); i++) {
					animation->m_finalMatrix.GetData(i) = modelTran[i] * animation->m_boneArray[i].offsetMatrix;
				}

				if (m_currentFrame > 1) {
					animation->m_finalMatrix.UpdateBuffer();
				}
			}
		}
	}

	void Animator::GetAnimation(std::vector<Mat4f>& mat)
	{
		Animation* animation = m_animationList[animationIndex].second;

		int sizeBA = animation->m_boneArray.size();
		int numFrames = animation->m_timeStamp.size();
		m_frameTime = ResClock::now();
		DurationLD duration = std::chrono::duration_cast<std::chrono::nanoseconds>(m_frameTime - m_currentTime);

		if (duration.count() > animation->m_duration * m_scale) {
			m_currentFrame = 0;
			m_currentTime = ResClock::now();
		}

		if (m_currentFrame < (numFrames-2)) {

			if (duration.count() > animation->m_timeStamp[m_currentFrame + 1] * m_scale) {
				m_currentFrame++;
			}

			float f = duration.count() - (animation->m_timeStamp[m_currentFrame] * m_scale);
			float h = (animation->m_timeStamp[m_currentFrame + 1] * m_scale) - (animation->m_timeStamp[m_currentFrame] * m_scale);
			float t = f / h;

			for (int i = 0; i < sizeBA; i++) {
				std::string bName = animation->m_boneArray[i].boneName;
				Mat4f currentFrame = animation->m_keyFrames.at(bName)[m_currentFrame].rotation;
				Mat4f nextFrame = animation->m_keyFrames.at(bName)[m_currentFrame + 1].rotation;

				mat.emplace_back(Lerp(currentFrame, nextFrame, t));
			}
		}
		else {
			for (int i = 0; i < sizeBA; i++) {
				std::string bName = animation->m_boneArray[i].boneName;
				Mat4f currentFrame = animation->m_keyFrames.at(bName)[m_currentFrame].rotation;

				mat.emplace_back(currentFrame);
			}
		}
	}

	void Animator::BindBuffer()
	{
		if (animationIndex < m_animationList.size()) {
			m_animationList[animationIndex].second->m_finalMatrix.BindSRV<VShader>(0);
		}
	}

	bool Animator::HasAnimation(const std::string& anim)
	{
		for (auto& i : m_animationList) {
			if (i.first == anim)
				return true;
		}
		return false;
	}

	void Animator::SetAnimationScale(const float& newScale)
	{
		m_scale = newScale;
	}

	void Animator::RemoveAnimation(const std::string& animName)
	{
		if (!HasAnimation(animName))
			return;

		for (int i = 0; i < m_animationList.size(); i++) {
			if (m_animationList[i].first == animName) {
				m_animationList.erase(m_animationList.begin() + i);
				break;
			}
		}
		if (animationIndex > m_animationList.size())
			animationIndex = m_animationList.size()-1;
	}

	void Animator::Pause()
	{
		pause = false;
	}

	void Animator::Run()
	{
		pause = true;
	}

	void Animator::Reset()
	{
		m_currentFrame = 0;
		m_currentTime = ResClock::now();
	}

	Animator::Animator(const size_t& id)
		:Drawable(id), m_scale(1), m_currentFrame(0), animationIndex(0)
	{
		m_start = m_end = ResClock::now();
		SetFrameRate(60);
	}

	void Animator::AddAnimation(Animation& anim, const std::string& name)
	{
		if (HasAnimation(name))
			return;

		std::pair<std::string, Animation*> animation(name, &anim);
		m_animationList.emplace_back(animation);
	}

	void Animator::AddAnimation(const std::string& animName, const std::string& name)
	{
		if (HasAnimation(name))
			return;

		if (!Resource::AnimationExist(animName)) throw;
		std::pair<std::string, Animation*> animation(name, &Resource::GetAnimation(animName));
		m_animationList.emplace_back(animation);
	}

	void Animator::SetAnimation(const std::string& animName)
	{
		if (!HasAnimation(animName))
			return;

		for (int i = 0; i < m_animationList.size();i++) {
			if (m_animationList[i].first == animName) {
				animationIndex = i;
				break;
			}
		}
	}

	void Animator::SetFrameRate(const int& frameRate)
	{
		int ft = (int)(((double)1 / (double)frameRate) * (double)pow(10, 9));
		this->frameRate = std::chrono::nanoseconds{ ft };
	}

	void Animator::Draw(Renderer& renderer, const uint32_t& layer)
	{
		if (animationIndex < m_animationList.size()) {
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

			m_animationList[animationIndex].second->m_finalMatrix.BindSRV<VShader>(0);
			m_animationList[animationIndex].second->vBuff.BindBuffer();
			m_animationList[animationIndex].second->m_indexBuffer.BindBuffer();
			m_animationList[animationIndex].second->m_indexBuffer.DrawIndexed();
			RenderSystem::UnBindShaderResources<VShader>(0, 1);
		}
	}

	void Animator::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{
	}

	bool Animator::FrustumCull(Renderer& renderer)
	{
		return true;
	}
}