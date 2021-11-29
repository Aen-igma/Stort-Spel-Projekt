#include"PCH.h"
#include"Animator.h"
#include"Core/Renderer.h"

namespace Aen {
	void Animator::Update()
	{
		if (animation && pause) {
			m_end = ResClock::now();
			while (std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start) > frameRate) {
				m_start = ResClock::now();
				std::vector<Mat4f> anim;
				GetAnimation(anim);

				std::vector<Mat4f> localTran(animation->m_boneArray.size());
				//std::vector<Mat4f> parentTran(animation->m_boneArray.size());
				std::vector<Mat4f> modelTran(animation->m_boneArray.size());

				for (int i = 0; i < animation->m_boneArray.size(); i++) {
					localTran[i] = anim[i].Transposed() * animation->m_boneArray[i].localMatrix;
				}

				modelTran[0] = localTran[0];

				for (int i = 1; i < animation->m_boneArray.size(); i++) {
					int parent = animation->m_boneArray[i].parentID;
					modelTran[i] = modelTran[parent] * localTran[i];
				}

				for (int i = 0; i < animation->m_boneArray.size(); i++) {
					animation->m_finalMatrix.GetData(i) = modelTran[i] * animation->m_boneArray[i].offsetMatrix;
				}

				animation->m_finalMatrix.UpdateBuffer();
			}
		}
	}

	void Animator::GetAnimation(std::vector<Mat4f>& mat)
	{
		int sizeBA = animation->m_boneArray.size();
		int numFrames = animation->m_timeStamp.size();
		m_frameTime = ResClock::now();
		DurationLD duration = std::chrono::duration_cast<std::chrono::nanoseconds>(m_frameTime - m_currentTime);

		if (duration.count() > animation->m_Duration * m_scale) {
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
				Mat4f currentFrame = animation->m_keyFrames.at(bName)[m_currentFrame].scale * animation->m_keyFrames.at(bName)[m_currentFrame].rotation * animation->m_keyFrames.at(bName)[m_currentFrame].position;
				Mat4f nextFrame = animation->m_keyFrames.at(bName)[m_currentFrame + 1].scale * animation->m_keyFrames.at(bName)[m_currentFrame + 1].rotation * animation->m_keyFrames.at(bName)[m_currentFrame + 1].position;

				mat.emplace_back(Lerp(currentFrame, nextFrame, t));
			}
		}
		else {
			for (int i = 0; i < sizeBA; i++) {
				std::string bName = animation->m_boneArray[i].boneName;
				Mat4f currentFrame = animation->m_keyFrames.at(bName)[m_currentFrame].scale * animation->m_keyFrames.at(bName)[m_currentFrame].rotation * animation->m_keyFrames.at(bName)[m_currentFrame].position;

				mat.emplace_back(currentFrame);
			}
		}
	}

	void Animator::BindBuffer()
	{
		if (animation) {
			animation->m_finalMatrix.BindSRV<VShader>(0);
		}
	}

	void Animator::SetAnimationScale(const float& newScale)
	{
		m_scale = newScale;
	}

	void Animator::Pause()
	{
		pause = false;
	}

	void Animator::Run()
	{
		pause = true;
	}

	Animator::Animator(const size_t& id)
		:Drawable(id), animation(nullptr), m_scale(1)
	{
		m_start = m_end = ResClock::now();
		SetFrameRate(60);
	}

	void Animator::SetAnimation(Animation& anim)
	{
		this->animation = &anim;
	}

	void Animator::SetAnimation(const std::string& animName)
	{
		if (!Resource::AnimationExist(animName)) throw;
		this->animation = &Resource::GetAnimation(animName);
	}

	void Animator::SetFrameRate(const int& frameRate)
	{
		int ft = (int)(((double)1 / (double)frameRate) * (double)pow(10, 9));
		this->frameRate = std::chrono::nanoseconds{ ft };
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

	bool Animator::FrustumCull(Renderer& renderer)
	{
		return true;
	}
}