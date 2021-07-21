#pragma once
#include"Material.h"
#include<unordered_map>

namespace Aen {
	
	class AEN_DECLSPEC ShaderMHandler {
		public:

		static const bool ShaderExist(const std::string& name) {
			return m_shaders.count(name) > 0;
		}

		static ShaderModel* CreateShader(const std::string& name) {
			m_shaders.emplace(name, AEN_NEW ShaderModel());
			return m_shaders.at(name);
		}

		static void RemoveShader(const std::string& name) {
			if(m_shaders.count(name) > 0) {
				delete m_shaders.at(name);
				m_shaders.at(name) = nullptr;
				m_shaders.erase(name);
			}
		}

		static ShaderModel& GetShader(const std::string& name) {
			if(m_shaders.count(name) > 0)
				return *m_shaders.at(name);

			throw;
		}

		private:

		ShaderMHandler();

		static void Destroy() {
			for(auto& i : m_shaders)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}

		static std::unordered_map<std::string, ShaderModel*> m_shaders;

		friend class Material;
		friend class GameLoop;
	};
}