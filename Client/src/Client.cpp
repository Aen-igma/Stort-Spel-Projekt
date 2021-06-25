#include"AenirEngine.h"
#include "Client.h"



Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className) {}

void Client::Start() {
}

void Client::Update(const float& deltaTime) {

	if(Aen::Input::KeyDown(Aen::Key::SPACE))
		AEN_PRINT("HOI!!!");

}
