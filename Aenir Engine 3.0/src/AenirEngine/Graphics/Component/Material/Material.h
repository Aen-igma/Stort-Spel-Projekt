#pragma once
#include"MaterialIHandler.h"
#include"MaterialHandler.h"
#include"Texture.h"

namespace Aen {

	struct Material {
		Material() = delete;
		Material(const std::string& name);

		PShader m_pShader;
		std::vector<DBLayout> m_dbLayouts;
		std::vector<Sampler> m_samplers;
		private:
		~Material();
		std::string m_name;

		friend class MaterialHandler;
	};

	class MaterialInstance {
		public:
		~MaterialInstance();
		MaterialInstance() = delete;
		MaterialInstance(const uint32_t& id, Material& material);

		void SetDiffuseMap(Texture& texture);
		void SetNormalMap(Texture& texture);
		void SetEmissionMap(Texture& texture);

		private:
		std::vector<DBuffer> m_dBuffers;
		Material* m_material;
		Texture* m_textures[3];
		uint32_t m_id;
	};
}