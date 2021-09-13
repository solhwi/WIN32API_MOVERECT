#include "stdafx.h"
#include "Window.h"
#include "Program.h"

WinDesc Window::desc;
Program* Window::program = nullptr;

Window::Window(WinDesc desc) // Window Descriptiow(����, ����)
{
	WNDCLASSEX wndClass; // ������ â�� ����� ���� Ŭ����
	ZeroMemory(&wndClass, sizeof(wndClass)); // �ش� �ּҸ� 0���� �ʱ�ȭ

	//cbClsExtra, cbWndExtra�� ������ �����Ƿ� 0���� ����
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ������, HBRUSH - BRUSH Handle
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW); // ���콺Ŀ��, IDC_ARROW - �Ϲ�Ŀ��, �ش� ���� �ٲ��ָ� �ٸ� Ŀ���� �����
	wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO); // ���ʻ���� ������
	wndClass.hIconSm = wndClass.hIcon; // ����������
	wndClass.hInstance = desc.instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc; //������ ���ν��� - ������ �޽��� ó���Լ�
	wndClass.lpszClassName = desc.AppName.c_str(); // ������ Ŭ���� ����
	wndClass.lpszMenuName = nullptr;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Ŭ���� ��Ÿ��
	wndClass.cbSize = sizeof(WNDCLASSEX); // ����Ȯ���� ���� ������ üũ

	WORD wHr = RegisterClassEx(&wndClass); // �������Ϳ� ���
	assert(wHr != 0);

	desc.handle = CreateWindowEx // ������â ����
	(
		WS_EX_APPWINDOW,
		desc.AppName.c_str(),
		desc.AppName.c_str(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_TABSTOP, // ������ ��Ÿ��
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		(HMENU)nullptr,
		desc.instance, // ������ ������ �ν��Ͻ�
		nullptr
	);

	RECT rect = { 0,0,(LONG)desc.width, (LONG)desc.height };

	//GetSystemMetrics - ȭ���� ���� ���� ������
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	MoveWindow
	(
		desc.handle,
		centerX,
		centerY,
		rect.right - rect.left,
		rect.bottom - rect.top,
		true
	);

	ShowWindow(desc.handle, SW_SHOWNORMAL);
	SetForegroundWindow(desc.handle);
	SetFocus(desc.handle);

	ShowCursor(true);

	Window::desc = desc;

}

Window::~Window()
{
	DestroyWindow(desc.handle);
	UnregisterClass(desc.AppName.c_str(), desc.instance);
}

WPARAM Window::Run()
{
	MSG msg = { 0 };

	Keyboard::Create();
	Mouse::Create();
	Time::Create();

	Time::Get()->Start();

	program = new Program();
	while (true)
	{
		//GetMessage - �ƹ��� �޽����� ���ٸ� ���ο� �޽����� �ö����� ��� ���
		//PeekMessage - �޽����� ���ٸ� �Լ�ȣ���� �ٷ� �����
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	SAFE_DELETE(program);

	Time::Delete();
	Mouse::Delete();
	Keyboard::Delete();

	return msg.wParam;
}

HDC hdc;
HWND handle;

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);

	PAINTSTRUCT ps;
	ZeroMemory(&ps, sizeof(ps));

	switch (message)
	{
	case WM_CREATE:
	{
		::handle = handle;
		SetTimer(handle, 1, 0, nullptr);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(handle, &ps);

		Time::Get()->Print();
		Mouse::Get()->Print();

		if (program)
			program->Render();

		EndPaint(handle, &ps);

		break;
	}

	case WM_TIMER:
	{
		Time::Get()->Update();
		Keyboard::Get()->Update();
		Mouse::Get()->Update();

		program->Update();

		InvalidateRect(handle, nullptr, true);

		break;
	}
	}


	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		KillTimer(handle, NULL);
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
