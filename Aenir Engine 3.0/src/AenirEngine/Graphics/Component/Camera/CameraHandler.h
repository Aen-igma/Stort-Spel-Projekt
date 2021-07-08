#pragma once
#include"Camera.h"
#include<unordered_map>

namespace Aen {

	class CameraHandler {
		private:

		static const bool CameraExist(const uint32_t& id) {
			return GetCameras().count(id) > 0;
		}

		static void CreateCamera(const uint32_t& id) {
			GetCameras().emplace(id, AEN_NEW Camera());
		}

		static void RemoveCamera(const uint32_t& id) {
			if(GetCameras().count(id) > 0) {
				delete GetCameras().at(id);
				GetCameras().at(id) = nullptr;
				GetCameras().erase(id);
			}
		}

		static Camera& GetCamera(const uint32_t& id) {
			if(GetCameras().count(id) > 0)
				return *GetCameras().at(id);

			throw;
		}

		static std::unordered_map<uint32_t, Camera*> GetCameras() {
			static std::unordered_map<uint32_t, Camera*> cameras;
			return cameras;
		}

		friend class Camera;
		friend class Entity;
	};
}