#pragma once
#include"../../Graphics.h"
#include<unordered_map>

namespace {

	class Translation;
	class Rotation;
	class Scale;

	class TranformHandler {
		public:

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

		private:
		
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

	};
}