#pragma once
#include"Texture.h"

namespace Aen {

	class Material {
		public:
		Material();

		void SetAlbedoMap(Texture* texture);
		void SetEmissionMap(Texture* texture);

		private:
		//MShader* m_mShader;
		Texture* m_textures[2];
	};
}