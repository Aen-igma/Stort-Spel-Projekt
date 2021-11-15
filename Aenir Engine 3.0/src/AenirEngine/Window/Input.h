#pragma once
#include"..\AenDefines.h"
#include"Window.h"
#include<Xinput.h>
#include<queue>

namespace Aen {

	enum class Key;
	enum class GP;
	enum class Analog;
	enum class DZType;
	enum class ADZType;

	class AEN_DECLSPEC MouseEvent
	{
	public:
		enum MouseInput {
			SCROLL_UP, SCROLL_DOWN,
			RAW_MOVE,
			Invalid
		};
	private:
		MouseInput m_type;
		int x, y;
	public:
		MouseEvent();
		MouseEvent(const MouseInput type, const int x, const int y);
		bool IsValid() const;
		POINT GetPos() const;
		MouseInput getInputType() const;
	};

	class AEN_DECLSPEC Input {
		public:

		static const bool KeyDown(const Key& key);
		static const bool KeyPress(const Key& key);
		static const bool KeyUp(const Key& key);
			   
		static const bool GPKeyDown(const uint32_t& index, const GP& key);
		static const bool GPKeyPress(const uint32_t& index, const GP& key);
		static const bool GPKeyUp(const uint32_t& index, const GP& key);
		static const bool GPGetActive(const uint32_t& index);

		static const Vec2f GPGetAnalog(const uint32_t& index, const Analog& key);
		static void GPSetSquircleR(const uint32_t& index, const Analog& key, const float& r);
		static void GPSetDeadZone(const uint32_t& index, const Analog& key, const Vec2f& dz);
		static void GPSetDeadZoneType(const uint32_t& index, const Analog& key, const DZType& type);
		static void GPSetAxisDeadZone(const uint32_t& index, const Analog& key, const Vec2f& dz);
		static void GPSetAxisDeadZoneType(const uint32_t& index, const Analog& key, const ADZType& type);

		static void SetMousePos(const Vec2i& pos);
		static void SetMousePos(const int& x, const int& y);
		static void SetMouseVisible(const bool& isVisible);
		static const Vec2i GetMousePos(Window& window);
		static const Vec2i GetMousePos();

		static MouseEvent ReadEvent();
		static bool MouseBufferIsEmbty();
		static void ToggleRawMouse(bool b = !m_isRawMouseOn);
		static const bool GetIsRawMouseOn();
		static const POINT GetRawMouse();

		private:
		static bool m_isRawMouseOn;
		static void SetRawMouse(int x, int y);
		static void OnWheelUp(int x, int y);
		static void OnWheelDown(int x, int y);
		static std::queue<MouseEvent> m_mouseBuffer;

		// raw mouse end
		static bool Initialize();
		static void Update();

		static unsigned char keys[256];
		static unsigned char prevKeys[256];

		static bool activeGP[XUSER_MAX_COUNT];
		static bool GPKeys[XUSER_MAX_COUNT][14];
		static bool prevGPKeys[XUSER_MAX_COUNT][14];
		static Vec2f analogs[XUSER_MAX_COUNT][3];
		static Vec2f deadZone[XUSER_MAX_COUNT][3];
		static Vec2f axisDeadZone[XUSER_MAX_COUNT][2];
		static DZType dzType[XUSER_MAX_COUNT][2];
		static ADZType adzType[XUSER_MAX_COUNT][2];
		static float scuircleR[XUSER_MAX_COUNT][2];
		
		friend class GameLoop;
		friend class Window;
	};

	enum class ADZType : int {
		NONE,
		RECTANGLE,
		CONE
	};

	enum class DZType : int {
		ELLIPSE,
		SQUARE,
		SQUIRCLE,
		DIAMOND
	};

	enum class Analog : int {
		LTHUMB,
		RTHUMB,
		TRIGGER
	};

	enum class GP : int {
		A,
		B,
		X,
		Y,
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
		LSHOULDER,
		RSHOULDER,
		LTHUMB_PRESS,
		RTHUMB_PRESS,
		START,
		SELECT
	};

	enum class Key : int {
		LMOUSE				= 0x01,
		RMOUSE				= 0x02,
		CANCEL				= 0x03,
		MBUTTON				= 0x04, 
		XMOUSE1				= 0x05,
		XMOUSE2				= 0x06, 
		BACK				= 0x08,
		TAB					= 0x09,
		CLEAR				= 0x0C,
		RETURN				= 0x0D,
		ENTER				= 0x0D,
		SHIFT				= 0x10,
		CONTROL				= 0x11,
		MENU				= 0x12,
		PAUSE				= 0x13,
		CAPITAL				= 0x14,
		KANA				= 0x15,
		HANGEUL				= 0x15,
		HANGUL				= 0x15,
		IME_ON				= 0x16,
		JUNJA				= 0x17,
		HANJA				= 0x19,
		KANJI				= 0x19,
		IME_OFF				= 0x1A,
		ESCAPE				= 0x1B,
		CONVERT				= 0x1C,
		NONCONVERT			= 0x1D,
		ACCEPT				= 0x1E,
		MODECHANGE			= 0x1F,
		SPACE				= 0x20,
		PRIOR				= 0x21,
		NEXT				= 0x22,
		END					= 0x23,
		HOME				= 0x24,
		LEFT				= 0x25,
		UP					= 0x26,
		RIGHT				= 0x27,
		DOWN				= 0x28,
		SELECT				= 0x29,
		PRINT				= 0x2A,
		EXECUTE				= 0x2B,
		SNAPSHOT			= 0x2C,
		INSERT				= 0x2D,
		DELETEKEY			= 0x2E,
		HELP				= 0x2F,
		NUM0				= 0x30,
		NUM1				= 0x31,
		NUM2				= 0x32,
		NUM3				= 0x33,
		NUM4				= 0x34,
		NUM5				= 0x35,
		NUM6				= 0x36,
		NUM7				= 0x37,
		NUM8				= 0x38,
		NUM9				= 0x39,
		A					= 0x41,
		B					= 0x42,
		C					= 0x43,
		D					= 0x44,
		E					= 0x45,
		F					= 0x46,
		G					= 0x47,
		H					= 0x48,
		I					= 0x49,
		J					= 0x4A,
		K					= 0x4B,
		L					= 0x4C,
		M					= 0x4D,
		N					= 0x4E,
		O					= 0x4F,
		P					= 0x50,
		Q					= 0x51,
		R					= 0x52,
		S					= 0x53,
		T					= 0x54,
		U					= 0x55,
		V					= 0x56,
		W					= 0x57,
		X					= 0x58,
		Y					= 0x59,
		Z					= 0x5A,
		LWIN				= 0x5B,
		RWIN				= 0x5C,
		APPS				= 0x5D,
		SLEEP				= 0x5F,
		NUMPAD0				= 0x60,
		NUMPAD1				= 0x61,
		NUMPAD2				= 0x62,
		NUMPAD3				= 0x63,
		NUMPAD4				= 0x64,
		NUMPAD5				= 0x65,
		NUMPAD6				= 0x66,
		NUMPAD7				= 0x67,
		NUMPAD8				= 0x68,
		NUMPAD9				= 0x69,
		MULTIPLY			= 0x6A,
		ADD					= 0x6B,
		SEPARATOR			= 0x6C,
		SUBTRACT			= 0x6D,
		DECIMAL				= 0x6E,
		DIVIDE				= 0x6F,
		F1					= 0x70,
		F2					= 0x71,
		F3					= 0x72,
		F4					= 0x73,
		F5					= 0x74,
		F6					= 0x75,
		F7					= 0x76,
		F8					= 0x77,
		F9					= 0x78,
		F10					= 0x79,
		F11					= 0x7A,
		F12					= 0x7B,
		F13					= 0x7C,
		F14					= 0x7D,
		F15					= 0x7E,
		F16					= 0x7F,
		F17					= 0x80,
		F18					= 0x81,
		F19					= 0x82,
		F20					= 0x83,
		F21					= 0x84,
		F22					= 0x85,
		F23					= 0x86,
		F24					= 0x87,
		NUMLOCK				= 0x90,
		SCROLL				= 0x91,
		OEM_NEC_EQUAL		= 0x92,
		OEM_FJ_JISHO		= 0x92,
		OEM_FJ_MASSHOU		= 0x93,
		OEM_FJ_TOUROKU		= 0x94,
		OEM_FJ_LOYA			= 0x95,
		OEM_FJ_ROYA			= 0x96,
		LSHIFT				= 0xA0,
		RSHIFT				= 0xA1,
		LCONTROL			= 0xA2,
		RCONTROL			= 0xA3,
		LMENU				= 0xA4,
		RMENU				= 0xA5,
		BROWSER_BACK        = 0xA6,
		BROWSER_FORWARD     = 0xA7,
		BROWSER_REFRESH     = 0xA8,
		BROWSER_STOP        = 0xA9,
		BROWSER_SEARCH      = 0xAA,
		BROWSER_FAVORITES   = 0xAB,
		BROWSER_HOME        = 0xAC,
		VOLUME_MUTE         = 0xAD,
		VOLUME_DOWN         = 0xAE,
		VOLUME_UP           = 0xAF,
		MEDIA_NEXT_TRACK    = 0xB0,
		MEDIA_PREV_TRACK    = 0xB1,
		MEDIA_STOP          = 0xB2,
		MEDIA_PLAY_PAUSE    = 0xB3,
		LAUNCH_MAIL         = 0xB4,
		LAUNCH_MEDIA_SELECT = 0xB5,
		LAUNCH_APP1         = 0xB6,
		LAUNCH_APP2         = 0xB7,
		OEM_1				= 0xBA,
		OEM_PLUS			= 0xBB,
		OEM_COMMA			= 0xBC,
		OEM_MINUS			= 0xBD,
		OEM_PERIOD			= 0xBE,
		OEM_2				= 0xBF,
		OEM_3				= 0xC0,
		GP_A				= 0xC3,
		GP_B				= 0xC4,
		GP_X				= 0xC5,
		GP_Y				= 0xC6,
		GP_RSHOULDER        = 0xC7,
		GP_LSHOULDER        = 0xC8,
		GP_LTRIGGER         = 0xC9,
		GP_RTRIGGER         = 0xCA,
		GP_DPAD_UP          = 0xCB,
		GP_DPAD_DOWN        = 0xCC,
		GP_DPAD_LEFT        = 0xCD,
		GP_DPAD_RIGHT       = 0xCE,
		GP_MENU             = 0xCF,
		GP_VIEW             = 0xD0,
		GP_LJOYTICK_BUTTON  = 0xD1,
		GP_RJOYTICK_BUTTON  = 0xD2,
		GP_LJOYTICK_UP      = 0xD3,
		GP_LJOYTICK_DOWN    = 0xD4,
		GP_LJOYTICK_LEFT    = 0xD6,
		GP_LJOYTICK_RIGHT   = 0xD5,
		GP_RJOYTICK_UP      = 0xD7,
		GP_RJOYTICK_DOWN    = 0xD8,
		GP_RJOYTICK_LEFT    = 0xDA,
		GP_RJOYTICK_RIGHT   = 0xD9,
		OEM_4				= 0xDB,
		OEM_5				= 0xDC,
		OEM_6				= 0xDD,
		OEM_7				= 0xDE,
		OEM_8				= 0xDF,
		ICO_CLEAR			= 0xE6,
		OEM_RESET			= 0xE9,
		OEM_JUMP			= 0xEA,
		OEM_PA1				= 0xEB,
		OEM_PA2				= 0xEC,
		OEM_PA3				= 0xED,
		OEM_WSCTRL			= 0xEE,
		OEM_CUSEL			= 0xEF,
		OEM_ATTN			= 0xF0,
		OEM_FINISH			= 0xF1,
		OEM_COPY			= 0xF2,
		OEM_AUTO			= 0xF3,
		OEM_ENLW			= 0xF4,
		OEM_BACKTAB			= 0xF5,
		ATTN				= 0xF6,
		CRSEL				= 0xF7,
		EXSEL				= 0xF8,
		EREOF				= 0xF9,
		PLAY				= 0xFA,
		ZOOM				= 0xFB,
		NONAME				= 0xFC,
		PA1					= 0xFD,
		OEM_CLEAR			= 0xFE
	};
}