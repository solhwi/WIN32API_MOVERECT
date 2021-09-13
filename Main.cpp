#include "stdafx.h"
#include "Systems/Window.h"

//H - handle
//INSTANCE - 실행되고 있는 각각의 프로그램을 인스턴스라고 함
// prevInstance는 항상 nullptr이므로 신경쓸 필요 x
// param - 들어오는 인수
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	WinDesc desc;
	desc.AppName = L"WinApi";
	desc.instance = instance;
	desc.handle = nullptr;
	desc.width = WinMaxWidth;
	desc.height = WinMaxHeight;

	Window* window = new Window(desc);
	WPARAM wParam = window->Run();
	SAFE_DELETE(window);

	return wParam;
}