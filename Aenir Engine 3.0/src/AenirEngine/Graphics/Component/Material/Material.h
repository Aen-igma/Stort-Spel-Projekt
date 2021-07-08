#pragma once
#include"..\..\Graphics.h"
#include"Texture.h"

namespace Aen {

	struct BLayout {
		DBLayout m_dbLayout;
		UINT m_slot;
	};

	struct SamplerData {
		Sampler m_sampler;
		UINT m_slot;
	};

	struct Material {
		Material() = default;

		PShader m_pShader;
		VShader m_vShader;
		std::vector<BLayout> m_bLayouts;
		std::vector<SamplerData> m_samplerDatas;

		private:
		~Material() = default;

		friend class MaterialHandler;
	};

	class MaterialInstance {
		public:
		MaterialInstance();
		MaterialInstance(Material& material);

		void Create(Material& material);
		void SetDiffuseMap(Texture& texture);
		void SetNormalMap(Texture& texture);
		void SetEmissionMap(Texture& texture);

		private:
		~MaterialInstance();

		std::vector<DBuffer> m_dBuffers;
		Material* m_material;
		Texture* m_textures[3];

		friend class MaterialIHandler;
	};
}