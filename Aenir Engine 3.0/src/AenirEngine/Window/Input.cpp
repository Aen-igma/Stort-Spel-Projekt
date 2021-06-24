#include"Input.h"

namespace Aen {

	unsigned char Input::keys[256];
	unsigned char Input::prevKeys[256];

	bool Input::activeGP[XUSER_MAX_COUNT];
	bool Input::GPKeys[XUSER_MAX_COUNT][14];
	bool Input::prevGPKeys[XUSER_MAX_COUNT][14];
	Vec2f Input::analogs[XUSER_MAX_COUNT][3];
	Vec2f Input::deadZone[XUSER_MAX_COUNT][3]{
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f), 
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f), 
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f), 
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f)};
	DZType Input::dzType[XUSER_MAX_COUNT][2]{};
	Vec2f Input::axisDeadZone[XUSER_MAX_COUNT][2]{
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f),
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f),
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f),
		Vec2f(0.3f, 0.3f), Vec2f(0.3f, 0.3f)};
	ADZType Input::adzType[XUSER_MAX_COUNT][2];
	float Input::scuircleR[XUSER_MAX_COUNT][2]{3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f};

	const bool Input::KeyDown(const Key& key) {
		return ((keys[(int)key] >> 7) && !(prevKeys[(int)key] >> 7));
	}

	const bool Input::KeyPress(const Key& key) {
		return (keys[(int)key] >> 7);
	}

	const bool Input::KeyUp(const Key& key) {
		return (!(keys[(int)key] >> 7) && (prevKeys[(int)key] >> 7));
	}

	const bool Input::GPKeyDown(const uint32_t& index, const GP& key) {
		return (GPKeys[index][(int)key] && !prevGPKeys[index][(int)key]);
	}

	const bool Input::GPKeyPress(const uint32_t& index, const GP& key) {
		return GPKeys[index][(int)key];
	}

	const bool Input::GPKeyUp(const uint32_t& index, const GP& key) {
		return (!GPKeys[index][(int)key] && prevGPKeys[index][(int)key]);
	}

	const bool Input::GPGetActive(const uint32_t& index) {
		return activeGP[index];
	}

	const Vec2f Input::GPGetAnalog(const uint32_t& index, const Analog& key) {
		static Vec2f out;
		static Vec2f an;
		static Vec2f dz;
		static Vec2f adz;

		an = analogs[index][(int)key];
		dz = deadZone[index][(int)key];

		if(key == Analog::TRIGGER) {
			out.x = (Abs(an.x) > dz.x) ? an.x : 0.f;
			out.y = (Abs(an.y) > dz.y) ? an.y : 0.f;
			return out;
		}

		static float mag;
		switch(dzType[index][(int)key]) {
			case Aen::DZType::ELLIPSE:
				mag = Sqrd(an.x) / Sqrd(dz.x) + Sqrd(an.y) / Sqrd(dz.y);
				out = (mag < 1.f) ? Vec2f::zero : an;
				break;
			case Aen::DZType::SQUIRCLE:
				mag = std::powf(Abs(an.x / dz.x), scuircleR[index][(int)key]) + std::powf(Abs(an.y / dz.y), scuircleR[index][(int)key]);
				out = (mag < 1.f) ? Vec2f::zero : an;
				break;
			case Aen::DZType::DIAMOND:
				mag = Abs(an.x / dz.x) + Abs(an.y / dz.y);
				out = (mag < 1.f) ? Vec2f::zero : an;
				break;
			case Aen::DZType::SQUARE:
				out = (Abs(an.x) < Abs(dz.x) && Abs(an.y) < Abs(dz.y)) ? Vec2f::zero : an;
				break;
		}

		if(mag > 0.001f) {
			adz = axisDeadZone[index][(int)key];
			switch(adzType[index][(int)key]) {
				case Aen::ADZType::RECTANGLE:
					out.x = (Abs(an.x) < Abs(adz.x)) ? 0.f : an.x;
					out.y = (Abs(an.y) < Abs(adz.y)) ? 0.f : an.y;
					break;
				case Aen::ADZType::CONE:
					/*float s = Vec2f(1.f, 0.f).Normalized() * Vec2f(1.f, adz.y).Normalized();
					float sx = Vec2f(0.f, Sign(an.y)).Normalized() * Vec2f(an.x, Sign(an.y)).Normalized();
					float sy = Vec2f(Sign(an.x), 0.f).Normalized() * Vec2f(Sign(an.x), an.y).Normalized();
					out.x = (s < sx) ? 0.f : an.x;
					out.y = (s < sy) ? 0.f : an.y;*/
					break;
			}
		}

		return out;
	}

	void Input::GPSetSquircleR(const uint32_t& index, const Analog& key, const float& r) {
		scuircleR[index][(int)key] = r;
		
	}

	void Input::GPSetDeadZone(const uint32_t& index, const Analog& key, const Vec2f& dz) {
		deadZone[index][(uint32_t)key] = dz;
	}

	void Input::GPSetDeadZoneType(const uint32_t& index, const Analog& key, const DZType& type) {
		dzType[index][(uint32_t)key] = type;
	}

	void Input::GPSetAxisDeadZone(const uint32_t& index, const Analog& key, const Vec2f& dz) {
		axisDeadZone[index][(int)key] = dz;
	}

	void Input::GPSetAxisDeadZoneType(const uint32_t& index, const Analog& key, const ADZType& type) {
		adzType[index][(int)key] = type;
	}

	void Input::Update() {
		GetKeyState(0);
		std::memcpy(prevKeys, keys, sizeof(unsigned char) * 256);
		GetKeyboardState(keys);

		static const WORD xButtons[] = {
			XINPUT_GAMEPAD_A,
			XINPUT_GAMEPAD_B,
			XINPUT_GAMEPAD_X,
			XINPUT_GAMEPAD_Y,
			XINPUT_GAMEPAD_DPAD_UP,
			XINPUT_GAMEPAD_DPAD_DOWN,
			XINPUT_GAMEPAD_DPAD_LEFT,
			XINPUT_GAMEPAD_DPAD_RIGHT,
			XINPUT_GAMEPAD_LEFT_SHOULDER,
			XINPUT_GAMEPAD_RIGHT_SHOULDER,
			XINPUT_GAMEPAD_LEFT_THUMB,
			XINPUT_GAMEPAD_RIGHT_THUMB,
			XINPUT_GAMEPAD_START,
			XINPUT_GAMEPAD_BACK
		};

		for(uint32_t i = 0; i < XUSER_MAX_COUNT; i++) {
			static XINPUT_STATE gpState;
			activeGP[i] = (XInputGetState(i, &gpState) == ERROR_SUCCESS);

			if(activeGP[i]) {
				std::memcpy(prevGPKeys[i], GPKeys[i], sizeof(bool) * 14);
				for(uint32_t j = 0; j < 14; j++)
					GPKeys[i][j] = (gpState.Gamepad.wButtons & xButtons[j]);

				analogs[i][0].x = (float)gpState.Gamepad.sThumbLX / 32768.f;
				analogs[i][0].y = (float)gpState.Gamepad.sThumbLY / 32768.f;
				analogs[i][1].x = (float)gpState.Gamepad.sThumbRX / 32768.f;
				analogs[i][1].y = (float)gpState.Gamepad.sThumbRY / 32768.f;
				analogs[i][2].x = (float)gpState.Gamepad.bLeftTrigger / 256.f;
				analogs[i][2].y = (float)gpState.Gamepad.bRightTrigger / 256.f;
			}
		}
	}
}