#pragma once
#include"Tranform.h"
#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC TransformHandler {
		private:

		TransformHandler();

		static const bool TranslationExist(const uint32_t& id) {
			return m_translations.count(id) > 0;
		}

		static const bool RotationExist(const uint32_t& id) {
			return m_rotations.count(id) > 0;
		}

		static const bool ScaleExist(const uint32_t& id) {
			return m_scales.count(id) > 0;
		}

		static void CreateTranslation(const uint32_t& id) {
			m_translations.emplace(id, AEN_NEW Translation());
		}

		static void CreateRotation(const uint32_t& id) {
			m_rotations.emplace(id, AEN_NEW Rotation());
		}

		static void CreateScale(const uint32_t& id) {
			m_scales.emplace(id, AEN_NEW Scale());
		}

		static void RemoveTranform(const uint32_t& id) {
			if(m_translations.count(id) > 0) {
				delete m_translations.at(id);
				m_translations.at(id) = nullptr;
				m_translations.erase(id);
			}

			if(m_rotations.count(id) > 0) {
				delete m_rotations.at(id);
				m_rotations.at(id) = nullptr;
				m_rotations.erase(id);
			}

			if(m_scales.count(id) > 0) {
				delete m_scales.at(id);
				m_scales.at(id) = nullptr;
				m_scales.erase(id);
			}
		}

		static Translation& GetTranslation(const uint32_t& id) {
			if(m_translations.count(id) > 0)
				return *m_translations.at(id);

			throw;
		}
		
		static Rotation& GetRotation(const uint32_t& id) {
			if(m_rotations.count(id) > 0)
				return *m_rotations.at(id);

			throw;
		}
		
		static Scale& GetScale(const uint32_t& id) {
			if(m_scales.count(id) > 0)
				return *m_scales.at(id);

			throw;
		}
		
		static std::unordered_map<uint32_t, Translation*> m_translations;
		static std::unordered_map<uint32_t, Rotation*> m_rotations;
		static std::unordered_map<uint32_t, Scale*> m_scales;

		friend class Translation;
		friend class Rotation;
		friend class Scale;
		friend class Entity;
		friend class Renderer;
	};
}