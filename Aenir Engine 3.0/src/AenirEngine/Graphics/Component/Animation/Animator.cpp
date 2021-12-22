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
		int blendIndex = animation->m_hasActionLayer ? animation->GetBlendIndex() : 0;
		Animation* runLayer = animation->mp_runLayer;
		Animation* actionLayer = animation->m_hasActionLayer ? animation->mp_actionLayer[blendIndex] : nullptr;
		const bool& doRunBl = runLayer != nullptr;
		const bool& doActionBl = actionLayer != nullptr;

		float blendFactor = animation->GetRunningFactor();
		float actionFactor = animation->GetActionFactor();

		uint16_t sizeBA = animation->m_boneArray.size();

		uint16_t baseNumFrames = animation->m_timeStamp.size();
		uint16_t layerNumFrames = doRunBl ? runLayer->m_timeStamp.size() : 0.f;
		uint16_t actionNumFrames = doActionBl ? actionLayer->m_timeStamp.size() : 0.f;

		//const uint16_t baseOffset = doRunBl ? baseNumFrames / layerNumFrames : 1.f;
		//const uint16_t layerOffset = doRunBl ? layerNumFrames / baseNumFrames : 1.f;

		
		float duration = doRunBl ? duration = runLayer->m_duration * m_scale : duration = animation->m_duration * m_scale;



		if (m_time < duration) 
		{
			uint16_t l = 0u;
			uint16_t r = doRunBl ? layerNumFrames : baseNumFrames;
			uint16_t baseR = layerNumFrames;
			uint16_t mid = 0u;
			uint16_t layerMid = 0u;

			uint16_t fFrame = 0u;
			uint16_t sFrame = 0u;

			while (true) {
				mid = (l + r) / 2u;
				float ft = doRunBl ? runLayer->m_timeStamp[mid] * duration : animation->m_timeStamp[mid] * duration;
				float st = doRunBl ? runLayer->m_timeStamp[Clamp(mid + 1u, 0u, layerNumFrames - 1u)] * duration : animation->m_timeStamp[Clamp(mid + 1u, 0u, baseNumFrames - 1u)] * duration;
				if ((m_time >= ft && m_time <= st) || ft == st) {
					fFrame = mid;
					sFrame = doRunBl ? (mid + 1u) % baseNumFrames : (mid + 1u) % baseNumFrames;
					break;
				}

				if (m_time < ft)
					r = mid;

				if (m_time > ft)
					l = mid;
			}
			//while (doBl)
			//{
			//	mid = (l + r) / 2u;
			//	layerMid = doBl ? (l + baseR) / 2u : 0u;
			//	float ft = doBl ? aniLayer->m_timeStamp[mid] * duration : animation->m_timeStamp[mid] * duration;
			//	float st = doBl ? aniLayer->m_timeStamp[Clamp(mid + 1u, 0u, layerNumFrames - 1u)] * duration : animation->m_timeStamp[Clamp(mid + 1u, 0u, baseNumFrames - 1u)] * duration;
			//	if ((m_time >= ft && m_time <= st) || ft == st) {
			//		fFrame = mid;
			//		sFrame = doBl ? (mid + 1u) % baseNumFrames : (mid + 1u) % baseNumFrames;
			//		break;
			//	}

			//	if (m_baseTime < ft)
			//		r = mid;

			//	if (m_baseTime > ft)
			//		l = mid;
			//}

			float f = animation->IsBlendAnimation() ? m_time - runLayer->m_timeStamp[fFrame] * duration : m_time - animation->m_timeStamp[fFrame] * duration;
			float h = animation->IsBlendAnimation() ? runLayer->m_timeStamp[sFrame] * duration - runLayer->m_timeStamp[fFrame] * duration : animation->m_timeStamp[sFrame] * duration - animation->m_timeStamp[fFrame] * duration;
			float t = f / h;

			uint16_t baseFrame = fFrame % baseNumFrames;
			uint16_t layerFrame = doRunBl ? fFrame % layerNumFrames : 0.f;
			uint16_t actionFrame = doActionBl ? fFrame % actionNumFrames : 0.f;

			for (int i = 0; i < sizeBA; i++) {
				std::string bName = animation->m_boneArray[i].boneName;
				Mat4f currentFrame;
				Mat4f nextFrame;
				if (animation->IsBlendAnimation())
				{

					sm::Matrix currentRot0 = animation->m_keyFrames.at(bName)[baseFrame].rotation.smMat;
					sm::Matrix currentRot1 = runLayer->m_keyFrames.at(bName)[layerFrame].rotation.smMat;
					sm::Matrix currentBlendRot = currentBlendRot.Lerp(currentRot0, currentRot1, blendFactor);
					currentFrame.smMat = currentBlendRot;

					sm::Matrix nextRot0 = animation->m_keyFrames.at(bName)[sFrame % baseNumFrames].rotation.smMat;
					sm::Matrix nextRot1 = runLayer->m_keyFrames.at(bName)[sFrame % layerNumFrames].rotation.smMat;
					sm::Matrix nextBlendRot = currentBlendRot.Lerp(nextRot0, nextRot1, blendFactor);

					nextFrame.smMat = nextBlendRot;
				}
				else
				{
					currentFrame = animation->m_keyFrames.at(bName)[fFrame % baseNumFrames].rotation;
					nextFrame = animation->m_keyFrames.at(bName)[sFrame % baseNumFrames].rotation;
				}

				if (doActionBl && actionLayer->m_doBlendBone[i])
				{
					sm::Matrix current = actionLayer->m_keyFrames.at(bName)[actionFrame].rotation.smMat;
					sm::Matrix next = actionLayer->m_keyFrames.at(bName)[sFrame % actionNumFrames].rotation.smMat;

					currentFrame.smMat = current.Lerp(currentFrame.smMat, current, actionFactor);
					nextFrame.smMat = next.Lerp(nextFrame.smMat, next, actionFactor);
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
				if (animation->IsBlendAnimation())
				{
					sm::Matrix rot0 = animation->m_keyFrames.at(bName)[baseNumFrames - 1].rotation.smMat;
					sm::Matrix rot1 = runLayer->m_keyFrames.at(bName)[layerNumFrames - 1].rotation.smMat;
					sm::Matrix blendRot = blendRot.Lerp(rot0, rot1, blendFactor);
					currentFrame.smMat = blendRot;
				}
				else
					currentFrame = animation->m_keyFrames.at(bName)[baseNumFrames - 1].rotation;

				if (doActionBl && actionLayer->m_doBlendBone[i])
				{
					sm::Matrix actionRot = actionLayer->m_keyFrames.at(bName)[actionNumFrames - 1].rotation.smMat;
					currentFrame.smMat = actionRot.Lerp(currentFrame.smMat, actionRot, actionFactor);
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