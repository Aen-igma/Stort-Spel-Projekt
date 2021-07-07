#pragma once
#include"../../Graphics.h"
#include<unordered_map>

namespace Aen {

	class Translation;
	class Rotation;
	class Scale;

	class TranformHandler {
		private:

		static const bool TranslationExist(const uint32_t& id) {
			return GetTranslations().count(id) > 0;
		}

		static const bool RotationExist(const uint32_t& id) {
			return GetRotations().count(id) > 0;
		}

		static const bool ScaleExist(const uint32_t& id) {
			return GetScales().count(id) > 0;
		}

		static void CreateTranslation(const uint32_t& id) {
			GetTranslations().emplace(id, AEN_NEW Translation());
		}

		static void CreateRotation(const uint32_t& id) {
			GetRotations().emplace(id, AEN_NEW Rotation());
		}

		static void CreateScale(const uint32_t& id) {
			GetScales().emplace(id, AEN_NEW Scale());
		}

		static void RemoveTranform(const uint32_t& id) {
			if(GetTranslations().count(id) > 0) {
				delete GetTranslations().at(id);
				GetTranslations().at(id) = nullptr;
				GetTranslations().erase(id);
			}

			if(GetRotations().count(id) > 0) {
				delete GetRotations().at(id);
				GetRotations().at(id) = nullptr;
				GetRotations().erase(id);
			}

			if(GetScales().count(id) > 0) {
				delete GetScales().at(id);
				GetScales().at(id) = nullptr;
				GetScales().erase(id);
			}
		}

		static Translation& GetTranslation(const uint32_t& id) {
			if(GetTranslations().count(id) > 0)
				return *GetTranslations().at(id);

			throw;
		}
		
		static Rotation& GetRotation(const uint32_t& id) {
			if(GetRotations().count(id) > 0)
				return *GetRotations().at(id);

			throw;
		}
		
		static Scale& GetScale(const uint32_t& id) {
			if(GetScales().count(id) > 0)
				return *GetScales().at(id);

			throw;
		}
		
		static std::unordered_map<uint32_t, Translation*> GetTranslations() {
			static std::unordered_map<uint32_t, Translation*> translations;
			return translations;
		}

		static std::unordered_map<uint32_t, Rotation*> GetRotations() {
			static std::unordered_map<uint32_t, Rotation*> rotations;
			return rotations;
		}

		static std::unordered_map<uint32_t, Scale*> GetScales() {
			static std::unordered_map<uint32_t, Scale*> scales;
			return scales;
		}

		friend class Translation;
		friend class Rotation;
		friend class Scale;
		friend class Entity;
	};
}