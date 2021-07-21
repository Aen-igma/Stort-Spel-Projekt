#pragma once
#include"Renderer.h"
#include<string>

namespace Aen {

	class AEN_DECLSPEC GlobalSettings {
		public:
		
		static void SetDefaultShader(const std::string& name) {
			m_pDefaultShader = &ShaderMHandler::GetShader(name);
		}

		static ShaderModel* GetDefaultShader() {
			return m_pDefaultShader;
		}

		static void SetMainCamera(Entity& camera) {
			m_pMainCamera = &camera;
		}

		friend class GameLoop;
		friend class Renderer;

		private:

		GlobalSettings();

		static void Initialize() {

			// -------------------------- Initialize Default ShaderModel -------------------------- //

			m_pDefaultShader = ShaderMHandler::CreateShader("CellShader");
			
			if(!m_pDefaultShader->m_VShader.Create(L"../bin/Debug-x64/CellShaderVS.cso"))
				if(!m_pDefaultShader->m_VShader.Create(L"CellShaderVS.cso"))
					throw;

			if(!m_pDefaultShader->m_PShader.Create(L"../bin/Debug-x64/CellShaderPS.cso"))
				if(!m_pDefaultShader->m_PShader.Create(L"CellShaderPS.cso"))
					throw;

			m_pDefaultShader->m_iLayout.m_inputDesc = {
				{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UV",        0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};

			m_pDefaultShader->m_iLayout.Create(m_pDefaultShader->m_VShader);

			std::pair<UINT, DBLayout> layout;
			layout.first = 0;

			layout.second.Add<DBType::Float4>(	"BaseColor"			);
			layout.second.Add<DBType::Float4>(	"ShadowColor"		);
			layout.second.Add<DBType::Float4>(	"SpecularColor"		);
			layout.second.Add<DBType::Float>(	"SpecularPower"		);
			layout.second.Add<DBType::Float>(	"ShadowOffset"		);
			layout.second.Add<DBType::Float>(	"OuterFalloff"		);
			layout.second.Add<DBType::Float>(	"InnerFalloff"		);
			layout.second.Add<DBType::Float>(	"RimLightIntensity"	);
			layout.second.Add<DBType::Float>(	"RimLightSize"		);

			layout.second["BaseColor"]			= Color::White;
			layout.second["ShadowColor"]		= Color(0.03f, 0.03f, 0.05f, 1.f);
			layout.second["SpecularColor"]		= Color::White;
			layout.second["SpecularPower"]		= 5.f;
			layout.second["ShadowOffset"]		= 0.5f;
			layout.second["OuterFalloff"]		= 0.f;
			layout.second["InnerFalloff"]		= 0.f;
			layout.second["RimLightIntensity"]	= 1.f;
			layout.second["RimLightSize"]		= 0.1f;

			std::pair<UINT, Sampler> sampler(0, SamplerType::WRAP);

			m_pDefaultShader->m_dbLayouts.emplace_back(layout);
			m_pDefaultShader->m_samplerDatas.emplace_back(sampler);
		}


		static ShaderModel* m_pDefaultShader;
		static Entity* m_pMainCamera;
	};
}