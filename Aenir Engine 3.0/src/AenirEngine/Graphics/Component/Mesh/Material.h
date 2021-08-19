#pragma once
#include"..\..\Graphics.h"
#include"Texture.h"

namespace Aen {

	class ShaderModel {
		public:
		using PSampler = std::pair<UINT, Sampler>;

		ShaderModel() = delete;
		ShaderModel(Window& window);

		const bool SetFirstPass(const std::wstring& VSDir, const std::wstring& PSDir, const SamplerType& samplerType);
		const bool SetSecondPass(const std::wstring& VSDir, const std::wstring& PSDir, const SamplerType& samplerType);

		DBLayout m_dbLayout;
		std::string m_bufferName;

		private:

		VShader m_VShaderPass1;
		PShader m_PShaderPass1;
		ILayout m_iLayoutPass1;
		Reflection m_VSReflectPass1;
		Reflection m_PSReflectPass1;
		PSampler m_samplerDataPass1;
		uint32_t m_slotsPass1[14];

		VShader m_VShaderPass2;
		PShader m_PShaderPass2;
		ILayout m_iLayoutPass2;
		Reflection m_VSReflectPass2;
		Reflection m_PSReflectPass2;
		PSampler m_samplerDataPass2;
		uint32_t m_slotsPass2[14];

		GBuffer m_gBuffer;
		Window& m_window;

		~ShaderModel() = default;

		friend class Resource;
		friend class Renderer;
	};

	class AEN_DECLSPEC Material {
		public:
		Material(const bool& useDefaultShader = false);
		Material(ShaderModel& shaderModel);
		Material(const std::string& shaderName);

		void Create(ShaderModel& shaderModel);
		void Create(const std::string& shaderName);
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

		friend class Resource;
		friend class Renderer;
	};
}