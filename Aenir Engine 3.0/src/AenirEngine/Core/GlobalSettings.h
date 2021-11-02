#pragma once
#include"Renderer.h"
#include "../ImGuiHandler.h"
#include<string>
//#include"../LevelGeneration/LevelGenerator.h"

namespace Aen {

	class AEN_DECLSPEC GlobalSettings {
		public:
		
		static void SetDefaultShader(const std::string& name) {
			m_pDefaultShader = &Resource::GetShader(name);
		}

		static ShaderModel* GetDefaultShader() {
			return m_pDefaultShader;
		}

		static void SetMainCamera(Entity& camera) {
			m_pMainCamera = &camera;
		}

		static Entity* GetMainCamera() {
			return m_pMainCamera;
		}

		static void SetBGColor(const Color& color) {
			m_BGColor = color;
		}
		
		static void SetBGColor(const float& r, const float& g, const float& b, const float& a) {
			m_BGColor = Color(r, g, b, a);
		}

		static const Color& GetBGColor() {
			return m_BGColor;
		}

		static ImGuiHandler*& GetImGuiHandler() {
			return mp_guiHandler;
		}

		static void RemoveMainCamera()
		{
			m_pMainCamera = nullptr;
		}

		static Window* GetWindow(){
			return m_pWindow;
		}

		friend class GameLoop;
		friend class Renderer;

		private:

		GlobalSettings();

		static void Initialize(Window& window) {
			m_pWindow = &window;

			// -------------------------- Initialize Default ShaderModel -------------------------- //

			m_pDefaultShader = &Resource::CreateShader("CellShader", window);

			ShaderModelDesc SMDesc;
			SMDesc.PSDir = AEN_OUTPUT_DIR_WSTR(L"CellShaderPS.cso");
			SMDesc.CSDir = AEN_OUTPUT_DIR_WSTR(L"CellShaderCS.cso");
			SMDesc.samplerType = SamplerType::CLAMP;
			SMDesc.bufferName = "CB_CellShader";

			if(!m_pDefaultShader->Create(SMDesc)) {
				SMDesc.PSDir = L"CellShaderPS.cso";
				SMDesc.CSDir = L"CellShaderCS.cso";
				SMDesc.samplerType = SamplerType::CLAMP;
				SMDesc.bufferName = "CB_CellShader";
				SMDesc.bufferName = "CB_CellShader";

				if(!m_pDefaultShader->Create(SMDesc))
					throw;
			}

			m_pDefaultShader->m_dbLayout.Add<DBType::Float4>(		"BaseColor"					);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float4>(		"ShadowColor"				);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float4>(		"SpecularColor"				);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float4>(		"RimLightColor"				);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float4>(		"InnerEdgeColor"			);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float4>(		"OuterEdgeColor"			);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float4>(		"GlowColor"					);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"GlowStr"					);
			m_pDefaultShader->m_dbLayout.Add<DBType::Int>(			"InnerEdgeThickness"		);
			m_pDefaultShader->m_dbLayout.Add<DBType::Int>(			"OuterEdgeThickness"		);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"SpecularPower"				);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"SpecularStrength"			);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"Roughness"					);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"ShadowOffset"				);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"InnerFalloff"				);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"OuterFalloff"				);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"RimLightIntensity"			);
			m_pDefaultShader->m_dbLayout.Add<DBType::Float>(		"RimLightSize"				);

			m_pDefaultShader->m_dbLayout["BaseColor"]				= Color::White;
			m_pDefaultShader->m_dbLayout["ShadowColor"]				= Color(0.3f, 0.3f, 0.5f, 1.f);
			m_pDefaultShader->m_dbLayout["SpecularColor"]			= Color::White;
			m_pDefaultShader->m_dbLayout["RimLightColor"]			= Color::White;
			m_pDefaultShader->m_dbLayout["InnerEdgeColor"]			= Color::Black;
			m_pDefaultShader->m_dbLayout["OuterEdgeColor"]			= Color::Black;
			m_pDefaultShader->m_dbLayout["GlowColor"]				= Color::White;
			m_pDefaultShader->m_dbLayout["GlowStr"]					= 100.f;
			m_pDefaultShader->m_dbLayout["InnerEdgeThickness"]		= 2;
			m_pDefaultShader->m_dbLayout["OuterEdgeThickness"]		= 2;
			m_pDefaultShader->m_dbLayout["SpecularPower"]			= 0.6f;
			m_pDefaultShader->m_dbLayout["SpecularStrength"]		= 1.f;
			m_pDefaultShader->m_dbLayout["Roughness"]				= 0.5f;
			m_pDefaultShader->m_dbLayout["ShadowOffset"]			= 0.f;
			m_pDefaultShader->m_dbLayout["InnerFalloff"]			= 0.f;
			m_pDefaultShader->m_dbLayout["OuterFalloff"]			= 0.f;
			m_pDefaultShader->m_dbLayout["RimLightIntensity"]		= 1.f;
			m_pDefaultShader->m_dbLayout["RimLightSize"]			= 0.f;

			// --------------------------------- Default Material --------------------------------- //

			Aen::Material& defaultMaterial = Resource::CreateMaterial("DefaultMaterial");
			Aen::Texture& defaultTexture = Resource::CreateTexture("DefaultTexture");
			defaultMaterial["InnerEdgeThickness"] = 3;
			defaultMaterial["OuterEdgeThickness"] = 3;
			defaultMaterial["InnerEdgeColor"] = Aen::Color::Magenta;
			defaultMaterial["OuterEdgeColor"] = Aen::Color::Magenta;
			defaultTexture.LoadTexture(AEN_RESOURCE_DIR("Missing_Textures.png"));
			defaultMaterial.SetDiffuseMap(defaultTexture);

		}

		static ShaderModel* m_pDefaultShader;
		static Window* m_pWindow;
		static Entity* m_pMainCamera;
		static Color m_BGColor;

		static ImGuiHandler* mp_guiHandler;

	};


}