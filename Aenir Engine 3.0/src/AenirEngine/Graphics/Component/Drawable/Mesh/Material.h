#pragma once
#include"..\..\..\Graphics.h"
#include"Texture.h"

namespace Aen {

	struct ShaderModelDesc {
		std::wstring PSDir;
		std::wstring CSDir;
		SamplerType samplerType = SamplerType::NONE;
		std::string bufferName;
	};

	class ShaderModel {
		public:
		using PSampler = std::pair<UINT, Sampler>;

		ShaderModel() = delete;
		ShaderModel(Window& window);

		const bool Create(const ShaderModelDesc& desc);

		DBLayout m_dbLayout;

		private:
		std::string m_bufferName;
		
		PShader m_PShader;
		CShader m_CShader;
		GBuffer m_gBuffer;
		Reflection m_PSReflect;
		Reflection m_CSReflect;
		PSampler m_samplerData;
		uint32_t m_slots[19];

		Window& m_window;

		~ShaderModel() = default;

		friend class Resource;
		friend class Renderer;
		friend class MeshInstance;
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
		friend class MeshInstance;
		friend class GlobalSettings;
	};
}