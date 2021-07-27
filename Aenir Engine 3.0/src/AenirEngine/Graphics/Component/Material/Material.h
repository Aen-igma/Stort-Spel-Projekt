#pragma once
#include"..\..\Graphics.h"
#include"Texture.h"

namespace Aen {

	struct ShaderModel {
		ShaderModel() = default;

		using PLayout = std::pair<UINT, DBLayout>;
		using PSampler = std::pair<UINT, Sampler>;

		PShader m_PShader;
		VShader m_VShader;
		ILayout m_iLayout;
		std::vector<PLayout> m_dbLayouts;
		std::vector<PSampler> m_samplerDatas;

		private:
		~ShaderModel() = default;

		friend class ResourceHandler;
		friend class Renderer;
	};

	class AEN_DECLSPEC Material {
		public:
		Material(const bool& useDefaultShader = false);
		Material(ShaderModel& shaderModel);

		void Create(ShaderModel& shaderModel);
		void CreateDefault();

		void SetDiffuseMap(Texture& texture);
		void SetNormalMap(Texture& texture);
		void SetEmissionMap(Texture& texture);
		void SetOpacityMap(Texture& texture);

		DBuffer& GetBuffer(const uint32_t& index);

		private:
		~Material();

		ShaderModel* m_pShader;
		std::vector<DBuffer*> m_dBuffers;
		Texture* m_textures[4];

		friend class ResourceHandler;
		friend class Renderer;
	};

	class AEN_DECLSPEC MaterialInstance {
		public:
		MaterialInstance() = default;
		MaterialInstance(Material*& material);

		void SetMaterial(Material*& material);

		private:
		~MaterialInstance();

		Material* m_pMaterial;

		friend class ComponentHandler;
		friend class Renderer;
	};
}