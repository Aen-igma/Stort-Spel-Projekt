#pragma once
#include"../../Graphics.h"
#include<unordered_map>

namespace Aen {

	class Camera;

	class CameraHandler {
		public:

		static Camera& GetCamera(const uint32_t& id) {
			if(GetCameras().count(id) > 0)
				return *GetCameras().at(id);

			throw;
		}

		private:

		static std::unordered_map<uint32_t, Camera*> GetCameras() {
			static std::unordered_map<uint32_t, Camera*> cameras;
			return cameras;
		}

		friend class Camera;
	};
}