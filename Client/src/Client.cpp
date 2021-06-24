#include"AenirEngine.h"
#include "Client.h"



Client::Client(const Aen::WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
	:Aen::App(desc, windowName, className) {}

void Client::Run() {
	std::cout << "HOI!!!" << std::endl;
}
