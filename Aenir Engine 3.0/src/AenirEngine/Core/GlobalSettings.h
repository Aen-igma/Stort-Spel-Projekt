#pragma once
#include"Renderer.h"
#include "../ImGuiHandler.h"
#include<string>
#include "../Quadtree/Quadtree.h"
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
			UpdateFrstumGrid();
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

		static Material* GetDefaultMaterial() {
			return m_defaultMaterial;
		}

		static void SetVSync(const bool& set) {
			m_vSync = set;
		}

		static void RemoveMainCamera()
		{
			m_pMainCamera = nullptr;
		}

		static Window* GetWindow(){
			return m_pWindow;
		}

		static const bool GetVSync() {
			return m_vSync;
		}

		static void StartQuadtree(const unsigned& level,
			const unsigned& maxLevel, const unsigned& capacity) {
			if (m_pQuadtree)
			{
				delete m_pQuadtree;
				m_pQuadtree = nullptr;
			}
			else
			{
				m_pQuadtree = AEN_NEW Quadtree(level, maxLevel, capacity);
				m_pQuadtree->Initialize();
			}
		}

		static void StopQuadtree() {
			if (m_pQuadtree)
			{
				delete m_pQuadtree;
				m_pQuadtree = nullptr;
			}
		}

		static void RebuildAutoPass() 
		{
			m_pQuadtree->RebuildAutoPass();
		}

		friend class GameLoop;
		friend class Renderer;
		friend class Camera;

		private:

		static void UpdateFrstumGrid() {
			if(m_pMainCamera) {
				RenderSystem::BindShader(m_pRenderer->m_frustumGridCS);
				RenderSystem::BindUnOrderedAccessView(0u, m_pRenderer->m_frustumGrid);
				m_pRenderer->m_dispatchInfo.BindBuffer<CShader>(0u);
				m_pRenderer->m_cbTransform.BindBuffer<CShader>(1u);

				m_pRenderer->m_cbTransform.GetData().m_vMat = m_pMainCamera->GetComponent<Camera>().GetView().Transposed();
				m_pRenderer->m_cbTransform.GetData().m_pMat = m_pMainCamera->GetComponent<Camera>().GetProjecton().Transposed();
				m_pRenderer->m_cbTransform.GetData().m_ivMat = m_pRenderer->m_cbTransform.GetData().m_vMat.Inverse();
				m_pRenderer->m_cbTransform.GetData().m_ipMat = m_pRenderer->m_cbTransform.GetData().m_pMat.Inverse();
				m_pRenderer->m_cbTransform.UpdateBuffer();

				RenderSystem::Dispatch(m_pRenderer->m_dispatchInfo.GetData().threadGroups, 1u);

				RenderSystem::UnBindUnOrderedAccessViews(0u, 1u);
			}
		}


		static void Destroy() {
			delete m_defaultMaterial;
			delete m_defaultTexture;
		}

		GlobalSettings();

		static void Initialize(Window& window, Renderer* renderer) {
			m_pWindow = &window;
			m_pRenderer = renderer;

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

			m_defaultMaterial = AEN_NEW Material(true);
			m_defaultTexture = AEN_NEW Texture();
			(*m_defaultMaterial)["InnerEdgeThickness"] = 2;
			(*m_defaultMaterial)["OuterEdgeThickness"] = 2;
			(*m_defaultMaterial)["InnerEdgeColor"] = Aen::Color::Magenta;
			(*m_defaultMaterial)["OuterEdgeColor"] = Aen::Color::Magenta;
			m_defaultTexture->LoadTexture(AEN_TEXTURE_DIR("Missing_Textures.png"));
			m_defaultMaterial->SetDiffuseMap(*m_defaultTexture);

		}

		static ShaderModel* m_pDefaultShader;
		static Window* m_pWindow;
		static Entity* m_pMainCamera;
		static Color m_BGColor;

		static Material* m_defaultMaterial;
		static Texture* m_defaultTexture;

		//static ImGuiHandler* mp_guiHandler;
		static bool m_vSync;

		static Renderer* m_pRenderer;
		static Quadtree* m_pQuadtree;
	};


}