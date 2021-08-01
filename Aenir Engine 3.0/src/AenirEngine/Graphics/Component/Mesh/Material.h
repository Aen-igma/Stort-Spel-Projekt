#pragma once
#include"..\..\Graphics.h"
#include"Texture.h"

namespace Aen {

	struct ShaderModel {
		ShaderModel() = default;

		using PLayout = std::pair<UINT, DBLayout>;
		using PSampler = std::pair<UINT, Sampler>;

		ILayout m_iLayoutPass1;
		VShader m_VShaderPass1;
		PShader m_PShaderPass1;
		PSampler m_samplerDataPass1;

		ILayout m_iLayoutPass2;
		VShader m_VShaderPass2;
		PShader m_PShaderPass2;
		PSampler m_samplerDataPass2;

		PLayout m_dbLayout;
		GBuffer m_gBuffer;

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

		void SetDiffuseMap(const std::string& textureName);
		void SetNormalMap(const std::string& textureName);
		void SetEmissionMap(const std::string& textureName);
		void SetOpacityMap(const std::string& textureName);
		
		void LoadeAndSetDiffuseMap(const std::string& dir);
		void LoadeAndSetNormalMap(const std::string& dir);
		void LoadeAndSetEmissionMap(const std::string& dir);
		void LoadeAndSetOpacityMap(const std::string& dir);

		ElRef operator[] (const std::string& name);

		private:
		~Material() = default;

		ShaderModel* m_pShaderModel;
		DBuffer m_dBuffer;
		Texture* m_textures[4];

		friend class ResourceHandler;
		friend class Renderer;
	};
}