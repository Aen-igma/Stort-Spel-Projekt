#include"PCH.h"
#include"Animator.h"
#include"Core/Renderer.h"
#include<omp.h>

namespace Aen {

	void Animator::Update() {
		if (animationIndex < m_animationList.size()) {
			Animation* animation = m_animationList[animationIndex].second;

			m_sEnd = omp_get_wtime();
			while(m_sEnd - m_sStart > m_frameRate) {
				double dTime = m_sEnd - m_sStart;
				m_sStart = omp_get_wtime();

				if(!m_pause) {
					std::vector<Mat4f> anim;
					GetAnimation(anim, (float)dTime);

					std::vector<Mat4f> localTran(animation->m_boneArray.size());
					std::vector<Mat4f> modelTran(animation->m_boneArray.size());

					for(int i = 0; i < animation->m_boneArray.size(); i++) {
						localTran[i] = animation->m_boneArray[i].localMatrix * anim[i].Transposed();
					}

					modelTran[0] = localTran[0];

					for(int i = 1; i < animation->m_boneArray.size(); i++) {
						int parent = animation->m_boneArray[i].parentID;
						modelTran[i] = modelTran[parent] * localTran[i];
					}

					for(int i = 0; i < animation->m_boneArray.size(); i++) {
						animation->m_finalMatrix.GetData(i) = modelTran[i] * animation->m_boneArray[i].offsetMatrix;
					}
				}

				animation->m_finalMatrix.UpdateBuffer();
			}
		}
	}

	void Animator::GetAnimation(std::vector<Mat4f>& mat, const float& deltaTime) {

		Animation* animation = m_animationList[animationIndex].second;
		Animation* runLayer = m_hasRunLayer ? m_animationList[m_runIndex].second : nullptr;
		Animation* actionLayer = m_hasActionLayer ? m_animationList[m_actionIndex].second : nullptr;

		uint16_t sizeBA = animation->m_boneArray.size();

		uint16_t baseNumFrames = animation->m_timeStamp.size();
		uint16_t runNumFrames = m_hasRunLayer ? runLayer->m_timeStamp.size() : 0.f;
		uint16_t actionNumFrames = m_hasActionLayer ? actionLayer->m_timeStamp.size() : 0.f;
		
		float duration = animation->GetDuration();

		if (m_hasRunLayer)
			duration = Aen::Max(duration, runLayer->GetDuration());
		if (m_hasActionLayer)
			duration = Aen::Max(duration, actionLayer->GetDuration());
			
		duration *= m_scale;

		if (m_time < duration) 
		{
			uint16_t l = 0u;
			uint16_t r = baseNumFrames;
			uint16_t mid = 0u;

			uint16_t fFrame = 0u;
			uint16_t sFrame = 0u;

			while (true) {
				mid = (l + r) / 2u;
				float ft = animation->m_timeStamp[mid] * duration;
				float st = animation->m_timeStamp[Clamp(mid + 1u, 0u, baseNumFrames - 1u)] * duration;
				if ((m_time >= ft && m_time <= st) || ft == st) {
					fFrame = mid;
					sFrame = (mid + 1u) % baseNumFrames;
					break;
				}

				if (m_time < ft)
					r = mid;

				if (m_time > ft)
					l = mid;
			}

			float f = m_time - animation->m_timeStamp[fFrame] * duration;
			float h = animation->m_timeStamp[sFrame] * duration - animation->m_timeStamp[fFrame] * duration;
			float t = f / h;

			uint16_t baseFrame = fFrame % baseNumFrames;
			uint16_t layerFrame = m_hasRunLayer ? fFrame % runNumFrames : 0.f;
			uint16_t actionFrame = m_hasActionLayer ? fFrame % actionNumFrames : 0.f;

			for (int i = 0; i < sizeBA; i++) {
				std::string bName = animation->m_boneArray[i].boneName;
				Mat4f currentFrame;
				Mat4f nextFrame;
				if (m_hasRunLayer)
				{
					sm::Matrix currentRot0 = animation->m_keyFrames.at(bName)[baseFrame].rotation.smMat;
					sm::Matrix currentRot1 = runLayer->m_keyFrames.at(bName)[layerFrame].rotation.smMat;
					sm::Matrix currentBlendRot = currentBlendRot.Lerp(currentRot0, currentRot1, m_runFactor);
					currentFrame.smMat = currentBlendRot;

					sm::Matrix nextRot0 = animation->m_keyFrames.at(bName)[sFrame % baseNumFrames].rotation.smMat;
					sm::Matrix nextRot1 = runLayer->m_keyFrames.at(bName)[sFrame % runNumFrames].rotation.smMat;
					sm::Matrix nextBlendRot = currentBlendRot.Lerp(nextRot0, nextRot1, m_runFactor);

					nextFrame.smMat = nextBlendRot;
				}
				else
				{
					currentFrame = animation->m_keyFrames.at(bName)[fFrame % baseNumFrames].rotation;
					nextFrame = animation->m_keyFrames.at(bName)[sFrame % baseNumFrames].rotation;
				}

				if (m_hasActionLayer)
				{
					sm::Matrix current = actionLayer->m_keyFrames.at(bName)[actionFrame].rotation.smMat;
					sm::Matrix next = actionLayer->m_keyFrames.at(bName)[sFrame % actionNumFrames].rotation.smMat;

					currentFrame.smMat = current.Lerp(currentFrame.smMat, current, m_actionFactor);
					nextFrame.smMat = next.Lerp(nextFrame.smMat, next, m_actionFactor);
				}
				mat.emplace_back(Lerp(currentFrame, nextFrame, t));
			}

			m_time += deltaTime;
		}
		else
		{
			if(m_loop)
				m_time = 0.f;

			for (int i = 0; i < sizeBA; i++) {
				std::string bName = animation->m_boneArray[i].boneName;
				Mat4f currentFrame;
				if (m_hasRunLayer)
				{
					sm::Matrix rot0 = animation->m_keyFrames.at(bName)[baseNumFrames - 1].rotation.smMat;
					sm::Matrix rot1 = runLayer->m_keyFrames.at(bName)[runNumFrames - 1].rotation.smMat;
					sm::Matrix blendRot = blendRot.Lerp(rot0, rot1, m_runFactor);
					currentFrame.smMat = blendRot;
				}
				else
					currentFrame = animation->m_keyFrames.at(bName)[baseNumFrames - 1].rotation;

				if (m_hasActionLayer)
				{
					sm::Matrix actionRot = actionLayer->m_keyFrames.at(bName)[actionNumFrames - 1].rotation.smMat;
					currentFrame.smMat = actionRot.Lerp(currentFrame.smMat, actionRot, m_actionFactor);
				}

				mat.emplace_back(currentFrame);
			}
		}

	}

	void Animator::BindBuffer() {
		if (animationIndex < m_animationList.size()) {
			m_animationList[animationIndex].second->m_finalMatrix.BindSRV<VShader>(0);
		}
	}

	bool Animator::HasAnimation(const std::string& anim) {
		for (auto& i : m_animationList) {
			if (i.first == anim)
				return true;
		}
		return false;
	}

	void Animator::SetAnimationScale(const float& newScale) {
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

	void Animator::SetPaused(const bool& pause) {
		m_pause = pause;
	}

	void Animator::SetLoopAnim(const bool& loop) {
		m_loop = loop;
	}

	void Animator::Reset() {
		m_time = 0.f;
	}

	void Animator::SetActionFactor(float f)
	{
		m_actionFactor = f;
	}

	void Animator::SetRunFactor(float f)
	{
		m_runFactor = f;
	}

	Animator::Animator(const size_t& id)
		:Drawable(id), m_scale(1), animationIndex(0), m_pause(false), m_loop(true), m_time(0.f) {
		SetFrameRate(60);
	}

	void Animator::AddAnimation(Animation& anim, const std::string& name)
	{
		if (HasAnimation(name))
			return;

		std::pair<std::string, Animation*> animation(name, &anim);
		m_animationList.emplace_back(animation);
	}

	void Animator::AddAnimation(const std::string& animName, const std::string& name) {
		if (HasAnimation(name))
			return;

		if (!Resource::AnimationExist(animName)) throw;
		std::pair<std::string, Animation*> animation(name, &Resource::GetAnimation(animName));
		m_animationList.emplace_back(animation);
	}

	void Animator::SetAnimation(const std::string& animName) {
		if (!HasAnimation(animName))
			return;

		for (int i = 0; i < m_animationList.size();i++) {
			if (m_animationList[i].first == animName) {
				animationIndex = i;
				break;
			}
		}
	}

	void Animator::SetActionLayer(const std::string& animName)
	{
		if (!HasAnimation(animName))
			return;

		m_hasActionLayer = true;

		for (int i = 0; i < m_animationList.size(); i++) {
			if (m_animationList[i].first == animName) {
				m_actionIndex = i;
				break;
			}
		}
	}

	void Animator::SetRunLayer(const std::string& animName)
	{
		if (!HasAnimation(animName))
			throw;

		m_hasRunLayer = true;

		for (int i = 0; i < m_animationList.size(); i++) {
			if (m_animationList[i].first == animName) {
				m_runIndex = i;
				break;
			}
		}
	}

	void Animator::SetFrameRate(const int& frameRate) {
		m_frameRate = 1.0 / (double)frameRate;
	}

	void Animator::Draw(Renderer& renderer, const uint32_t& layer) {
		#ifdef _DEBUG
			if (animationIndex < m_animationList.size()) {
				Mat4f m = EntityHandler::GetEntity(m_id).GetTransformation();
				renderer.m_cbTransform.GetData().m_mdlMat = m.Transposed();
				renderer.m_cbTransform.UpdateBuffer();

				RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
				renderer.m_collisionBuffer.BindBuffer<PShader>(0);
				renderer.m_collisionBuffer.GetData().color = { 1.0f, 0.f, 0.f };
				renderer.m_collisionBuffer.UpdateBuffer();
				RenderSystem::BindShader(renderer.m_collisionPS);
			
				renderer.m_cbTransform.BindBuffer<VShader>(0);
				RenderSystem::BindShader(renderer.m_opaqueVS);
				RenderSystem::SetPrimitiveTopology(Topology::LINELIST);
				RenderSystem::BindRenderTargetView(renderer.m_backBuffer);

				m_animationList[animationIndex].second->m_finalMatrix.BindSRV<VShader>(0);
				m_animationList[animationIndex].second->vBuff.BindBuffer();
				m_animationList[animationIndex].second->m_indexBuffer.BindBuffer();
				m_animationList[animationIndex].second->m_indexBuffer.DrawIndexed();
				
				RenderSystem::UnBindShader<VShader>();
				RenderSystem::UnBindShader<PShader>();
				RenderSystem::UnBindShaderResources<VShader>(0, 1);
			}
		#endif
	}

	const Mat4f Animator::GetBoneMat(const uint32_t & index) {
		if(animationIndex < m_animationList.size()) {
			Animation* animation = m_animationList[animationIndex].second;
			return animation->m_finalMatrix.GetData(index).Transposed();
		}

		return Mat4f::identity;
	}

	void Animator::DepthDraw(Renderer& renderer, const uint32_t& layer) {
	}

	bool Animator::FrustumCull(Renderer& renderer) {
		return true;
	}
}