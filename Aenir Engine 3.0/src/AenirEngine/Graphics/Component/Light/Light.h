#pragma once
#include"../Component.h"

namespace Aen {

	enum class LightType : uint32_t {
		SpotLight,
		PointLight,
		DirectionalLight
	};

	struct SB_Light {
		Color m_color;
		Vec4f m_dist;
		Vec3f m_pos;
		float m_strength;
		Vec3f m_dir;
		float m_angle;
		LightType m_type;
		Vec3f m_pad;

		SB_Light():m_color(Color::Black), m_dist(), m_pos(), m_strength(0.f), m_dir(), m_angle(), m_type(LightType::SpotLight), m_pad(Vec3f::zero) {}

		SB_Light(const Color& color, const Vec4f& dist, const Vec3f& pos, const float& strength, const Vec3f& dir, const float& angle, const LightType& type)
			:m_color(color), m_dist(dist), m_pos(pos), m_strength(strength), m_dir(dir), m_angle(angle), m_type(type), m_pad(Vec3f::zero) {}

	};

	class AEN_DECLSPEC Light : public Component {
		public:
		virtual ~Light() = 0;
		Light() = delete;
		Light(const LightType& type, const size_t& id);
		Light(const LightType& type, const Vec4f& dist, const size_t& id);

		void SetColor(const Color& color);
		void SetColor(const float& r, const float& g, const float& b, const float& a);
		void SetStrength(const float& strength);

		protected:

		SB_Light m_light;

		friend class ComponentHandler;
		friend class Renderer;
	};

	class AEN_DECLSPEC SpotLight : public Light {
		public:
		SpotLight(const size_t& id);
		
		void SetLightDist(const Vec4f& dist);
		void SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist);
		void SetConeSize(const float& ang);

		private:
		~SpotLight() = default;

		friend class ComponentHandler;
		friend class Renderer;
		friend class LevelExporter;
		friend class ImGuiHandler;
		friend class ImGuiImporter;

	};

	class AEN_DECLSPEC PointLight : public Light {
		public:
		PointLight(const size_t& id);
		
		void SetLightDist(const Vec4f& dist);
		void SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist);

		const Vec4f GetDist();

		private:
		~PointLight() = default;

		friend class ComponentHandler;
		friend class Renderer;
		friend class LevelExporter;
		friend class ImGuiHandler;
		friend class ImGuiImporter;

	};

	class AEN_DECLSPEC DirectionalLight : public Light {
		public:
		DirectionalLight(const size_t& id);

		private:
		~DirectionalLight() = default;

		friend class ComponentHandler;
		friend class Renderer;
		friend class LevelExporter;
		friend class ImGuiHandler;
		friend class ImGuiImporter;

	};
}