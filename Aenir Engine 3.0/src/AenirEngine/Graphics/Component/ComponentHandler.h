#pragma once
#include"Tranform/Tranform.h"
#include"Camera/Camera.h"
#include"Drawable/Mesh/MeshInstance.h"
#include"Light/Light.h"
#include"RigidBody/StaticBody.h"
#include"RigidBody/DynamicBody.h"
#include"CharacterController/CharacterController.h"
#include "Drawable\UI\UIComponent.h"
#include"Collision\AABBComponent.h"
#include"Collision\OBBComponent.h"

#include<unordered_map>
#include<array>
#include<map>

namespace Aen {

	class AEN_DECLSPEC ComponentHandler {
		private:

		ComponentHandler();

		// ---------- Camera Component ------------ //

		static const bool CameraExist(const size_t& id) {
			return m_cameras.count(id) > 0;
		}

		static void CreateCamera(const size_t& id) {
			m_cameras.emplace(id, AEN_NEW Camera(id));
		}

		static void RemoveCamera(const size_t& id) {
			if(m_cameras.count(id) > 0) {
				delete m_cameras.at(id);
				m_cameras.at(id) = nullptr;
				m_cameras.erase(id);
			}
		}

		static Camera& GetCamera(const size_t& id) {
			if(m_cameras.count(id) > 0)
				return *m_cameras.at(id);

			throw;
		}

		// ----------- Mesh Instance Component ---------- //

		static const bool MeshInstanceExist(const size_t& id) {
			return m_mesheInstances.count(id) > 0;
		}

		static void CreateMeshInstance(const size_t& id, const size_t& layer) {
			m_mesheInstances.emplace(id, AEN_NEW MeshInstance(id));
			m_meshLayer[layer].emplace(id, m_mesheInstances.at(id));
		}

		static void RemoveMeshInstance(const size_t& id) {
			if(m_mesheInstances.count(id) > 0) {
				delete m_mesheInstances.at(id);
				m_mesheInstances.at(id) = nullptr;
				m_mesheInstances.erase(id);
			}
		}

		static MeshInstance& GetMeshInstance(const size_t& id) {
			if(m_mesheInstances.count(id) > 0)
				return *m_mesheInstances.at(id);
		}

		// ----------- UI Component ---------- //

		static const bool UIComponentExist(const size_t& id) {
			return m_UI.count(id) > 0;
		}

		static void CreateUI(const size_t& id, const size_t& layer) {
			m_UI.emplace(id, AEN_NEW UIComponent(id));
			m_meshLayer[layer].emplace(id, m_UI.at(id));
		}

		static void RemoveUI(const size_t& id) {
			if (m_UI.count(id) > 0) {
				delete m_UI.at(id);
				m_UI.at(id) = nullptr;
				m_UI.erase(id);
			}
		}

		static UIComponent& GetUI(const size_t& id) {
			if (m_UI.count(id) > 0)
				return *m_UI.at(id);
		}

		// ------------ Transform Component ------------- //

		static const bool TranslationExist(const size_t& id) {
			return m_translations.count(id) > 0;
		}

		static const bool RotationExist(const size_t& id) {
			return m_rotations.count(id) > 0;
		}

		static const bool ScaleExist(const size_t& id) {
			return m_scales.count(id) > 0;
		}

		static void CreateTranslation(const size_t& id) {
			m_translations.emplace(id, AEN_NEW Translation(id));
		}

		static void CreateRotation(const size_t& id) {
			m_rotations.emplace(id, AEN_NEW Rotation(id));
		}

		static void CreateScale(const size_t& id) {
			m_scales.emplace(id, AEN_NEW Scale(id));
		}

		static void RemoveTranform(const size_t& id) {
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

		static Translation& GetTranslation(const size_t& id) {
			if(m_translations.count(id) > 0)
				return *m_translations.at(id);
			
			throw;
		}

		static Rotation& GetRotation(const size_t& id) {
			if(m_rotations.count(id) > 0)
				return *m_rotations.at(id);

			throw;
		}

		static Scale& GetScale(const size_t& id) {
			if(m_scales.count(id) > 0)
				return *m_scales.at(id);

			throw;
		}

		// ----------------- Spot Light Component ------------------ //

		static const bool SpotLightExist(const size_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<SpotLight*>(i->second))
						return true;

			return false;
		}

		static void CreateSpotLight(const size_t& id) {
			m_lights.emplace(id, AEN_NEW SpotLight(id));
		}

		static void RemoveSpotLight(const size_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<SpotLight*>(i->second)) {
						delete i->second;
						i->second = nullptr;
						m_lights.erase(i);
						break;
					}
		}

		static SpotLight& GetSpotLight(const size_t& id) {
			SpotLight* pSLight = nullptr;
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<SpotLight*>(i->second))
						pSLight = reinterpret_cast<SpotLight*>(i->second);

			if(!pSLight) throw;
			return *pSLight;
		}

		// ----------------- Point Light Component ------------------ //

		static const bool PointLightExist(const size_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<PointLight*>(i->second))
						return true;

			return false;
		}

		static void CreatePointLight(const size_t& id) {
			m_lights.emplace(id, AEN_NEW PointLight(id));
		}

		static void RemovePointLight(const size_t& id) {
			if(m_lights.count(id) > 0) 
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<PointLight*>(i->second)) {
						delete i->second;
						i->second = nullptr;
						m_lights.erase(i);
						break;
					}
		}

		static PointLight& GetPointLight(const size_t& id) {
			PointLight* pPLight = nullptr;
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<PointLight*>(i->second))
						pPLight = reinterpret_cast<PointLight*>(i->second);

			if(!pPLight) throw;
			return *pPLight;
		}

		// ----------------- Directional Light Component ------------------ //

		static const bool DirectionalLightExist(const size_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<DirectionalLight*>(i->second))
						return true;

			return false;
		}

		static void CreateDirectionalLight(const size_t& id) {
			m_lights.emplace(id, AEN_NEW DirectionalLight(id));
		}

		static void RemoveDirectionalLight(const size_t& id) {
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<DirectionalLight*>(i->second)) {
						delete i->second;
						i->second = nullptr;
						m_lights.erase(i);
						break;
					}
		}

		static DirectionalLight& GetDirectionalLight(const size_t& id) {
			DirectionalLight* pDLight = nullptr;
			if(m_lights.count(id) > 0)
				for(auto i = m_lights.lower_bound(id); i != m_lights.upper_bound(id); i++)
					if(dynamic_cast<DirectionalLight*>(i->second)) 
						pDLight = reinterpret_cast<DirectionalLight*>(i->second);

			if(!pDLight) throw;
			return *pDLight;
		}

		// ----------- StaticBody Component ----------- //

		static const bool StaticBodyExist(const size_t& id) {
			return m_staticBodies.count(id) > 0;
		}

		static void CreateStaticBody(const size_t& id) {
			m_staticBodies.emplace(id, AEN_NEW StaticBody(id));
		}

		static void RemoveStaticBody(const size_t& id) {
			if (m_staticBodies.count(id) > 0) {
				delete m_staticBodies.at(id);
				m_staticBodies.at(id) = nullptr;
				m_staticBodies.erase(id);
			}
		}

		static StaticBody& GetStaticBody(const size_t& id) {
			if (m_staticBodies.count(id) > 0)
				return *m_staticBodies.at(id);
			throw;
		}

		// ----------- DynamicBody Component ----------- //

		static const bool DynamicBodyExist(const size_t& id) {
			return m_dynamicBodies.count(id) > 0;
		}

		static void CreateDynamicBody(const size_t& id) {
			m_dynamicBodies.emplace(id, AEN_NEW DynamicBody(id));
		}

		static void RemoveDynamicBody(const size_t& id) {
			if (m_dynamicBodies.count(id) > 0) {
				delete m_dynamicBodies.at(id);
				m_dynamicBodies.at(id) = nullptr;
				m_dynamicBodies.erase(id);
			}
		}

		static DynamicBody& GetDynamicBody(const size_t& id) {
			if (m_dynamicBodies.count(id) > 0)
				return *m_dynamicBodies.at(id);
			throw;
		}

		// ------ CharacterController Component ------- //

		static const bool CharacterControllerExist(const size_t& id) {
			return m_characterControllers.count(id) > 0;
		}

		static void CreateCharacterController(const size_t& id) {
			m_characterControllers.emplace(id, AEN_NEW CharacterController(id));
		}

		static void RemoveCharacterController(const size_t& id) {
			if (m_characterControllers.count(id) > 0) {
				delete m_characterControllers.at(id);
				m_characterControllers.at(id) = nullptr;
				m_characterControllers.erase(id);
			}
		}

		static CharacterController& GetCharacterController(const size_t& id) {
			if (m_characterControllers.count(id) > 0)
				return *m_characterControllers.at(id);
			throw;
		}

		// -------------------------------------------- //

		// ----------- Axis Aligned Bounding Box ------------ //

		static const bool AABBExist(const size_t& id) {
			return m_AABBs.count(id) > 0;
		}

		static void CreateAABB(const size_t& id, const size_t& layer) {
			m_AABBs.emplace(id, AEN_NEW AABoundBox(id));
			m_meshLayer[6].emplace(id, m_AABBs.at(id));
		}

		static void RemoveAABB(const size_t& id) {
			if (m_AABBs.count(id) > 0) {
				delete m_AABBs.at(id);
				m_AABBs.at(id) = nullptr;
				m_AABBs.erase(id);
			}
		}

		static AABoundBox& GetAABB(const size_t& id) {
			if (m_AABBs.count(id) > 0)
				return *m_AABBs.at(id);
			throw;
		}

		// -------------------------------------------- //

		// ----------- Axis Aligned Bounding Box ------------ //

		static const bool OBBExist(const size_t& id) {
			return m_OBBs.count(id) > 0;
		}

		static void CreateOBB(const size_t& id, const size_t& layer) {
			m_OBBs.emplace(id, AEN_NEW OBBox(id));
			m_meshLayer[6].emplace(id, m_OBBs.at(id));
		}

		static void RemoveOBB(const size_t& id) {
			if (m_OBBs.count(id) > 0) {
				delete m_OBBs.at(id);
				m_OBBs.at(id) = nullptr;
				m_OBBs.erase(id);
			}
		}

		static OBBox& GetOBB(const size_t& id) {
			if (m_OBBs.count(id) > 0)
				return *m_OBBs.at(id);
			throw;
		}

		// -------------------------------------------- //

		// ----------- Mesh Instance Layer ------------ //

		static void SetRenderLayer(MeshInstance& mesh, const size_t id, const size_t& layer) {
			m_meshLayer[layer].emplace(id, &mesh);
		}

		static std::unordered_map<size_t, Drawable*>& GetLayer(const size_t& layer) {
			return m_meshLayer[layer];
		}

		static void RemoveMeshFromLayer(const size_t id, const size_t& layer) {
			m_meshLayer[layer].erase(id);
		}
		// ------------------------------------------ //

		static std::unordered_map<size_t, Camera*> m_cameras;
		static std::unordered_map<size_t, MeshInstance*> m_mesheInstances;
		static std::unordered_map<size_t, Translation*> m_translations;
		static std::unordered_map<size_t, Rotation*> m_rotations;
		static std::unordered_map<size_t, Scale*> m_scales;
		static std::unordered_map<size_t, StaticBody*> m_staticBodies;
		static std::unordered_map<size_t, DynamicBody*> m_dynamicBodies;
		static std::unordered_map<size_t, CharacterController*> m_characterControllers;
		static std::unordered_map<size_t, AABoundBox*> m_AABBs;
		static std::unordered_map<size_t, OBBox*> m_OBBs;
		static std::unordered_map<size_t, UIComponent*> m_UI;
		static std::multimap<size_t, Light*> m_lights;
		
		static std::array<std::unordered_map<size_t, Drawable*>, 7> m_meshLayer;

		friend class Entity;
		friend class MeshInstance;
		friend class Renderer;
		friend class ImGuiHandler;
		friend class AABoundBox;
		friend class OBBox;
		friend class Camera;
		friend class LevelExporter;
		friend class ImGuiImporter;
		friend class StaticBody;
		friend class DynamicBody;
	};

}