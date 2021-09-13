#include "stdafx.h"
#include "Systems/Window.h"

//H - handle
//INSTANCE - ����ǰ� �ִ� ������ ���α׷��� �ν��Ͻ���� ��
// prevInstance�� �׻� nullptr�̹Ƿ� �Ű澵 �ʿ� x
// param - ������ �μ�
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