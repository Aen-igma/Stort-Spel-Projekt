#pragma once
#include"Renderer.h"
#include<string>

namespace Aen {

	class AEN_DECLSPEC GlobalSettings {
		public:
		
		static void SetDefaultShader(const std::string& name) {
			m_pDefaultShader = &ResourceHandler::GetShader(name);
		}

		static ShaderModel* GetDefaultShader() {
			return m_pDefaultShader;
		}

		static void SetMainCamera(Entity& camera) {
			m_pMainCamera = &camera;
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

		friend class GameLoop;
		friend class Renderer;

		private:

		GlobalSettings();

		static void Initialize(Window& window) {
			m_pWindow = &window;

			// -------------------------- Initialize Default ShaderModel -------------------------- //

			m_pDefaultShader = &ResourceHandler::CreateShader("CellShader");

			if(!m_pDefaultShader->m_VShaderPass1.Create(AEN_OUTPUT_DIR_WSTR(L"CellShaderVS.cso")))
				if(!m_pDefaultShader->m_VShaderPass1.Create(L"CellShaderVS.cso"))
					throw;

			if(!m_pDefaultShader->m_PShaderPass1.Create(AEN_OUTPUT_DIR_WSTR(L"CellShaderPS.cso")))
				if(!m_pDefaultShader->m_PShaderPass1.Create(L"CellShaderPS.cso"))
					throw;

			if(!m_pDefaultShader->m_VShaderPass2.Create(AEN_OUTPUT_DIR_WSTR(L"CellPostVS.cso")))
				if(!m_pDefaultShader->m_VShaderPass2.Create(L"CellPostVS.cso"))
					throw;

			if(!m_pDefaultShader->m_PShaderPass2.Create(AEN_OUTPUT_DIR_WSTR(L"CellPostPS.cso")))
				if(!m_pDefaultShader->m_PShaderPass2.Create(L"CellPostPS.cso"))
					throw;

			m_pDefaultShader->m_iLayoutPass1.m_inputDesc = {
				{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BITANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UV",			0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			m_pDefaultShader->m_iLayoutPass2.m_inputDesc = {
				{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UV",        0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			m_pDefaultShader->m_iLayoutPass1.Create(m_pDefaultShader->m_VShaderPass1);
			m_pDefaultShader->m_iLayoutPass2.Create(m_pDefaultShader->m_VShaderPass2);

			m_pDefaultShader->m_dbLayout.first = 1;

			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float4>(		"BaseColor"					);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float4>(		"ShadowColor"				);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float4>(		"SpecularColor"				);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float4>(		"RimLightColor"				);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float4>(		"InnerEdgeColor"			);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float4>(		"OuterEdgeColor"			);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"InnerEdgeThickness"		);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"OuterEdgeThickness"		);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"SpecularPower"				);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"SpecularStrength"			);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"Roughness"					);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"ShadowOffset"				);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"InnerFalloff"				);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"OuterFalloff"				);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"RimLightIntensity"			);
			m_pDefaultShader->m_dbLayout.second.Add<DBType::Float>(			"RimLightSize"				);

			m_pDefaultShader->m_dbLayout.second["BaseColor"]				= Color::White;
			m_pDefaultShader->m_dbLayout.second["ShadowColor"]				= Color(0.3f, 0.3f, 0.5f, 1.f);
			m_pDefaultShader->m_dbLayout.second["SpecularColor"]			= Color::White;
			m_pDefaultShader->m_dbLayout.second["RimLightColor"]			= Color::White;
			m_pDefaultShader->m_dbLayout.second["InnerEdgeColor"]			= Color::Black;
			m_pDefaultShader->m_dbLayout.second["OuterEdgeColor"]			= Color::Black;
			m_pDefaultShader->m_dbLayout.second["InnerEdgeThickness"]		= 0.001f;
			m_pDefaultShader->m_dbLayout.second["OuterEdgeThickness"]		= 0.001f;
			m_pDefaultShader->m_dbLayout.second["SpecularPower"]			= 0.6f;
			m_pDefaultShader->m_dbLayout.second["SpecularStrength"]			= 1.f;
			m_pDefaultShader->m_dbLayout.second["Roughness"]				= 0.5f;
			m_pDefaultShader->m_dbLayout.second["ShadowOffset"]				= 0.f;
			m_pDefaultShader->m_dbLayout.second["InnerFalloff"]				= 0.f;
			m_pDefaultShader->m_dbLayout.second["OuterFalloff"]				= 0.f;
			m_pDefaultShader->m_dbLayout.second["RimLightIntensity"]		= 1.f;
			m_pDefaultShader->m_dbLayout.second["RimLightSize"]				= 0.f;
			
			m_pDefaultShader->m_samplerDataPass1.first = 0;
			m_pDefaultShader->m_samplerDataPass1.second.Create(SamplerType::WRAP);

			m_pDefaultShader->m_samplerDataPass2.first = 0;
			m_pDefaultShader->m_samplerDataPass2.second.Create(SamplerType::CLAMP);

			m_pDefaultShader->m_gBuffer.Create(window, 4);

			// -------------------------- Initialize Default ShaderModel -------------------------- //

			ShaderModel& hairShader = ResourceHandler::CreateShader("HairShader");

			if(!hairShader.m_VShaderPass1.Create(AEN_OUTPUT_DIR_WSTR(L"HairShaderVS.cso")))
				if(!hairShader.m_VShaderPass1.Create(L"HairShaderVS.cso"))
					throw;

			if(!hairShader.m_PShaderPass1.Create(AEN_OUTPUT_DIR_WSTR(L"HairShaderPS.cso")))
				if(!hairShader.m_PShaderPass1.Create(L"HairShaderPS.cso"))
					throw;

			if(!hairShader.m_VShaderPass2.Create(AEN_OUTPUT_DIR_WSTR(L"HairPostVS.cso")))
				if(!hairShader.m_VShaderPass2.Create(L"HairPostVS.cso"))
					throw;

			if(!hairShader.m_PShaderPass2.Create(AEN_OUTPUT_DIR_WSTR(L"HairPostPS.cso")))
				if(!hairShader.m_PShaderPass2.Create(L"HairPostPS.cso"))
					throw;

			hairShader.m_iLayoutPass1.m_inputDesc = {
				{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BITANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UV",			0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			hairShader.m_iLayoutPass2.m_inputDesc = {
				{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UV",        0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			hairShader.m_iLayoutPass1.Create(hairShader.m_VShaderPass1);
			hairShader.m_iLayoutPass2.Create(hairShader.m_VShaderPass2);

			hairShader.m_dbLayout.first = 1;

			hairShader.m_dbLayout.second.Add<DBType::Float4>(		"BaseColor"				);
			hairShader.m_dbLayout.second.Add<DBType::Float4>(		"ShadowColor"			);
			hairShader.m_dbLayout.second.Add<DBType::Float4>(		"SpecularColor"			);
			hairShader.m_dbLayout.second.Add<DBType::Float4>(		"RimLightColor"			);
			hairShader.m_dbLayout.second.Add<DBType::Float4>(		"InnerEdgeColor"		);
			hairShader.m_dbLayout.second.Add<DBType::Float4>(		"OuterEdgeColor"		);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"InnerEdgeThickness"	);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"OuterEdgeThickness"	);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"SpecularPower"			);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"SpecularStrength"		);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"Roughness"				);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"ShadowOffset"			);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"InnerFalloff"			);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"OuterFalloff"			);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"RimLightIntensity"		);
			hairShader.m_dbLayout.second.Add<DBType::Float>(		"RimLightSize"			);

			hairShader.m_dbLayout.second["BaseColor"]				= Color::White;
			hairShader.m_dbLayout.second["ShadowColor"]				= Color(0.3f, 0.3f, 0.5f, 1.f);
			hairShader.m_dbLayout.second["SpecularColor"]			= Color::White;
			hairShader.m_dbLayout.second["RimLightColor"]			= Color::White;
			hairShader.m_dbLayout.second["InnerEdgeColor"]			= Color::Black;
			hairShader.m_dbLayout.second["OuterEdgeColor"]			= Color::Black;
			hairShader.m_dbLayout.second["InnerEdgeThickness"]		= 0.001f;
			hairShader.m_dbLayout.second["OuterEdgeThickness"]		= 0.001f;
			hairShader.m_dbLayout.second["SpecularPower"]			= 0.6f;
			hairShader.m_dbLayout.second["SpecularStrength"]		= 1.f;
			hairShader.m_dbLayout.second["Roughness"]				= 0.5f;
			hairShader.m_dbLayout.second["ShadowOffset"]			= 0.f;
			hairShader.m_dbLayout.second["InnerFalloff"]			= 0.f;
			hairShader.m_dbLayout.second["OuterFalloff"]			= 0.f;
			hairShader.m_dbLayout.second["RimLightIntensity"]		= 1.f;
			hairShader.m_dbLayout.second["RimLightSize"]			= 0.f;

			hairShader.m_samplerDataPass1.first = 0;
			hairShader.m_samplerDataPass1.second.Create(SamplerType::WRAP);

			hairShader.m_samplerDataPass2.first = 0;
			hairShader.m_samplerDataPass2.second.Create(SamplerType::CLAMP);

			hairShader.m_gBuffer.Create(window, 4);

			// --------------------------------- Default Material --------------------------------- //

			Aen::Material& defaultMaterial = ResourceHandler::CreateMaterial("DefaultMaterial");
			Aen::Texture& defaultTexture = ResourceHandler::CreateTexture("DefaultTexture");
			defaultMaterial["InnerEdgeThickness"] = 0.0006f;
			defaultMaterial["OuterEdgeThickness"] = 0.0006f;
			defaultMaterial["InnerEdgeColor"] = Aen::Color::Magenta;
			defaultMaterial["OuterEdgeColor"] = Aen::Color::Magenta;
			defaultTexture.LoadTexture("../Resource/Missing_Textures.png");
			defaultMaterial.SetDiffuseMap(defaultTexture);
		}

		static ShaderModel* m_pDefaultShader;
		static Window* m_pWindow;
		static Entity* m_pMainCamera;
		static Color m_BGColor;
	};
}