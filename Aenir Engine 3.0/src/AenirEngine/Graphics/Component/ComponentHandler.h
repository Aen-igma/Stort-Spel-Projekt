#pragma once
#include"Tranform/Tranform.h"
#include"Camera/Camera.h"
#include"Drawable/Mesh/MeshInstance.h"
#include"Light/Light.h"
#include"ThirdParty/PhysX/RigidBody.h"
#include"Animation/Animator.h"

#include<unordered_map>
#include<array>
#include<map>

namespace Aen {

	class AEN_DECLSPEC ComponentHandler {
		private:

		ComponentHandler();

		// ---------- Camera Component ------------ //

		static const bool CameraExist(const uint32_t& id) {
			return m_cameras.count(id) > 0;
		}

		static void CreateCamera(const uint32_t& id) {
			m_cameras.emplace(id, AEN_NEW Camera());
		}

		static void RemoveCamera(const uint32_t& id) {
			if(m_cameras.count(id) > 0) {
				delete m_cameras.at(id);
				m_cameras.at(id) = nullptr;
				m_cameras.erase(id);
			}
		}

		static Camera& GetCamera(const uint32_t& id) {
			if(m_cameras.count(id) > 0)
				return *m_cameras.at(id);

			throw;
		}

		// ----------- Mesh Instance Component ---------- //

		static const bool MeshInstanceExist(const uint32_t& id) {
			return m_mesheInstances.count(id) > 0;
		}

		static void CreateMeshInstance(const uint32_t& id, const uint32_t& layer) {
			m_mesheInstances.emplace(id, AEN_NEW MeshInstance());
			m_meshLayer[layer].emplace(id, m_mesheInstances.at(id));
		}

		static void RemoveMeshInstance(const uint32_t& id) {
			if(m_mesheInstances.count(id) > 0) {
				delete m_mesheInstances.at(id);
				m_mesheInstances.at(id) = nullptr;
				m_mesheInstances.erase(id);
			}
		}

		static MeshInstance& GetMeshInstance(const uint32_t& id) {
			if(m_mesheInstances.count(id) > 0)
				return *m_mesheInstances.at(id);
		}

		// ------------ Transform Component ------------- //

		static const bool TranslationExist(const uint32_t& id) {
			return m_translations.count(id) > 0;
		}

		static const bool RotationExist(const uint32_t& id) {
			return m_rotations.count(id) > 0;
		}

		static const bool ScaleExist(const uint32_t& id) {
			return m_scales.count(id) > 0;
		}

		static void CreateTranslation(const uint32_t& id) {
			m_translations.emplace(id, AEN_NEW Translation());
		}

		static void CreateRotation(const uint32_t& id) {
			m_rotations.emplace(id, AEN_NEW Rotation());
		}

		static void CreateScale(const uint32_t& id) {
			m_scales.emplace(id, AEN_NEW Scale());
		}

		static void RemoveTranform(const uint32_t& id) {
			if(m_translations.count(id) > 0) {
				delete m_translations.at(id);
				m_translations.at(id) = nullptr;
				m_translations.erase(id);
			}

			if(m_rotations.count(id) > 0) {
				delete m_rotations.at(id);
				m_rotations.at(id) = nullptr;
				m_rotations.erase(id);
			}

			if(m_scales.count(id) > 0) {
				delete m_scales.at(id);
				m_scales.at(id) = nullptr;
				m_scales.erase(id);
			}
		}

		static Translation& GetTranslation(const uint32_t& id) {
			if(m_translations.count(id) > 0)
				return *m_translations.at(id);

			throw;
		}

		static Rotation& GetRotation(const uint32_t& id) {
			if(m_rotations.count(id) > 0)
				return *m_rotations.at(id);

			throw;
		}

		static Scale& GetScale(const uint32_t& id) {
			if(m_scales.count(id) > 0)
				return *m_scales.at(id);

			throw;
		}

		// ----------------- Spot Light Component ------------------ //

		static const bool SpotLightExist(const uint32_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<SpotLight*>(i->second))
						return true;

			return false;
		}

		static void CreateSpotLight(const uint32_t& id) {
			m_lights.emplace(id, AEN_NEW SpotLight());
		}

		static void RemoveSpotLight(const uint32_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<SpotLight*>(i->second)) {
						delete i->second;
						i->second = nullptr;
						m_lights.erase(i);
						break;
					}
		}

		static SpotLight& GetSpotLight(const uint32_t& id) {
			SpotLight* pSLight = nullptr;
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<SpotLight*>(i->second))
						pSLight = reinterpret_cast<SpotLight*>(i->second);

			if(!pSLight) throw;
			return *pSLight;
		}

		// ----------------- Point Light Component ------------------ //

		static const bool PointLightExist(const uint32_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<PointLight*>(i->second))
						return true;

			return false;
		}

		static void CreatePointLight(const uint32_t& id) {
			m_lights.emplace(id, AEN_NEW PointLight());
		}

		static void RemovePointLight(const uint32_t& id) {
			if(m_lights.count(id) > 0) 
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<PointLight*>(i->second)) {
						delete i->second;
						i->second = nullptr;
						m_lights.erase(i);
						break;
					}
		}

		static PointLight& GetPointLight(const uint32_t& id) {
			PointLight* pPLight = nullptr;
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<PointLight*>(i->second))
						pPLight = reinterpret_cast<PointLight*>(i->second);

			if(!pPLight) throw;
			return *pPLight;
		}

		// ----------------- Directional Light Component ------------------ //

		static const bool DirectionalLightExist(const uint32_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<DirectionalLight*>(i->second))
						return true;

			return false;
		}

		static void CreateDirectionalLight(const uint32_t& id) {
			m_lights.emplace(id, AEN_NEW DirectionalLight());
		}

		static void RemoveDirectionalLight(const uint32_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<DirectionalLight*>(i->second)) {
						delete i->second;
						i->second = nullptr;
						m_lights.erase(i);
						break;
					}
		}

		static DirectionalLight& GetDirectionalLight(const uint32_t& id) {
			DirectionalLight* pDLight = nullptr;
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<DirectionalLight*>(i->second)) 
						pDLight = reinterpret_cast<DirectionalLight*>(i->second);

			if(!pDLight) throw;
			return *pDLight;
		}

		// ----------- Rigid Body Component ----------- //

		static const bool RigidExist(const uint32_t& id) {
			return m_rigids.count(id) > 0;
		}

		static void CreateRigid(const uint32_t& id) {
			m_rigids.emplace(id, AEN_NEW RigidBody());
		}

		static void RemoveRigid(const uint32_t& id) {
			if (m_rigids.count(id) > 0) {
				delete m_rigids.at(id);
				m_rigids.at(id) = nullptr;
				m_rigids.erase(id);
			}
		}

		static RigidBody& GetRigid(const uint32_t& id) {
			if (m_rigids.count(id) > 0)
				return *m_rigids.at(id);
			throw;
		}

		// -------------------------------------------- //

		// ----------- Mesh Instance Layer ---------- //

		static void SetRenderLayer(MeshInstance& mesh, const uint32_t id, const uint32_t& layer) {
			m_meshLayer[layer].emplace(id, &mesh);
		}

		static std::unordered_map<uint32_t, Drawable*>& GetLayer(const uint32_t& layer) {
			return m_meshLayer[layer];
		}

		static void RemoveMeshFromLayer(const uint32_t id, const uint32_t& layer) {
			m_meshLayer[layer].erase(id);
		}
		// ------------------------------------------ //

		// ------------------ Animator --------------------- //
		static const bool AnimatorExists(const uint32_t& id) {
			return m_animators.count(id) > 0;
		}

		static void CreateAnimator(const uint32_t& id) {
			m_animators.emplace(id, AEN_NEW Animator());
			m_meshLayer[5].emplace(id, m_animators.at(id));
		}

		static void RemoveAnimators(const uint32_t& id) {
			if (m_animators.count(id) > 0) {
				delete m_animators.at(id);
				m_animators.at(id) = nullptr;
				m_animators.erase(id);
			}
		}

		static Animator& GetAnimator(const uint32_t& id) {
			if (m_animators.count(id) > 0)
				return *m_animators.at(id);
		}
		// --------------------------------------------------//


		static std::unordered_map<uint32_t, Camera*> m_cameras;
		static std::unordered_map<uint32_t, MeshInstance*> m_mesheInstances;
		static std::unordered_map<uint32_t, Translation*> m_translations;
		static std::unordered_map<uint32_t, Rotation*> m_rotations;
		static std::unordered_map<uint32_t, Scale*> m_scales;
		static std::unordered_map<uint32_t, RigidBody*> m_rigids;
		static std::unordered_map<uint32_t, Animator*> m_animators;
		static std::multimap<uint32_t, Light*> m_lights;
		
		
		static std::array<std::unordered_map<uint32_t, Drawable*>, 7> m_meshLayer;

		friend class Entity;
		friend class MeshInstance;
		friend class Renderer;
		friend class ImGuiHandler;
	};

}