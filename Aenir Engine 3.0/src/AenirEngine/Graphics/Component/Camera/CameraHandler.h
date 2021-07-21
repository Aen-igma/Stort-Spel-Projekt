#pragma once
#include"Camera.h"
#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC CameraHandler {
		private:

		CameraHandler();

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

		static std::unordered_map<uint32_t, Camera*> m_cameras;

		friend class Camera;
		friend class Entity;
	};
}